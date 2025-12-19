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
# Helper: derive include path for ARM toolchain from GCC's sysroot
#
# We intentionally avoid fragile string slicing on the compiler path and
# instead ask the compiler where its sysroot is. Typical layouts place the
# C headers under:
#   <sysroot>/include
# or
#   <sysroot>/arm-none-eabi/include
#
# If we can determine a usable include directory, we echo it to stdout so
# that callers can capture it.
###############################################################################

detect_arm_include_dir() {
  # Require arm-none-eabi-gcc to be present
  if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
    return 1
  fi

  # 1) Try sysroot-based detection first (works for some packaged toolchains)
  local sysroot
  sysroot=$(arm-none-eabi-gcc -print-sysroot 2>/dev/null || true)

  if [ -n "$sysroot" ] && [ "$sysroot" != "/" ]; then
    if [ -d "$sysroot/include" ]; then
      echo "$sysroot/include"
      return 0
    fi
    if [ -d "$sysroot/arm-none-eabi/include" ]; then
      echo "$sysroot/arm-none-eabi/include"
      return 0
    fi
  fi

  # 2) Fallback: ask GCC directly for its internal include directory.
  # On many Linux distributions, -print-sysroot is empty for bare-metal toolchains,
  # but -print-file-name=include still returns a valid include path.
  local gcc_include
  gcc_include=$(arm-none-eabi-gcc -print-file-name=include 2>/dev/null || true)

  if [ -n "$gcc_include" ] && [ "$gcc_include" != "include" ] && [ -d "$gcc_include" ]; then
    echo "$gcc_include"
    return 0
  fi

  # Nothing usable found.
  return 1
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

  # Derive an include directory for the ARM toolchain and feed it into C_INCLUDE_PATH.
  arm_inc_dir=$(detect_arm_include_dir || true)
  if [ -n "$arm_inc_dir" ]; then
    # Prepend the ARM include directory. If C_INCLUDE_PATH was already set,
    # we append the old value to keep user customisation.
    if [ -n "${C_INCLUDE_PATH:-}" ]; then
      export C_INCLUDE_PATH="${arm_inc_dir}:${C_INCLUDE_PATH}"
    else
      export C_INCLUDE_PATH="${arm_inc_dir}"
    fi
  else
    # log_warn "Could not auto-detect ARM include directory on Linux (sysroot may be empty for this toolchain). You may need to set C_INCLUDE_PATH manually."
    # now:

    # ---------------------------------------------------------------------------
    # Clang cross builds need a C library include directory (e.g. Newlib) to find
    # headers like stdlib.h. GCC usually finds these internally, but clang does not
    # unless we provide a sysroot/toolchain or additional include paths.
    #
    # On Ubuntu (apt: gcc-arm-none-eabi), Newlib headers are typically here:
    #   /usr/arm-none-eabi/include
    #
    # Additionally, GCC has an internal include directory that is useful for both
    # gcc and clang setups:
    #   arm-none-eabi-gcc -print-file-name=include
    # ---------------------------------------------------------------------------
    newlib_inc="/usr/arm-none-eabi/include"
    gcc_inc="$(arm-none-eabi-gcc -print-file-name=include 2>/dev/null || true)"
  
    if [ -d "$newlib_inc" ]; then
      if [ -n "${C_INCLUDE_PATH:-}" ]; then
        export C_INCLUDE_PATH="${newlib_inc}:${C_INCLUDE_PATH}"
      else
        export C_INCLUDE_PATH="${newlib_inc}"
      fi
    else
      log_warn "Expected Newlib include directory not found: ${newlib_inc} (clang cross may fail to find stdlib.h)."
    fi

    if [ -n "$gcc_inc" ] && [ "$gcc_inc" != "include" ] && [ -d "$gcc_inc" ]; then
      if [ -n "${C_INCLUDE_PATH:-}" ]; then
        export C_INCLUDE_PATH="${gcc_inc}:${C_INCLUDE_PATH}"
      else
        export C_INCLUDE_PATH="${gcc_inc}"
      fi
    else
      log_warn "GCC internal include directory not found via: arm-none-eabi-gcc -print-file-name=include"
    fi
  fi

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

  # Prefer to use the same sysroot-based detection as on Linux.
  arm_inc_dir=$(detect_arm_include_dir || true)
  if [ -n "$arm_inc_dir" ]; then
    if [ -n "${C_INCLUDE_PATH:-}" ]; then
      export C_INCLUDE_PATH="${arm_inc_dir}:${C_INCLUDE_PATH}"
    else
      export C_INCLUDE_PATH="${arm_inc_dir}"
    fi
  else
    log_warn "Could not auto-detect ARM include directory via arm-none-eabi-gcc on macOS."

    # Fallback for the legacy ARM GNU Toolchain installed under /Applications.
    # This is *not* guaranteed to be present and may need manual adjustment.
    if command -v brew >/dev/null 2>&1 && brew list --cask >/dev/null 2>&1; then
      if brew list --cask | grep -q "^gcc-arm-embedded$"; then
        version=$(brew list --cask --versions gcc-arm-embedded | awk '{print $2}' | head -n1)
        log_info "Found Homebrew cask gcc-arm-embedded, version: $version"

        # Typical layout for the ARM GNU Toolchain GUI installer on macOS.
        candidate="/Applications/ArmGNUToolchain/${version}/arm-none-eabi/arm-none-eabi/include"
        if [ -d "$candidate" ]; then
          if [ -n "${C_INCLUDE_PATH:-}" ]; then
            export C_INCLUDE_PATH="${candidate}:${C_INCLUDE_PATH}"
          else
            export C_INCLUDE_PATH="${candidate}"
          fi
        else
          log_warn "ARM include directory not found at: $candidate. You may need to adjust C_INCLUDE_PATH manually."
        fi
      else
        log_info "Homebrew cask gcc-arm-embedded is not installed."
      fi
    fi
  fi

elif [[ "$OSTYPE" == "cygwin" ]]; then
  log_info "Detected platform: Cygwin on Windows (OSTYPE=$OSTYPE)"

  # Under Cygwin, aggressive parallel builds (-j) are known to cause blocking
  # or instability on some setups. We therefore disable it by default.
  export MAKE_JOBS=""

  # Attempt to detect the ARM include directory exactly as on Linux.
  arm_inc_dir=$(detect_arm_include_dir || true)
  if [ -n "$arm_inc_dir" ]; then
    if [ -n "${C_INCLUDE_PATH:-}" ]; then
      export C_INCLUDE_PATH="${arm_inc_dir}:${C_INCLUDE_PATH}"
    else
      export C_INCLUDE_PATH="${arm_inc_dir}"
    fi
  else
    log_warn "Could not auto-detect ARM include directory on Cygwin. You may need to set C_INCLUDE_PATH manually."
  fi

elif [[ "$OSTYPE" == "msys" ]]; then
  log_info "Detected platform: MSYS / MinGW on Windows (OSTYPE=$OSTYPE)"

  # Same reasoning as for Cygwin: parallel make can be problematic; keep it off
  # by default to avoid hard-to-debug hangs.
  export MAKE_JOBS=""

  # Again, try the sysroot-based detection first.
  arm_inc_dir=$(detect_arm_include_dir || true)
  if [ -n "$arm_inc_dir" ]; then
    if [ -n "${C_INCLUDE_PATH:-}" ]; then
      export C_INCLUDE_PATH="${arm_inc_dir}:${C_INCLUDE_PATH}"
    else
      export C_INCLUDE_PATH="${arm_inc_dir}"
    fi
  else
    log_warn "Could not auto-detect ARM include directory on MSYS. You may need to set C_INCLUDE_PATH manually."
  fi

elif [[ "$OSTYPE" == "win32" ]]; then
  # This branch is rarely seen with modern bash installations. It is kept
  # only as a diagnostic in case OSTYPE is literally "win32".
  log_info "Detected platform: Windows (OSTYPE=$OSTYPE)"
  log_info "No default configuration implemented for plain win32."
  export MAKE_JOBS=""
  # We do not attempt to detect C_INCLUDE_PATH here because the installation
  # paths vary widely on native Windows environments.

elif [[ "$OSTYPE" == "freebsd"* ]]; then
  log_info "Detected platform: FreeBSD (OSTYPE=$OSTYPE)"

  # FreeBSD is closer to Linux in behaviour; enable parallel builds by default.
  export MAKE_JOBS="-j"

  arm_inc_dir=$(detect_arm_include_dir || true)
  if [ -n "$arm_inc_dir" ]; then
    if [ -n "${C_INCLUDE_PATH:-}" ]; then
      export C_INCLUDE_PATH="${arm_inc_dir}:${C_INCLUDE_PATH}"
    else
      export C_INCLUDE_PATH="${arm_inc_dir}"
    fi
  else
    log_warn "Could not auto-detect ARM include directory on FreeBSD. You may need to set C_INCLUDE_PATH manually."
  fi

else
  log_info "Detected platform: Unknown (OSTYPE=$OSTYPE)"
  log_info "No platform-specific configuration available."
  export MAKE_JOBS=""
  # We do not attempt C_INCLUDE_PATH auto-detection here.
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
  echo "C_INCLUDE_PATH: ${C_INCLUDE_PATH:-<unset>}"
  echo "MAKE_JOBS     : ${MAKE_JOBS:-<unset>}"
  echo "====================================="
  echo
fi
