#!/usr/bin/env python3
"""Check exported bytes and example dependencies; does not run the plasma solver."""
import hashlib
import json
import math
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def walk(value):
    if isinstance(value, dict):
        yield value
        for child in value.values():
            yield from walk(child)
    elif isinstance(value, list):
        for child in value:
            yield from walk(child)


def main():
    manifest = json.loads((ROOT / "SOURCE_MANIFEST.json").read_text())
    for entry in manifest:
        path = ROOT / entry["path"]
        require(path.is_file(), f"Missing source: {path}")
        require(hashlib.sha256(path.read_bytes()).hexdigest() == entry["sha256"], f"Checksum mismatch: {path}")
    count = 0
    for folder in sorted((ROOT / "examples").iterdir()):
        for file in folder.rglob("*.json"):
            model = json.loads(file.read_text(encoding="utf-8-sig"))
            total = sum(s["molar fraction"] for s in model["gas"]["neutral species"])
            require(math.isclose(total, 1.0, abs_tol=1e-10), f"Gas fractions do not sum to one: {file}")
            plasma = {s["name"] for s in model["plasma species"]}
            for item in walk(model):
                if "file" in item:
                    require((folder / item["file"]).is_file(), f"Missing table {item['file']} in {folder}")
            for reaction in model["photo reactions"]:
                if "absorption" not in reaction:
                    continue
                absorption = reaction["absorption"]
                require(absorption["type"] == "plasma species", "Unsupported absorber type")
                require(absorption["species"] in plasma, "Absorber not declared")
                require(absorption["cross section"] >= 0, "Negative cross section")
                reactants = reaction["reaction"].split("->")[0].strip().split(" + ")
                require(reactants.count(absorption["species"]) == 1, "Absorber must occur once")
            count += 1
        for file in folder.rglob("*.inputs"):
            values = {}
            for raw in file.read_text().splitlines():
                line = raw.split("#", 1)[0].strip()
                if "=" in line:
                    key, value = line.split("=", 1)
                    values[key.strip()] = value.strip()
            for key in ["CdrPlasmaJSON.chemistry_file", "StochasticAir.voltage_file"]:
                require((folder / values[key]).is_file(), f"Missing input dependency: {values[key]}")
            expected = "false" if folder.name.startswith("continuum") else "true"
            require(values["CdrPlasmaJSON.discrete_photons"] == expected, f"Driver mismatch: {file}")
    code = (ROOT / "solver_overlay/Physics/CdrPlasma/PlasmaModels/CdrPlasmaJSON/CD_CdrPlasmaJSON.cpp").read_text()
    for token in ["computeRtAbsorptionCoefficients", "reaction.getAbsorptionCrossSection() * absorberDensity",
                  "Units::c * pathKappa", "a_cdrSources[r] -= k", "a_cdrSources[p] += k"]:
        require(token in code, f"Missing charged-absorber implementation: {token}")
    print(f"PASS: {len(manifest)} source checksums; {count} chemistry files; input dependencies; photon modes; coupling signatures.")
    print("Scope: static packaging checks, not compilation or numerical validation.")


if __name__ == "__main__":
    main()
