# Archived examples

Run each example from its own directory so relative paths in its input and
chemistry files resolve correctly. The numerical files were copied without
changing their parameters.

| Example | Input | Driver | Role |
| --- | --- | --- | --- |
| `discrete_pure` | `pure_CO3m1E8.inputs` | discrete | Archived CO2 reference configuration |
| `discrete_mixture` | `fresh_l0_g1.inputs` | discrete | Archived 25.3% N2 mixture configuration |
| `continuum_r3_only` | `inputs/r4z.inputs` | continuum | Indirect R3 pathway with direct-release cross section zero |
| `continuum_r3_r4` | `inputs/r4m21.inputs` | continuum | Same example family with direct-release cross section `1e-21 m^2` |

The continuum pair has initial carbonate density `1e14 m^-3`, uniform electron
density `1e6 m^-3`, and N2-driven R3 cross section `1e-22 m^2`. Its neutral
fractions are CO2 = 0.7469, N2 = 0.253 and CO = 0.0001. The generic direct
release is encoded using `CO3-` as the carrier label. This is a functional test,
not a molecular assignment of that cross section to carbonate.

The discrete examples retain the earlier initial carbonate density `1e8 m^-3`
and 1% CO neutral background. They must not be interpreted as the final
species-resolved NO2- parameter set. Parameter provenance is deliberately not
inferred from a figure filename or changed to match a reported result.

The original relative source directories are listed in `source_snapshot.json`.
Compiled binaries, restart dependencies, large field outputs, exploratory
restart variants, private notes and server access information are not included.

No final species-resolved NO2- input file was identified in the exported
example collection. The general charged-absorber implementation is included,
but the example inputs are not a replacement for that production-case archive.

