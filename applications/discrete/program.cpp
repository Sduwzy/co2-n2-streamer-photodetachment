#include "CD_Driver.H"
#include <CD_GeoCoarsener.H>
#include <CD_FieldSolverFactory.H>
#include <CD_FieldSolverMultigrid.H>
#include <CD_CdrLayoutImplem.H>
#include <CD_CdrCTU.H>
#include <CD_RtLayoutImplem.H>
#include <CD_McPhoto.H>
#include <CD_CdrPlasmaJSON.H>
#include <CD_Vessel.H>
#include <CD_CdrPlasmaGodunovStepper.H>
#include <CD_CdrPlasmaStreamerTagger.H>
#include <ParmParse.H>

#include <fstream>
#include <vector>
#include <algorithm>

// Voltage lookup table (loaded from file)
std::vector<Real> g_time_s;
std::vector<Real> g_volt_V;

Real
voltageCurve(const Real a_time)
{
  // Clamp to data range
  if (a_time <= g_time_s.front()) return g_volt_V.front();
  if (a_time >= g_time_s.back())  return g_volt_V.back();

  // Linear interpolation
  auto it = std::lower_bound(g_time_s.begin(), g_time_s.end(), a_time);
  int  i  = (it - g_time_s.begin()) - 1;
  Real f  = (a_time - g_time_s[i]) / (g_time_s[i + 1] - g_time_s[i]);

  return g_volt_V[i] + f * (g_volt_V[i + 1] - g_volt_V[i]);
}

using namespace ChomboDischarge;
using namespace Physics::CdrPlasma;

int
main(int argc, char* argv[])
{

#ifdef CH_MPI
  MPI_Init(&argc, &argv);
#endif

  // Build class options from input script and command line options
  const std::string input_file = argv[1];
  ParmParse         pp(argc - 2, argv + 2, NULL, input_file.c_str());

  // Get settings from input script
  std::string basename;
  {
    ParmParse pp("StochasticAir");
    pp.get("basename", basename);
    setPoutBaseName(basename);

    // Load voltage waveform from file
    // File format: two columns, time (ns) and voltage (kV)
    std::string voltage_file;
    pp.get("voltage_file", voltage_file);

    std::ifstream ifs(voltage_file);
    if (!ifs.is_open()) {
      MayDay::Error(("voltageCurve: could not open voltage file " + voltage_file).c_str());
    }

    Real t_ns, v_kV;
    while (ifs >> t_ns >> v_kV) {
      g_time_s.push_back(t_ns * 1.0e-9); // ns -> s
      g_volt_V.push_back(v_kV * 1.0e3);  // kV -> V
    }
    ifs.close();

    if (g_time_s.size() < 2) {
      MayDay::Error("voltageCurve: voltage file has fewer than 2 data points");
    }

    pout() << "Loaded voltage waveform: " << g_time_s.size() << " points, "
           << "t = [" << g_time_s.front() * 1e9 << ", " << g_time_s.back() * 1e9 << "] ns, "
           << "V = [" << *std::min_element(g_volt_V.begin(), g_volt_V.end()) * 1e-3 << ", "
           << *std::max_element(g_volt_V.begin(), g_volt_V.end()) * 1e-3 << "] kV"
           << std::endl;
  }

  // Set geometry and AMR
  RefCountedPtr<ComputationalGeometry> compgeom   = RefCountedPtr<ComputationalGeometry>(new Vessel());
  RefCountedPtr<AmrMesh>               amr        = RefCountedPtr<AmrMesh>(new AmrMesh());
  RefCountedPtr<GeoCoarsener>          geocoarsen = RefCountedPtr<GeoCoarsener>(new GeoCoarsener());

  // Set up physics
  RefCountedPtr<CdrPlasmaPhysics> physics     = RefCountedPtr<CdrPlasmaPhysics>(new CdrPlasmaJSON());
  RefCountedPtr<CdrPlasmaStepper> timestepper = RefCountedPtr<CdrPlasmaStepper>(new CdrPlasmaGodunovStepper(physics));
  RefCountedPtr<CellTagger>       tagger      = RefCountedPtr<CellTagger>(
    new CdrPlasmaStreamerTagger(physics, timestepper, amr, compgeom));

  // Create solver factories
  auto poi_fact = new FieldSolverFactory<FieldSolverMultigrid>();
  auto cdr_fact = new CdrFactory<CdrSolver, CdrCTU>();
  auto rte_fact = new RtFactory<RtSolver, McPhoto>();

  // Instantiate solvers
  auto poi = poi_fact->newSolver();
  auto cdr = cdr_fact->newLayout(physics->getCdrSpecies());
  auto rte = rte_fact->newLayout(physics->getRtSpecies());

  // Send solvers to TimeStepper
  timestepper->setFieldSolver(poi);
  timestepper->setCdrSolvers(cdr);
  timestepper->setRadiativeTransferSolvers(rte);

  // Set voltage
  timestepper->setVoltage(voltageCurve);

  // Set up the Driver and run it
  RefCountedPtr<Driver> engine = RefCountedPtr<Driver>(new Driver(compgeom, timestepper, amr, tagger, geocoarsen));
  engine->setupAndRun(input_file);

  // Clean up memory
  delete poi_fact;
  delete cdr_fact;
  delete rte_fact;

#ifdef CH_MPI
  CH_TIMER_REPORT();
  MPI_Finalize();
#endif
}

