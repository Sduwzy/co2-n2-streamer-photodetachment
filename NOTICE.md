# Source and attribution

The files in `solver_overlay/` are modified versions of chombo-discharge source
files. Their upstream reference is commit
`faf3787b04ae1341f9a16f9e4327e30d2590c815`. The exported modified versions were
collected on 2026-09-29; that is the snapshot date, not a claim about when each
individual modification was made.

The modifications cover charged-species absorption in the chemistry parser,
photon absorption-coefficient assembly, plasma source-term bookkeeping,
timestep coupling and the associated continuum radiative-transfer boundary
conditions. Application drivers derive from the upstream CdrPlasma examples
and include tabulated voltage-waveform loading.

The GNU General Public License version 3 accompanies these derived sources.
Upstream copyright attribution is preserved separately. Original chemistry
inputs and generated BOLSIG+ tables are included as numerical example inputs;
no BOLSIG+ executable or third-party journal article is distributed.

Repository packaging scripts and documentation were added on 2026-09-29 and
are distributed under GPL-3.0-or-later. Dependency licenses remain those of
their respective upstream projects. `SOURCE_MANIFEST.json` records the exact
bytes of the exported solver, drivers and numerical example files.

