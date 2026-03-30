#!/bin/bash
#
# build_environment.sh
#
# This script prepares a build environment for an ARM bare-metal toolchain.
# It is intended to be *sourced* (". ./build_environment.sh") so that the
# environment variables (PATH, C_INCLUDE_PATH, MAKE_JOBS, …) persist in
# the current shell.
#
# Behavior:
#   - By default, it only prints errors and warnings.
#   - If called with -v or --verbose, it prints additional informational output.
#
# Example:
#   . ./build_environment.sh          # quiet, only errors/warnings
#   . ./build_environment.sh -v       # verbose

###############################################################################
# Verbosity handling
###############################################################################

VERBOSE=0

# Look for -v / --verbose in the current shell's positional parameters.
# Note: when sourced, this inspects the caller's "$@", which usually is what
# you passed to ". ./build_environment.sh ...".
for arg in "$@"; do
  case "$arg" in
    -v|--verbose)
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
  local toolchain_root
  local dirs=""

  sysroot=$(detect_arm_sysroot)
  gcc_include=$(detect_arm_gcc_include)
  toolchain_root=$(detect_arm_toolchain_root || true)

  append_unique_dir dirs "$gcc_include"

  if [ -n "$sysroot" ] && [ "$sysroot" != "/" ]; then
    append_unique_dir dirs "$sysroot/include"
    append_unique_dir dirs "$sysroot/arm-none-eabi/include"
  fi

  if [ -n "$toolchain_root" ]; then
    append_unique_dir dirs "$toolchain_root/arm-none-eabi/include"
  fi

  printf '%s\n' "$dirs"
}

prepend_colon_path_list() {
  local existing="$1"
  shift
  local result=""
  local dir

  for dir in "$@"; do
    [ -n "$dir" ] || continue
    [ -d "$dir" ] || continue
    if [ -n "$result" ]; then
      result="$result:$dir"
    else
      result="$dir"
    fi
  done

  if [ -n "$existing" ]; then
    if [ -n "$result" ]; then
      printf '%s:%s\n' "$result" "$existing"
    else
      printf '%s\n' "$existing"
    fi
  else
    printf '%s\n' "$result"
  fi
}

export_clang_cross_env() {
  local allow_gcc_toolchain="${1:-1}"
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

  if [ "$allow_gcc_toolchain" = "1" ] && [ -n "$toolchain_root" ]; then
    export CLANG_GCC_TOOLCHAIN="$toolchain_root"
    log_info "Set CLANG_GCC_TOOLCHAIN=$CLANG_GCC_TOOLCHAIN"
  elif [ "$allow_gcc_toolchain" = "1" ]; then
    log_warn "Could not auto-detect the ARM GCC toolchain root for clang."
  else
    unset CLANG_GCC_TOOLCHAIN
    log_info "Leaving CLANG_GCC_TOOLCHAIN unset on this platform."
  fi

  if [ -n "$include_dirs" ]; then
    # shellcheck disable=SC2086
    clang_sys_includes=$(join_as_isystem_flags $include_dirs)
    export CLANG_SYS_INCLUDES="$clang_sys_includes"
    # shellcheck disable=SC2086
    export C_INCLUDE_PATH="$(prepend_colon_path_list "${C_INCLUDE_PATH:-}" $include_dirs)"
    log_info "Set CLANG_SYS_INCLUDES=$CLANG_SYS_INCLUDES"
  else
    log_warn "Could not auto-detect ARM include directories for clang."
  fi
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
  export_clang_cross_env 1

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

  export_clang_cross_env 1

elif [[ "$OSTYPE" == "cygwin" ]]; then
  log_info "Detected platform: Cygwin on Windows (OSTYPE=$OSTYPE)"

  # Under Cygwin, aggressive parallel builds (-j) are known to cause blocking
  # or instability on some setups. We therefore disable it by default.
  export MAKE_JOBS=""
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "msys"* ]]; then
  log_info "Detected platform: MSYS / MinGW on Windows (OSTYPE=$OSTYPE)"

  # Same reasoning as for Cygwin: parallel make can be problematic; keep it off
  # by default to avoid hard-to-debug hangs.
  export MAKE_JOBS=""
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "win32" ]]; then
  # This branch is rarely seen with modern bash installations. It is kept
  # only as a diagnostic in case OSTYPE is literally "win32".
  log_info "Detected platform: Windows (OSTYPE=$OSTYPE)"
  log_info "No default configuration implemented for plain win32."
  export MAKE_JOBS=""
  export_clang_cross_env 0

elif [[ "$OSTYPE" == "freebsd"* ]]; then
  log_info "Detected platform: FreeBSD (OSTYPE=$OSTYPE)"

  # FreeBSD is closer to Linux in behaviour; enable parallel builds by default.
  export MAKE_JOBS="-j"
  export_clang_cross_env 1

else
  log_info "Detected platform: Unknown (OSTYPE=$OSTYPE)"
  log_info "No platform-specific configuration available."
  export MAKE_JOBS=""
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
