#!/usr/bin/env python3
"""Prepare a new pinned upstream checkout and apply the exported source overlay."""
import argparse
import json
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def run(*args):
    subprocess.run(args, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", type=Path, required=True)
    args = parser.parse_args()
    target = args.target.expanduser().resolve()
    if target.exists():
        parser.error("Target exists; choose a new directory. Nothing was overwritten.")
    metadata = json.loads((ROOT / "docs/source_snapshot.json").read_text())
    target.parent.mkdir(parents=True, exist_ok=True)
    run("git", "init", str(target))
    run("git", "-C", str(target), "remote", "add", "origin", metadata["upstream_url"])
    run("git", "-C", str(target), "fetch", "--depth=1", "origin", metadata["upstream_commit"])
    run("git", "-C", str(target), "checkout", "--detach", "FETCH_HEAD")
    run("git", "-C", str(target), "submodule", "update", "--init", "--recursive", "--depth=1")
    for entry in metadata["submodules"]:
        commit, relative, *_ = entry.split()
        module = target / relative
        current = subprocess.check_output(["git", "-C", str(module), "rev-parse", "HEAD"], text=True).strip()
        if current != commit:
            run("git", "-C", str(module), "fetch", "--depth=1", "origin", commit)
            run("git", "-C", str(module), "checkout", "--detach", commit)
    for source in (ROOT / "solver_overlay").rglob("*"):
        if source.is_file():
            dest = target / source.relative_to(ROOT / "solver_overlay")
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, dest)
    for mode in ["discrete", "continuum"]:
        app = target / "Exec/Examples/CdrPlasma" / ("CO2_N2_" + mode)
        shutil.copytree(ROOT / "applications" / mode, app)
    print(f"Prepared {target}. See docs/BUILD.md for local toolchain configuration.")


if __name__ == "__main__":
    main()

