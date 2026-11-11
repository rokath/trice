#!/usr/bin/env python3
"""Rebuild the committed F030 examples and compare their Flash and static RAM."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile


# Keep the baseline first so every reported delta has the same reference.
TARGETS = ("F030_bare", "F030_inst_rtt", "F030_inst_uart")

# Overlay only build inputs when explicitly measuring uncommitted work.
SOURCE_PATHS = ("src", "examples/F030_bare", "examples/F030_inst_rtt",
                "examples/F030_inst_uart", "cmd", "internal", "pkg", "go.mod", "go.sum")


def command(arguments, *, cwd=None, capture=False, env=None, stdout=None):
    """Propagate failed tools; capture diagnostics only when the caller needs text."""
    # Windows treats .lld as an existing extension and will not append .exe.
    # Resolve PATHEXT explicitly, using the same PATH as the child process.
    executable = shutil.which(arguments[0], path=None if env is None else env.get("PATH"))
    if executable is None:
        raise ValueError(f"Required executable is missing from PATH: {arguments[0]}")
    return subprocess.run(
        [executable, *arguments[1:]], cwd=cwd, env=env, check=True, text=True,
        encoding="utf-8", errors="replace",
        stdout=subprocess.PIPE if capture else stdout,
    ).stdout


def linker_values(text):
    """Read the literal F030 regions and minimum reservations, rejecting drift."""
    values = {}
    for name in ("FLASH", "RAM"):
        match = re.search(
            rf"\b{name}\s*\([^)]*\)\s*:\s*ORIGIN\s*=\s*(0x[\da-fA-F]+|\d+)"
            rf"\s*,\s*LENGTH\s*=\s*(0x[\da-fA-F]+|\d+)([KkMm]?)\b", text,
        )
        if not match:
            raise ValueError(f"Expected a literal {name} region in the F030 linker script")
        origin, length, unit = match.groups()
        values[name] = (int(origin, 0), int(length, 0) * {"": 1, "k": 1024, "m": 1048576}[unit.lower()])
    for name in ("Heap", "Stack"):
        match = re.search(rf"_Min_{name}_Size\s*=\s*(0x[\da-fA-F]+|\d+)\s*;", text)
        if not match:
            raise ValueError(f"Expected a literal _Min_{name}_Size in the F030 linker script")
        values[name] = int(match[1], 0)
    return values


def section_sizes(text, layout):
    """Count load bytes in Flash and allocated RAM once, separating reservations.

    GNU size's bss total includes ._user_heap_stack. Use objdump's allocation
    and load flags instead, so initialized .data counts in both Flash and RAM.
    """
    sections = []
    lines = text.splitlines()
    for index, line in enumerate(lines):
        match = re.match(r"\s*\d+\s+(\S+)\s+([\da-fA-F]+)\s+([\da-fA-F]+)\s+([\da-fA-F]+)\s+", line)
        if match:
            if index + 1 == len(lines):
                raise ValueError("Missing objdump section flags")
            name, size, vma, lma = match.groups()
            sections.append((name, int(size, 16), int(vma, 16), int(lma, 16), lines[index + 1]))
    if not sections:
        raise ValueError("No sections found in arm-none-eabi-objdump -h output")

    def inside(address, size, region):
        """Require the complete section to fit, including its final byte."""
        origin, capacity = layout[region]
        return origin <= address and address + size <= origin + capacity

    flash = static_ram = reserved_ram = 0
    for name, size, vma, lma, flags in sections:
        if "ALLOC" not in flags or size == 0:
            continue
        if not any(inside(vma, size, region) for region in ("FLASH", "RAM")):
            raise ValueError(f"Allocated section {name} does not fit in FLASH or RAM")
        if inside(vma, size, "RAM"):
            if name == "._user_heap_stack":
                reserved_ram += size
            else:
                static_ram += size
        if "LOAD" in flags:
            if not inside(lma, size, "FLASH"):
                raise ValueError(f"Load image for {name} does not fit in FLASH")
            flash += size
    if reserved_ram < layout["Heap"] + layout["Stack"]:
        raise ValueError("ELF heap/stack reservation is smaller than the linker minima")
    return {"flash": flash, "static_ram": static_ram, "reserved_ram": reserved_ram,
            "heap_min": layout["Heap"], "stack_min": layout["Stack"]}


def comparison_table(rows):
    """Put differences first and retain absolute byte counts for verification."""
    baseline = next(row for row in rows if row["target"] == TARGETS[0])
    cells = [["Example", "Delta Flash [B]", "Delta static RAM [B]", "Flash [B]", "Static RAM [B]"]]
    for row in rows:
        cells.append([row["target"], f'{row["flash"] - baseline["flash"]:+d}',
                      f'{row["static_ram"] - baseline["static_ram"]:+d}',
                      str(row["flash"]), str(row["static_ram"])])
    widths = [max(len(row[column]) for row in cells) for column in range(5)]
    formatted = [" | ".join(value.ljust(widths[column]) for column, value in enumerate(row)) for row in cells]
    formatted.insert(1, " | ".join("-" * width if column == 0 else "-" * (width - 1) + ":"
                                   for column, width in enumerate(widths)))
    return "\n".join(line.rstrip() for line in formatted) + "\n"


def arguments(argv=None):
    """Keep each compiler/framing series explicit and LTO enabled by default."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", choices=("gcc", "clang"), default="gcc")
    parser.add_argument("--framing", choices=("tcobs", "cobs"), default="tcobs")
    parser.add_argument("--lto", choices=("on", "off"), default="on")
    parser.add_argument("--worktree", action="store_true",
                        help="Overlay tracked source changes onto HEAD and record their patch hash")
    return parser.parse_args(argv)


def series_name(options):
    """Separate artifacts so another series cannot overwrite its baseline."""
    return f"{options.compiler}-{options.framing}-{'lto' if options.lto == 'on' else 'no-lto'}"


def membrowse_target(project, options):
    """Keep bare/RTT independent of the deferred UART framing setting."""
    name = f"{project}_{options.compiler}_{'lto' if options.lto == 'on' else 'no_lto'}"
    return f"{name}_{options.framing}" if project == "F030_inst_uart" else name


def overlay_sources(source, repository):
    """Copy tracked edits into the disposable snapshot, including deletions.

    The patch hash distinguishes local measurements from a committed build.
    User-owned files are only read; archived material is never overlaid.
    """
    patch = subprocess.check_output(["git", "diff", "--binary", "HEAD", "--", *SOURCE_PATHS], cwd=repository)
    changed = subprocess.check_output(["git", "diff", "--name-only", "-z", "HEAD", "--", *SOURCE_PATHS], cwd=repository)
    for name in changed.decode("utf-8").split("\0"):
        if not name:
            continue
        relative = Path(name)
        if relative.is_absolute() or ".." in relative.parts or "obsolete" in relative.parts:
            raise ValueError(f"Cannot overlay source path: {name}")
        origin, destination = repository / relative, source / relative
        if origin.is_file():
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(origin, destination)
        else:
            destination.unlink(missing_ok=True)
    return hashlib.sha256(patch).hexdigest() if patch else None


def clang_make_options():
    """Use LLD for LLVM LTO and the same GNU ARM startup objects and libraries.

    GCC's nano.specs are a driver feature Clang cannot read. Resolve the
    matching Cortex-M0 resources explicitly instead of changing the linker
    script or silently omitting the CubeMX startup/runtime objects.
    """
    resources = {}
    for name in ("crti.o", "crtbegin.o", "crt0.o", "crtend.o", "crtn.o",
                 "libc_nano.a", "libm.a", "libnosys.a", "libgcc.a"):
        value = command(["arm-none-eabi-gcc", "-mcpu=cortex-m0", "-mthumb",
                         f"-print-file-name={name}"], capture=True).strip()
        path = Path(value).resolve()
        if not path.is_file():
            raise ValueError(f"ARM runtime file {name} is unavailable at {value}")
        resources[name] = path.as_posix()
    include = next((parent.parent / "include" for parent in Path(resources["libc_nano.a"]).parents
                    if parent.name == "lib" and (parent.parent / "include/stdlib.h").is_file()), None)
    if include is None:
        raise ValueError("Newlib stdlib.h was not found beside the pinned ARM libraries")
    compiler = shlex.join(["clang", "--target=arm-none-eabi", "-isystem", include.as_posix()])
    startup = shlex.join([resources[name] for name in ("crti.o", "crtbegin.o", "crt0.o")])
    libraries = shlex.join([resources[name] for name in ("libc_nano.a", "libm.a", "libnosys.a", "libgcc.a")])
    finish = shlex.join([resources[name] for name in ("crtend.o", "crtn.o")])
    flags = ("$(MCU) $(OPT) $(LTO_FLAGS) $(DEBUG_FLAGS) -ffunction-sections -fdata-sections "
             "-fuse-ld=lld -nostdlib -T$(LDSCRIPT) "
             "-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections "
             f"{startup} -Wl,--start-group {libraries} -Wl,--end-group {finish}")
    return [f"CC={compiler}", f"AS={compiler} -x assembler-with-cpp", "LISTING_FLAGS=", f"LFLAGS={flags}"]


def check_build_plan(plan, target, options):
    """Reject stale Makefiles instead of labeling a non-LTO build as LTO."""
    commands = [shlex.split(line) for line in plan.splitlines() if " -c " in line or f"/{target}.elf" in line]
    compiled = [tokens for tokens in commands if "-c" in tokens and any(token.endswith(".c") for token in tokens)]
    linked = [tokens for tokens in commands if "-o" in tokens and any(token.endswith(f"/{target}.elf") for token in tokens)]
    framing = f"-DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_{options.framing.upper()}"
    if not compiled or len(linked) != 1:
        raise ValueError(f"Expected C compilation and one ELF link in the {target} build plan")
    for tokens in compiled + linked:
        if ("-flto" in tokens) != (options.lto == "on") or "-Og" not in tokens:
            raise ValueError(f"{target} does not apply the requested -Og/LTO settings. Commit the synchronized Makefiles or use --worktree.")
        if not {"-ffunction-sections", "-fdata-sections"}.issubset(tokens):
            raise ValueError(f"{target} must preserve function/data sections during compilation and the LTO link")
        if not {"-g", "-gdwarf-2"}.issubset(tokens):
            raise ValueError(f"{target} must preserve matching DWARF 2 debug information during compilation and linking")
    if any(framing not in tokens for tokens in compiled):
        raise ValueError(f"{target} does not apply the common framing option. Commit the synchronized Makefiles or use --worktree.")


def build_comparison(options):
    """Build each series from the same snapshot with identical common settings."""
    os.chdir(Path(__file__).resolve().parent.parent)
    repository = Path.cwd()
    output = Path("temp/memory-usage") / series_name(options)
    output.mkdir(parents=True, exist_ok=True)
    pins = Path("scripts/test-tools.env").read_text(encoding="utf-8")
    expected = re.search(r"^TRICE_ARM_GCC_VERSION=(\S+)$", pins, re.MULTILINE)[1]
    actual = command(["arm-none-eabi-gcc", "-dumpfullversion"], capture=True).strip()
    if actual != expected:
        raise ValueError(f"ARM GCC {expected} is required; found {actual}. Select the pinned ARM bundle first.")
    arm_compiler = command(["arm-none-eabi-gcc", "--version"], capture=True).splitlines()[0]
    compiler, linker = arm_compiler, None
    # Pass framing to all three projects; bare and direct RTT do not use it.
    make_options = ["SHELL=sh", "OPT=-Og", f"LTO={int(options.lto == 'on')}",
                    f"TRICE_FLAGS=-DTRICE_DEFERRED_OUT_FRAMING=TRICE_FRAMING_{options.framing.upper()}"]
    if options.compiler == "clang":
        compiler = command(["clang", "--version"], capture=True).splitlines()[0]
        host = "WINDOWS" if os.name == "nt" else "DARWIN" if sys.platform == "darwin" else "LINUX"
        clang_version = re.search(r"clang version ([\d.]+)", compiler)
        expected_clang = re.search(rf"^TRICE_CLANG_{host}_VERSION=(\S+)$", pins, re.MULTILINE)[1]
        if clang_version is None or clang_version[1] != expected_clang:
            raise ValueError(f"Clang {expected_clang} is required on {host}; found {compiler}")
        linker = command(["ld.lld", "--version"], capture=True).strip()
        if not re.search(rf"\bLLD {re.escape(expected_clang)}\b", linker):
            raise ValueError(f"LLD {expected_clang} must match Clang; found {linker}")
        make_options += clang_make_options()
    commit = command(["git", "rev-parse", "HEAD"], capture=True).strip()
    # Failed rebuilds must not leave an earlier summary looking like a new result.
    for name in ("memory-usage.json", "memory-usage.md"):
        (output / name).unlink(missing_ok=True)
    archive = output / "source.tar"
    command(["git", "archive", "--format=tar", f"--output={archive}", commit])
    rows = []
    source_patch = None
    try:
        with tempfile.TemporaryDirectory(prefix="source-", dir=output) as temporary:
            source = Path(temporary).resolve()
            # Relative paths avoid MSYS backslash parsing and tar's treatment
            # of a Windows drive prefix as a remote archive host.
            command(["tar", "-xf", archive.name, "-C", source.name], cwd=output)
            if options.worktree:
                source_patch = overlay_sources(source, repository)
            # This CLI comes from the same committed sources as all three ELFs.
            executable = "trice.exe" if os.name == "nt" else "trice"
            binary = source / "temp/memory-bin" / executable
            binary.parent.mkdir(parents=True)
            command(["go", "build", "-buildvcs=false", "-o", str(binary), "./cmd/trice"], cwd=source)
            environment = dict(os.environ, PATH=str(binary.parent) + os.pathsep + os.environ["PATH"])
            for target in TARGETS:
                project = source / "examples" / target
                plan = command(["make", "-n", "-B", *make_options], cwd=project, capture=True)
                check_build_plan(plan, target, options)
                # A fresh snapshot plus clean forbids reuse of objects from a different series.
                command(["make", "clean"], cwd=project, env=environment)
                command(["sh", "build.sh", *make_options], cwd=project, env=environment)
                destination = output / target
                destination.mkdir(exist_ok=True)
                (destination / "build-plan.txt").write_text(plan, encoding="utf-8")
                for relative in (f"out/{target}.elf", f"out/{target}.map", "STM32F030R8Tx_FLASH.ld"):
                    artifact = project / relative
                    if not artifact.is_file() or artifact.stat().st_size == 0:
                        generated = ", ".join(path.name for path in (project / "out").glob("*")) or "(none)"
                        raise ValueError(f"Missing or empty build artifact: {artifact}; compiler: {compiler}; out/ contains: {generated}")
                    shutil.copy2(artifact, destination / artifact.name)
                layout = linker_values((project / "STM32F030R8Tx_FLASH.ld").read_text(encoding="utf-8"))
                sections = command(["arm-none-eabi-objdump", "-h", f"out/{target}.elf"], cwd=project, capture=True)
                (destination / "sections.txt").write_text(sections, encoding="utf-8")
                symbols = command(["arm-none-eabi-nm", "-S", "--size-sort", f"out/{target}.elf"], cwd=project, capture=True)
                (destination / "symbols.txt").write_text(symbols, encoding="utf-8")
                rows.append(dict(target=target, **section_sizes(sections, layout),
                                 membrowse_target=membrowse_target(target, options),
                                 elf_sha256=hashlib.sha256((destination / f"{target}.elf").read_bytes()).hexdigest()))
    finally:
        archive.unlink(missing_ok=True)
    metadata = dict(commit=commit, source_patch_sha256=source_patch, compiler=compiler,
                    arm_runtime=arm_compiler, linker=linker, optimization="-Og",
                    lto=options.lto == "on", framing=options.framing, linker_gc=True, targets=rows)
    (output / "memory-usage.json").write_text(json.dumps(metadata, indent=2) + "\n", encoding="utf-8")
    source_note = f"; tracked source patch `{source_patch}`" if source_patch else ""
    summary = (f"Commit: `{commit}`{source_note}\n\nCompiler: `{compiler}`; `-Og`, "
               f"LTO {options.lto}, {options.framing.upper()}, `--gc-sections`.\n\n" + comparison_table(rows))
    (output / "memory-usage.md").write_text(summary, encoding="utf-8")
    print(summary)
    print(f"Reports, ELF files, maps and linker scripts: {output}")


if __name__ == "__main__":
    try:
        build_comparison(arguments())
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f"Memory comparison failed: {error}", file=sys.stderr)
        sys.exit(1)
