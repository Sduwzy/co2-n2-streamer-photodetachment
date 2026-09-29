# CO2/N2 streamer simulation and anion photoreactions

Source code for CO2/N2 streamer calculations based on
[chombo-discharge](https://github.com/chombo-discharge/chombo-discharge).
This repository provides the charged-absorber extension, application drivers,
and representative input files. It is a code release, not a manuscript-source
repository.

## Contents

| Directory | Contents |
| --- | --- |
| `solver_overlay/` | 23 modified solver files, including plasma-dependent photon absorption and reaction source terms |
| `applications/discrete/` | Discrete-photon streamer driver |
| `applications/continuum/` | Continuum `EddingtonSP1` streamer driver |
| `examples/` | Four archived example configurations, their chemistry, voltage waveform and transport tables |
| `docs/` | Implementation notes, example scope and source-version record |
| `scripts/` | Pinned-source preparation and package checks |

The continuum extension evaluates `S = c * sigma * n_absorber * Psi`, adds
`sigma * n_absorber` to the photon absorption coefficient, and applies the
corresponding plasma-reactant sinks and product sources. The absorber is selected
in the chemistry JSON rather than hard-coded as one ion. See
[the implementation notes](docs/PHOTOREACTIONS.md).

## Source version

The exported files were read from the development solver on 29 September 2026.
They are supplied as an overlay on the exact upstream commit
`faf3787b04ae1341f9a16f9e4327e30d2590c815`, with pinned submodule revisions.
No simulation physics was rewritten for this release. The upstream source is
retrieved by the preparation script; it is not necessary to copy compiled
libraries or server executables.

```bash
python3 scripts/verify_package.py
python3 scripts/prepare_solver.py --target /path/to/new/co2-solver
```

Then follow [BUILD.md](docs/BUILD.md) to configure the Linux toolchain, compile
the appropriate driver and run an example.

## Example scope

The archived inputs demonstrate the discrete CO2/mixture calculations and the
generic indirect/direct photon-release comparison. Their original numerical
parameters are preserved. They are **not a complete, figure-by-figure archive of
the final manuscript calculations**. In particular, the supplied continuum
examples use the generic carbonate-labelled carrier and are not the final
species-resolved NO2- production inputs. See [EXAMPLES.md](docs/EXAMPLES.md).

Release checks verify source checksums, configuration parsing, referenced input
tables and the presence of the coupled absorption/source implementation. They do
not constitute a new numerical validation or a rerun of the reported trajectories.

## Licensing and attribution

The solver is derived from chombo-discharge. Its GPLv3 license and copyright
notice are retained in `LICENSE` and `UPSTREAM_COPYRIGHT.txt`; see also
[NOTICE.md](NOTICE.md). The upstream citation is retained in
`docs/upstream_CITATION.cff`.

