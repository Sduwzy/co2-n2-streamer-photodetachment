# Building and running

## Requirements

Use Linux with Git, Python 3.9 or later, GNU make, a compatible C++/Fortran
toolchain, MPI and HDF5. Chombo configuration must match the installed MPI and
HDF5 libraries. This release does not contain machine-specific binaries or a
preconfigured compiler installation.

The upstream installation guide is
<https://chombo-discharge.github.io/chombo-discharge/Base/Installation.html>.
Use the pinned checkout prepared below rather than replacing it with the
latest upstream branch.

## Prepare the source

From this repository:

```bash
python3 scripts/verify_package.py
python3 scripts/prepare_solver.py --target "$HOME/co2-solver"
export DISCHARGE_HOME="$HOME/co2-solver"
export CHOMBO_HOME="$DISCHARGE_HOME/Submodules/Chombo-3.3/lib"
```

The preparation script refuses to overwrite an existing directory. It fetches
the upstream commit, initializes the pinned dependencies, applies the 23
exported source files and installs both application directories. It does not
compile, run simulations, or modify the numerical example parameters.

Configure `CHOMBO_HOME/mk/Make.defs.local` and the chombo-discharge build
settings for the available compiler/MPI/HDF5 installation, following the
installation instructions in the pinned checkout. Then build the application:

```bash
cd "$DISCHARGE_HOME/Exec/Examples/CdrPlasma/CO2_N2_continuum"
make -j4 DIM=2 MPI=TRUE OPT=HIGH DEBUG=FALSE program
```

For the discrete driver, use `CO2_N2_discrete` instead. The original makefiles
build the discharge and CdrPlasma libraries as dependencies.

## Run an example

Set `EXE` to the actual executable produced by the appropriate build. Keep the
repository path separate from the solver checkout:

```bash
export REPO=/path/to/CO2_N2_Streamer_Code
export EXE=/absolute/path/to/compiled/program2d.Linux.64.mpic++.gfortran.OPTHIGH.MPI.ex
cd "$REPO/examples/continuum_r3_r4"
mkdir -p logs runs/r4m21
mpirun -np 4 "$EXE" inputs/r4m21.inputs
```

Use the selected example input to check its output directory and
`StochasticAir.basename`; create their parent directories before launch.
Start with a small MPI allocation and assess memory requirements before a full
production run. No long-running jobs are launched automatically by this package.

For time-dependent comparisons, read `Driver.start_time` in the specific
input. Raw solver time and time relative to the start of a case are distinct.
The voltage table uses ns and kV; the driver converts them to seconds and volts.

## Verification scope

`verify_package.py` checks the archive and configuration consistency without
requiring Chombo. It is not a substitute for compilation, mesh convergence or
a trajectory comparison. A clean full solver rebuild and complete production
rerun were not performed during this source-code packaging step.
