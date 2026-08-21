#!/bin/bash
#
# _150_setup_build_environment.sh
#
# This script prepares a build environment for an ARM bare-metal toolchain.
# It is intended to be sourced so that the
# environment variables (PATH, CLANG_SYS_INCLUDES, MAKE_JOBS, …) persist in
# the current shell.
#
# Behavior:
#   - By default, it only prints errors and warnings.
#   - If called with -v or --verbose, it prints additional informational output.
#
# Example:
#   . ./scripts/_150_setup_build_environment.sh          # quiet, only errors/warnings
#   . ./scripts/_150_setup_build_environment.sh -v       # verbose

###############################################################################
# Verbosity handling
###############################################################################

VERBOSE=0

# Look for -v / --verbose in the current shell's positional parameters.
# Note: when sourced, this inspects the caller's "$@", which usually is what
# you passed when sourcing this script.
for arg in "$@"; do
  case "$arg" in
    -v | --verbose)
      VERBOSE=1
      ;;
  esac
done

###############################################################################
# Logging helpers
###############################################################################

log_info() {
  # Informational messages only printed in verbose mode
  if [ "$VERBOSE" -eq 1 ]; then
    echo "INFO:" "$@"
  fi
}

log_warn() {
  # Warnings are always printed to stderr
  echo "WARNING:" "$@" >&2
}

log_error() {
  # Errors are always printed to stderr
  echo "ERROR:" "$@" >&2
}

###############################################################################
# Basic tool presence checks
###############################################################################

if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  log_warn "arm-none-eabi-gcc not found in PATH. Cross-compilation will not work until you install it or adjust PATH."
fi

if ! command -v clang >/dev/null 2>&1; then
  log_warn "clang not found in PATH. Some projects or checks may require Clang/LLVM."
fi

###############################################################################
# ARM bare-metal toolchain helpers
###############################################################################

append_unique_dir() {
  local var_name="$1"
  local candidate="$2"
  local current
  local entry

  [ -n "$candidate" ] || return 0
  [ -d "$candidate" ] || return 0

  eval "current=\${$var_name:-}"
  for entry in $current; do
    [ "$entry" = "$candidate" ] && return 0
  done

  if [ -n "$current" ]; then
    eval "$var_name=\"\$current \$candidate\""
  else
    eval "$var_name=\"\$candidate\""
  fi
}

join_as_isystem_flags() {
  local result=""
  local dir

  for dir in "$@"; do
    [ -n "$dir" ] || continue
    if [ -n "$result" ]; then
      result="$result -isystem $dir"
    else
      result="-isystem $dir"
    fi
  done

  printf '%s\n' "$result"
}

detect_arm_gcc_path() {
  command -v arm-none-eabi-gcc 2>/dev/null || true
}

detect_arm_sysroot() {
  arm-none-eabi-gcc -print-sysroot 2>/dev/null || true
}

detect_arm_gcc_include() {
  arm-none-eabi-gcc -print-file-name=include 2>/dev/null || true
}

detect_arm_libc_include() {
  local libc_path
  local libc_root

  # Ask GCC for its default C library so the matching standard headers can be
  # derived without assuming a distribution- or installer-specific layout.
  libc_path=$(arm-none-eabi-gcc -print-file-name=libc.a 2>/dev/null || true)
  if [ -z "$libc_path" ] || [ "$libc_path" = "libc.a" ] || [ ! -f "$libc_path" ]; then
    return 1
  fi

  libc_root=$(cd "$(dirname "$libc_path")/.." && pwd)
  [ -d "$libc_root/include" ] || return 1
  printf '%s\n' "$libc_root/include"
}

detect_arm_toolchain_root() {
  local gcc_path
  local sysroot
  local parent

  gcc_path=$(detect_arm_gcc_path)
  sysroot=$(detect_arm_sysroot)

  if [ -n "$sysroot" ] && [ "$sysroot" != "/" ] && [ -d "$sysroot" ]; then
    case "$sysroot" in
      */arm-none-eabi)
        parent=$(cd "$sysroot/.." && pwd)
        [ -d "$parent" ] && {
          printf '%s\n' "$parent"
          return 0
        }
        ;;
    esac
  fi

  if [ -n "$gcc_path" ]; then
    parent=$(cd "$(dirname "$gcc_path")/.." && pwd)
    [ -d "$parent" ] && {
      printf '%s\n' "$parent"
      return 0
    }
  fi

  return 1
}

collect_arm_clang_include_dirs() {
  local sysroot
  local gcc_include
  local libc_include
  local toolchain_root
  local dirs=""

  sysroot=$(detect_arm_sysroot)
  gcc_include=$(detect_arm_gcc_include)
  libc_include=$(detect_arm_libc_include || true)
  toolchain_root=$(detect_arm_toolchain_root || true)

  append_unique_dir dirs "$gcc_include"
  append_unique_dir dirs "$libc_include"

  if [ -n "$sysroot" ] && [ "$sysroot" != "/" ]; then
    append_unique_dir dirs "$sysroot/include"
    append_unique_dir dirs "$sysroot/arm-none-eabi/include"
  fi

  if [ -n "$toolchain_root" ]; then
    append_unique_dir dirs "$toolchain_root/arm-none-eabi/include"
  fi

  printf '%s\n' "$dirs"
}

export_clang_cross_env() {
  local allow_auto_gcc_toolchain="${1:-1}"
  local toolchain_root
  local include_dirs
  local clang_sys_includes

  if ! command -v clang >/dev/null 2>&1; then
    return 0
  fi
  if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
    return 0
  fi

  toolchain_root=$(detect_arm_toolchain_root || true)
  include_dirs=$(collect_arm_clang_include_dirs)

  # Only auto-export CLANG_GCC_TOOLCHAIN on platforms where that setup is
  # known to be helpful. Keep it unset by default because auto-injecting
  # --gcc-toolchain can cause noisy host-specific Clang warnings although the
  # build itself is otherwise valid.
  #
  # Rationale:
  # - The Clang build already gets the required ARM headers via CLANG_SYS_INCLUDES.
  # - Auto-injecting --gcc-toolchain caused host-specific warnings such as
  #   "-Wmultilib-not-found" or "-Wunused-command-line-argument" although the
  #   build itself was otherwise valid.
  # - The Makefiles still support an explicit CLANG_GCC_TOOLCHAIN override from the
  #   outer environment or CI when a setup really needs it.
  #
  # So the behaviour is now:
  # - respect an already provided CLANG_GCC_TOOLCHAIN value
  # - otherwise auto-detect it only when the caller explicitly allows that
  # - in all cases still compute CLANG_SYS_INCLUDES below
  if [ -n "${CLANG_GCC_TOOLCHAIN:-}" ]; then
    log_info "Using pre-set CLANG_GCC_TOOLCHAIN=$CLANG_GCC_TOOLCHAIN"
  elif [ "$allow_auto_gcc_toolchain" = "1" ] && [ -n "$toolchain_root" ]; then
    export CLANG_GCC_TOOLCHAIN="$toolchain_root"
    log_info "Auto-set CLANG_GCC_TOOLCHAIN=$CLANG_GCC_TOOLCHAIN"
  else
    unset CLANG_GCC_TOOLCHAIN
    if [ -n "$toolchain_root" ]; then
      log_info "Leaving CLANG_GCC_TOOLCHAIN unset; detected toolchain root is $toolchain_root"
    else
      log_info "Leaving CLANG_GCC_TOOLCHAIN unset; no explicit override provided."
    fi
  fi

  if [ -n "$include_dirs" ]; then
    # shellcheck disable=SC2086
    clang_sys_includes=$(join_as_isystem_flags $include_dirs)
    export CLANG_SYS_INCLUDES="$clang_sys_includes"
    log_info "Set CLANG_SYS_INCLUDES=$CLANG_SYS_INCLUDES"
  else
    log_warn "Could not auto-detect ARM include directories for clang."
  fi
}

# is_positive_job_count accepts only the bounded decimal values GNU Make can
# safely consume as a Windows parallelism setting.
is_positive_job_count() {
  case "$1" in
    "" | *[!0-9]* | 0) return 1 ;;
    *) return 0 ;;
  esac
}

set_windows_make_jobs() {
  local detected_jobs=""
  local detection_source=""

  # Respect an explicit caller choice. This lets developers and CI lower the
  # job count for constrained systems or raise it after validating a specific
  # Windows toolchain. A bare "-j" remains the caller's responsibility because
  # GNU Make interprets it as an unlimited job count, not as "one job per CPU".
  if [ -n "${MAKE_JOBS:-}" ]; then
    log_info "Respecting pre-set Windows build parallelism: MAKE_JOBS=$MAKE_JOBS"
    export MAKE_JOBS
    return 0
  fi

  # Prefer the number of physical CPU cores on Windows. NUMBER_OF_PROCESSORS
  # and nproc report logical processors, including Hyper-Threading siblings.
  # Compiling one C source file per logical processor caused the native ARM GCC
  # 13.2 toolchain to terminate intermittently with STATUS_ACCESS_VIOLATION on
  # an otherwise healthy 4-core/8-thread Windows machine.
  #
  # Windows PowerShell is available on supported Windows versions and can query
  # the physical-core count directly. Remove carriage returns from its output
  # because this function is running inside a Unix-like Windows shell.
  if command -v powershell.exe >/dev/null 2>&1; then
    detected_jobs=$(powershell.exe -NoProfile -NonInteractive -Command \
      '(Get-CimInstance Win32_Processor | Measure-Object -Property NumberOfCores -Sum).Sum' \
      2>/dev/null | tr -d '\r' | tail -n 1)
    detection_source="Windows physical cores via PowerShell"
  fi
  if ! is_positive_job_count "$detected_jobs"; then
    detected_jobs=""
    detection_source=""
  fi
  if [ -z "$detected_jobs" ] && command -v pwsh.exe >/dev/null 2>&1; then
    detected_jobs=$(pwsh.exe -NoProfile -NonInteractive -Command \
      '(Get-CimInstance Win32_Processor | Measure-Object -Property NumberOfCores -Sum).Sum' \
      2>/dev/null | tr -d '\r' | tail -n 1)
    detection_source="Windows physical cores via PowerShell 7"
  fi
  if ! is_positive_job_count "$detected_jobs"; then
    detected_jobs=""
    detection_source=""
  fi
  if [ -z "$detected_jobs" ] && [ -n "${NUMBER_OF_PROCESSORS:-}" ]; then
    # This fallback counts logical processors when PowerShell is unavailable or
    # its CIM query is denied by local policy or sandboxing.
    detected_jobs="$NUMBER_OF_PROCESSORS"
    detection_source="logical processors via NUMBER_OF_PROCESSORS"
  fi
  if ! is_positive_job_count "$detected_jobs"; then
    detected_jobs=""
    detection_source=""
  fi
  if [ -z "$detected_jobs" ] && command -v nproc >/dev/null 2>&1; then
    # nproc is the usual logical-processor fallback in Unix-like environments.
    detected_jobs=$(nproc 2>/dev/null || true)
    detection_source="logical processors via nproc"
  fi
  if ! is_positive_job_count "$detected_jobs"; then
    detected_jobs=""
    detection_source=""
  fi
  if [ -z "$detected_jobs" ] && command -v getconf >/dev/null 2>&1; then
    # getconf is more widely standardized, but it is not present in every
    # minimal Git Bash installation and also reports online logical processors.
    detected_jobs=$(getconf _NPROCESSORS_ONLN 2>/dev/null || true)
    detection_source="logical processors via getconf"
  fi

  # Accept only a positive decimal integer. If detection is unavailable or
  # malformed, use two jobs: this preserves some parallelism without restoring
  # the unbounded process burst that caused ARM GCC to crash on Windows.
  if ! is_positive_job_count "$detected_jobs"; then
    detected_jobs=2
    log_warn "Could not determine a valid Windows processor count; defaulting to $detected_jobs make jobs."
  fi

  MAKE_JOBS="-j$detected_jobs"
  export MAKE_JOBS
  log_info "Using bounded Windows build parallelism from ${detection_source:-fallback}: MAKE_JOBS=$MAKE_JOBS"
}

###############################################################################
# OS-specific configuration
#
# We branch on OSTYPE, which is set by bash. Some typical values:
#   - linux-gnu* : GNU/Linux
#   - darwin*    : macOS
#   - cygwin     : Cygwin on Windows
#   - msys       : MSYS2 / MinGW on Windows
###############################################################################

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  log_info "Detected platform: Linux (OSTYPE=$OSTYPE)"

  # On Linux we usually want to enable parallel builds.
  export MAKE_JOBS="-j"

  # Clang receives the required ARM include paths separately. Let users or CI
  # opt into CLANG_GCC_TOOLCHAIN explicitly if their Linux setup needs it.
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "darwin"* ]]; then
  log_info "Detected platform: macOS (OSTYPE=$OSTYPE)"

  ###########################################################################
  # macOS specifics:
  #   - llvm-size is provided by Xcode Command Line Tools; ensure their bin is in PATH.
  #   - Homebrew binutils may be used for additional tools (e.g. objdump).
  #   - The ARM GNU toolchain may come from:
  #       * Homebrew formula (arm-none-eabi-gcc) or
  #       * ARM installer under /Applications/ArmGNUToolchain/<version>/...
  ###########################################################################

  # Ensure Xcode Command Line Tools are ahead in PATH so llvm-size is found
  if [ -d "/Library/Developer/CommandLineTools/usr/bin" ]; then
    export PATH="/Library/Developer/CommandLineTools/usr/bin:$PATH"
    log_info "Added /Library/Developer/CommandLineTools/usr/bin to PATH."
  fi

  # If Homebrew binutils exist, put them early in PATH as well
  if command -v brew >/dev/null 2>&1; then
    if [ -d "/opt/homebrew/opt/binutils/bin" ]; then
      export PATH="/opt/homebrew/opt/binutils/bin:$PATH"
      log_info "Added /opt/homebrew/opt/binutils/bin to PATH."
    fi
  fi

  # macOS builds are usually fine with parallel jobs
  export MAKE_JOBS="-j"

  export_clang_cross_env 0

elif [[ "$OSTYPE" == "cygwin" ]]; then
  log_info "Detected platform: Cygwin on Windows (OSTYPE=$OSTYPE)"

  # Limit make to the detected logical processor count. GNU Make's bare "-j"
  # would otherwise permit an unlimited number of concurrent compiler jobs.
  set_windows_make_jobs
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "msys"* ]]; then
  log_info "Detected platform: MSYS / MinGW on Windows (OSTYPE=$OSTYPE)"

  # Git Bash normally reaches this branch. Use all detected logical processors
  # while keeping the number of concurrent compiler processes bounded.
  set_windows_make_jobs
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "win32" ]]; then
  # This branch is rarely seen with modern bash installations. It is kept
  # only as a diagnostic in case OSTYPE is literally "win32".
  log_info "Detected platform: Windows (OSTYPE=$OSTYPE)"
  set_windows_make_jobs
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "freebsd"* ]]; then
  log_info "Detected platform: FreeBSD (OSTYPE=$OSTYPE)"

  # FreeBSD is closer to Linux in behaviour; enable parallel builds by default.
  export MAKE_JOBS="-j"
  export_clang_cross_env 1

else
  log_info "Detected platform: Unknown (OSTYPE=$OSTYPE)"
  log_info "No platform-specific configuration available."
  export MAKE_JOBS="-j"
  export_clang_cross_env 1
fi

###############################################################################
# Final summary (verbose only)
###############################################################################

if [ "$VERBOSE" -eq 1 ]; then
  echo
  echo "===== Build environment summary ====="
  echo "OSTYPE        : $OSTYPE"
  if command -v arm-none-eabi-gcc >/dev/null 2>&1; then
    echo "arm-none-eabi-gcc: $(command -v arm-none-eabi-gcc)"
  else
    echo "arm-none-eabi-gcc: NOT FOUND"
  fi
  if command -v clang >/dev/null 2>&1; then
    echo "clang         : $(command -v clang)"
  else
    echo "clang         : NOT FOUND"
  fi
  echo "CLANG_GCC_TOOLCHAIN: ${CLANG_GCC_TOOLCHAIN:-<unset>}"
  echo "CLANG_SYS_INCLUDES : ${CLANG_SYS_INCLUDES:-<unset>}"
  echo "C_INCLUDE_PATH: ${C_INCLUDE_PATH:-<unset>}"
  echo "MAKE_JOBS     : ${MAKE_JOBS:-<unset>}"
  echo "====================================="
  echo
fi
