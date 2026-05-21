#!/usr/bin/env python3
"""
riscv_run.py - Run RISC-V ELF files or assemble and run .asm files
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


def die(msg: str, code: int = 1) -> None:
    print(f"[ERROR] {msg}", file=sys.stderr)
    raise SystemExit(code)


def info(msg: str) -> None:
    print(f"[INFO] {msg}")


def which_or_env(env_name: str, default: str) -> str:
    return os.environ.get(env_name, shutil.which(default) or default)


def run_cmd(cmd: list[str]) -> None:
    try:
        subprocess.run(cmd, check=True)
    except FileNotFoundError:
        die(f"Command not found: {cmd[0]}")
    except subprocess.CalledProcessError as e:
        die(f"Command failed with exit code {e.returncode}: {' '.join(cmd)}", e.returncode)


def resolve_executor() -> str:
    exec_path = os.environ.get("RISCV_EXECUTOR", os.path.join("build", "riscv_executor"))
    p = Path(exec_path)

    if p.exists():
        return str(p)

    if sys.platform.startswith("win"):
        exe = p.with_suffix(".exe")
        if exe.exists():
            return str(exe)

    return str(p)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Run RISC-V ELF files or assemble and run .asm files"
    )
    parser.add_argument("filename", help="Input file (.elf or .asm)")
    args = parser.parse_args()

    filename = Path(args.filename)
    if not filename.exists():
        die(f"File not found: {filename}")

    assembler = which_or_env("RISCV_AS", "riscv64-linux-gnu-as")
    linker = which_or_env("RISCV_LD", "riscv64-linux-gnu-ld")
    executor = resolve_executor()

    suffix = filename.suffix.lower()
    basename = filename.with_suffix("")

    if suffix == ".elf":
        info(f"Running ELF file: {filename}")
        if not Path(executor).exists():
            die(f"Executor not found: {executor}")
        run_cmd([executor, str(filename)])
        return 0

    if suffix == ".asm":
        info(f"Assembling: {filename}")
        obj_file = basename.with_suffix(".o")
        elf_file = basename.with_suffix(".elf")

        run_cmd([assembler, "-o", str(obj_file), str(filename)])

        info(f"Linking: {obj_file}")
        run_cmd([linker, "-o", str(elf_file), str(obj_file)])

        info(f"Created: {elf_file}")
        info(f"Running: {elf_file}")

        if not Path(executor).exists():
            die(f"Executor not found: {executor}")
        run_cmd([executor, str(elf_file)])
        return 0

    die(f"Unsupported file type: {filename} (supported: .elf, .asm)")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
