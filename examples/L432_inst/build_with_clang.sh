#!/usr/bin/env bash
# Build the L432 firmware with Clang on Linux, macOS or Windows (Git Bash).
# Run bash ./build_with_clang.sh 98 for the minimum-overhead configuration.
# The number is required: this script never changes the default in triceConfig.h.
set -euo pipefail

# Keep installation and recovery instructions in one place. Every script-level
# failure preserves its exit status and points to that Reference Manual section.
fail() {
  local status=$1
  shift
  printf 'error: %s\n' "$1" >&2
  shift
  if [ "$#" -gt 0 ]; then printf '%s\n' "$@" >&2; fi
  printf 'See the Reference Manual: docs/rm/examples.md#l432-clang-prerequisites (from the repository root).\n' >&2
  exit "$status"
}

# Keep help independent of installed compilers and generated Trice headers.
usage() {
  cat <<'HELP'
Usage: bash ./build_with_clang.sh <configuration>
       bash ./build_with_clang.sh --help
       bash ./build_with_clang.sh --check

Choose a configuration from 0 through 100, for example:
  bash ./build_with_clang.sh 1
  bash ./build_with_clang.sh 98
CONFIGURATION=98 is also accepted, like the GCC build.sh argument.

--check checks prerequisites without preparing sources or building firmware.
The build uses -O3 and writes out.clang/L432KC.{elf,hex,bin}; it does not flash.
Prerequisites and troubleshooting: docs/rm/examples.md#l432-clang-prerequisites
(from the repository root).
HELP
}

# Reject invalid numbers before running any preparation that can write files.
if [ "$#" -eq 1 ] && { [ "$1" = --help ] || [ "$1" = -h ]; }; then
  usage
  exit 0
fi
if [ "$#" -ne 1 ]; then
  usage >&2
  exit 2
fi
# A prerequisite-only run needs no configuration and exits before Bind or Make.
check_only=0
if [ "$1" = --check ]; then
  check_only=1
else
  configuration=${1#CONFIGURATION=}
  case "$configuration" in
    0 | [1-9] | [1-9][0-9] | 100) ;;
    *) fail 2 "configuration must be a decimal number from 0 through 100; received: $1" ;;
  esac
fi

# All later project paths are relative to this directory, even when the caller
# starts the script from the repository root or a different working directory.
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

# Bash 3.2 is the system Bash on macOS. Avoid features from later releases, and
# reject an older interpreter before performing any source preparation.
if [ "${BASH_VERSINFO[0]}" -lt 3 ] || { [ "${BASH_VERSINFO[0]}" -eq 3 ] && [ "${BASH_VERSINFO[1]}" -lt 2 ]; }; then
  fail 1 'Bash 3.2 or newer is required.'
fi
platform=$(uname -s)
case "$platform" in
  Linux | Darwin | MINGW* | MSYS* | CYGWIN*) ;;
  *) fail 1 "unsupported build host: $platform" ;;
esac

# Check tools before generating headers. No tools are installed automatically.
# GNU objcopy/size are already part of the toolchain needed for linking; using
# them avoids an extra dependency on versioned LLVM utilities on Linux/macOS.
for tool in make clang arm-none-eabi-gcc arm-none-eabi-objcopy arm-none-eabi-size; do
  if ! command -v "$tool" >/dev/null 2>&1; then
    fail 1 "required tool not found on PATH: $tool"
  fi
done
if [ "${TRICE_ID_WORKFLOW_OWNER:-0}" != 1 ] && ! command -v trice >/dev/null 2>&1; then
  fail 1 'required tool not found on PATH: trice'
fi
case "$platform" in
  MINGW* | MSYS* | CYGWIN*)
    command -v cygpath >/dev/null 2>&1 || fail 1 'required Windows path utility not found on PATH: cygpath'
    ;;
esac
if ! make_version=$(make --version 2>&1); then
  fail 1 'could not run GNU make.' "$make_version"
fi
case "$make_version" in
  *'GNU Make '*) ;;
  *) fail 1 'the make command is not GNU make.' "$make_version" ;;
esac
if ! clang_targets=$(clang --print-targets 2>&1); then
  fail 1 'could not query Clang target support.' "$clang_targets"
fi
if ! printf '%s\n' "$clang_targets" | awk '$1 == "thumb" { found=1 } END { exit !found }'; then
  fail 1 'Clang does not provide the Thumb backend required by Cortex-M4.'
fi

# Ask for the same CPU/FPU ABI as mcu.mak, including when selecting multilib
# runtime files. A subshell excludes ambient host/generated header paths from
# discovery without changing the caller's environment. Locale makes GCC's
# documented verbose include-search markers predictable on every host language.
gcc_query() (
  unset CPATH C_INCLUDE_PATH
  export LC_ALL=C LANG=C
  arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard "$@"
)
if ! gcc_target=$(gcc_query -dumpmachine | tr -d '\r'); then
  fail 1 'could not query the Arm GNU compiler target.'
fi
[ "$gcc_target" = arm-none-eabi ] || fail 1 "arm-none-eabi-gcc reports an incompatible target: $gcc_target"
if ! gcc_include=$(gcc_query -print-file-name=include | tr -d '\r'); then
  fail 1 'could not query GCC internal headers.'
fi
[ -d "$gcc_include" ] || fail 1 "GCC internal header directory is unavailable: $gcc_include"

# Validate the runtime files used by this project's nano link, not just libc.a.
# GCC returns the unchanged filename when a file cannot be found; report that
# explicitly instead of assuming an adjacent include directory or library layout.
for runtime_file in libc.a libc_nano.a libm.a libnosys.a libgcc.a nano.specs; do
  if ! runtime_path=$(gcc_query "-print-file-name=$runtime_file" | tr -d '\r'); then
    fail 1 "could not query ARM runtime file: $runtime_file"
  fi
  if [ "$runtime_path" = "$runtime_file" ] || [ ! -s "$runtime_path" ]; then
    fail 1 "ARM runtime file is unavailable for Cortex-M4 hard-float: $runtime_file" "GCC reported: $runtime_path"
  fi
done

# Query GCC's actual search list with an empty input and preprocessing only.
# No object, source or temporary file is written. Library paths are deliberately
# irrelevant here: packaged, relocated and multilib toolchains place them in
# different directories. Preserve each header path as one line, including spaces.
libc_include=${CLANG_LIBC_INCLUDE:-}
if [ -z "$libc_include" ]; then
  if ! header_search=$(printf '\n' | gcc_query -E -v -x c - 2>&1); then
    fail 1 'could not query GCC header search paths.' "$header_search"
  fi
  header_dirs=$(printf '%s\n' "$header_search" | tr -d '\r' | awk '
    /^#include <\.\.\.> search starts here:/ { in_search=1; next }
    in_search && /^End of search list\./ { exit }
    in_search { sub(/^[[:space:]]+/, ""); print }
  ')
  while IFS= read -r candidate; do
    if [ -f "$candidate/stdlib.h" ]; then
      libc_include=$candidate
      break
    fi
  done <<< "$header_dirs"
  [ -n "$libc_include" ] || fail 1 'ARM C library header stdlib.h was not found in the GCC search paths.' "Searched directories:" "$header_dirs"
elif [ ! -f "$libc_include/stdlib.h" ]; then
  fail 1 "CLANG_LIBC_INCLUDE does not contain stdlib.h: $libc_include"
fi

# Clang and GNU make are native Windows programs in Git Bash. Give them forward
# slash Windows paths; Unix tools use ordinary POSIX paths. Quoting in the Make
# rules keeps toolchain and generated-header paths containing spaces intact.
native_path() {
  case "$platform" in
    MINGW* | MSYS* | CYGWIN*) cygpath -m "$1" ;;
    *) printf '%s\n' "$1" ;;
  esac
}
gcc_include=$(native_path "$(cd -- "$gcc_include" && pwd)")
libc_include=$(native_path "$(cd -- "$libc_include" && pwd)")
if [ "$check_only" -eq 1 ]; then
  printf 'Prerequisites OK.\nGCC headers: %s\nARM C library headers: %s\n' "$gcc_include" "$libc_include"
  exit 0
fi

# Reuse the repository's Bind preparation. A managed Insert/Bind test already
# owns its source state, so the helper leaves that state under its control.
# This script never starts a separate Insert/Clean lifecycle.
# shellcheck source=../prepareTriceBind.sh
source ../prepareTriceBind.sh || fail 1 'could not load Trice Bind preparation.'
if prepare_trice_bind_build "$(cd ../.. && pwd)"; then
  :
else
  fail "$?" 'Trice Bind preparation failed; see the diagnostics above.'
fi
bind_include=${TRICE_BIND_INCLUDE_DIR:-}
if [ -n "$bind_include" ]; then
  bind_include=$(native_path "$bind_include")
fi

# Keep parallelism bounded by default, including on Windows. Existing callers
# can supply MAKE_JOBS (for example -j4); splitting this option list is intended.
# -B rebuilds only the selected Clang target and avoids deleting GCC outputs or
# tracking configuration signatures that could survive an interrupted build.
printf 'Building L432_inst with Clang, CONFIGURATION=%s, optimization=-O3\n' "$configuration"
# shellcheck disable=SC2086
if make -B ${MAKE_JOBS:--j2} "TRICE_FLAGS=-DCONFIGURATION=$configuration" \
  "CLANG_GCC_INCLUDE=$gcc_include" "CLANG_LIBC_INCLUDE=$libc_include" \
  "TRICE_BIND_INCLUDE_DIR=$bind_include" clang; then
  :
else
  fail "$?" 'Clang firmware build failed; see the diagnostics above.'
fi

# A successful make must deliver all three usable images. Explain incomplete
# toolchain output here instead of reporting success with an empty/missing file.
for artifact in out.clang/L432KC.elf out.clang/L432KC.hex out.clang/L432KC.bin; do
  if [ ! -s "$artifact" ]; then
    printf 'Available Clang outputs:\n' >&2
    ls -l out.clang >&2 || true
    fail 1 "expected nonempty firmware image: $artifact"
  fi
done
printf 'Build complete: out.clang/L432KC.{elf,hex,bin}\n'
