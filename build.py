#!/usr/bin/env python3

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Final

ROOT: Final[Path] = Path(__file__).parent.resolve()
BUILD: Final[Path] = ROOT / "build"


def run(cmd: list[str], cwd: Path | None = None) -> None:
    print(">>", " ".join(cmd))
    subprocess.check_call(cmd, cwd=cwd)


def clean() -> None:
    if BUILD.exists():
        print(f"Removing {BUILD}")
        shutil.rmtree(BUILD)
    else:
        print("Build directory does not exist")


def configure() -> None:
    BUILD.mkdir(exist_ok=True)
    run(["cmake", "-G", "Ninja", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", ".."], cwd=BUILD)


def is_multi_config() -> bool:
    cache: Path = BUILD / "CMakeCache.txt"
    if not cache.exists():
        return False
    return any(
        line.startswith("CMAKE_CONFIGURATION_TYPES")
        for line in cache.read_text(encoding="utf-8").splitlines()
    )


def build(config: str) -> None:
    if not config:
        print("Error: --config must be specified when building")
        sys.exit(1)

    if not BUILD.exists():
        configure()

    cmd: list[str] = ["cmake", "--build", "."]
    if is_multi_config():
        cmd.extend(["--config", config])

    run(cmd, cwd=BUILD)


def main() -> None:
    parser = argparse.ArgumentParser(description="Build project.")
    parser.add_argument(
        "--clean", action="store_true", help="Clean the build directory"
    )
    parser.add_argument(
        "--build", action="store_true", help="Build the project (requires --config)"
    )
    parser.add_argument(
        "--config", type=str, help="Build configuration (Debug/Release)"
    )

    args = parser.parse_args()

    # ---- Enforce forbidden usage ----
    if args.config and not args.build:
        print("Error: --config can only be used together with --build")
        sys.exit(1)

    # ---- Execute actions ----
    if args.clean:
        clean()

    if args.build:
        if not args.config:
            print("Error: --config must be specified when building")
            sys.exit(1)
        build(args.config)

    if not args.clean and not args.build:
        parser.print_help()


if __name__ == "__main__":
    main()
