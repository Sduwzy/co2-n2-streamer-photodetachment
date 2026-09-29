# Charged-absorber photon coupling

## Implementation map

- `CD_CdrPlasmaJSON.cpp`: `parsePhotoReactions` reads the `absorption` block;
  `computeRtAbsorptionCoefficients` adds the evolving plasma opacity;
  `addPhotoIonization` applies plasma sinks and sources.
- `CD_CdrPlasmaPhotoReactionJSON.H/.cpp`: stores the absorbing-species index
  and reaction cross section.
- `CD_CdrPlasmaStepper.cpp`: assembles cell-dependent absorption coefficients
  and supplies them to the radiative-transfer solver.
- `CD_EddingtonSP1.H/.cpp`: accepts the updated coefficient field.
- The Godunov and ImExSdc timestepper overlays retain the corresponding coupling.

All paths are preserved beneath `solver_overlay/`. The implementation is an
extension of the upstream solver, not a standalone photodetachment simulator.

## Rate and opacity

For absorber A and photon group g, the implementation uses

```text
kappa_g = kappa_background,g + sum_A(sigma_A,g * max(n_A, 0))
S_A,g   = c * sigma_A,g * max(n_A, 0) * Psi_g
```

`Psi_g` is the photon-number-density field in the continuum solver. The
absorbing plasma species is consumed, the specified plasma products are
created, and the photon field is attenuated through `kappa_g`. Photon
attenuation is not a second independently prescribed electron source.

The `absorption` path requires `CdrPlasmaJSON.discrete_photons = false`.
The parser rejects this extension when discrete photons are selected.

## Chemistry interface

The following is an **interface illustration**, not an archived production
case or a complete reaction network:

```json
{
  "reaction": "Yr3N2 + NO2- -> e + (NO2)",
  "absorption": {
    "type": "plasma species",
    "species": "NO2-",
    "cross section": 9.8e-22
  }
}
```

The absorbing species must also be declared in `plasma species` with its charge,
transport choices and initial condition. The photon group must be declared in
`photon species` and supplied by the appropriate excitation/emission network.
Parenthesized neutral products follow the upstream JSON interface convention.

The illustrative cross section is in square metres. Its meaning depends on
the spectral normalization of the photon group: a value referenced to the
337.1 nm band must not be applied to a total-SPS photon density without the
corresponding change of normalization. The archive examples retain their own
photon-production settings; they have not been silently converted to the final
species-resolved manuscript configuration.

