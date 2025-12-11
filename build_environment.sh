#!/bin/bash
#
# build_environment.sh
#
# This script prepares a build environment for an ARM bare-metal toolchain.
# It is intended to be *sourced* (". ./build_environment.sh") so that the
# environment variables (PATH, C_INCLUDE_PATH, MAKE_JOBS, …) persist in
# the current shell.
#
# The script:
#   - checks for the presence of arm-none-eabi-gcc and clang
#   - sets C_INCLUDE_PATH so that ARM Clang can find the ARM GNU toolchain headers
#   - sets PATH for some platforms (e.g. macOS for llvm-size and binutils)
#   - sets MAKE_JOBS to control "make -j" usage per platform

###############################################################################
# Basic tool presence checks
###############################################################################

if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  echo "WARNING: arm-none-eabi-gcc not found in PATH."
  echo "         Cross-compilation will not work until you install it or adjust PATH."
fi

if ! command -v clang >/dev/null 2>&1; then
  echo "WARNING: clang not found in PATH."
  echo "         Some projects or checks may require Clang/LLVM."
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

  # Ask the compiler for its sysroot (where its libraries and headers live)
  local sysroot
  sysroot=$(arm-none-eabi-gcc -print-sysroot 2>/dev/null || true)

  # Some packaged toolchains may return empty or "/" if they do not use a sysroot.
  if [ -z "$sysroot" ] || [ "$sysroot" = "/" ]; then
    # We do not guess further here to avoid wrong paths.
    # The user can always override C_INCLUDE_PATH manually if needed.
    return 1
  fi

  # Common header locations, tested in order.
  if [ -d "$sysroot/include" ]; then
    echo "$sysroot/include"
    return 0
  fi

  if [ -d "$sysroot/arm-none-eabi/include" ]; then
    echo "$sysroot/arm-none-eabi/include"
    return 0
  fi

  # No known include directory found under sysroot.
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
  echo "Detected platform: Linux (OSTYPE=$OSTYPE)"

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
    echo "NOTE: Could not auto-detect ARM include directory on Linux."
    echo "      You may need to set C_INCLUDE_PATH manually for your toolchain."
  fi

elif [[ "$OSTYPE" == "darwin"* ]]; then
  echo "Detected platform: macOS (OSTYPE=$OSTYPE)"

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
  fi

  # If Homebrew binutils exist, put them early in PATH as well
  if command -v brew >/dev/null 2>&1; then
    if [ -d "/opt/homebrew/opt/binutils/bin" ]; then
      export PATH="/opt/homebrew/opt/binutils/bin:$PATH"
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
    echo "NOTE: Could not auto-detect ARM include directory via arm-none-eabi-gcc on macOS."

    # Fallback for the legacy ARM GNU Toolchain installed under /Applications.
    # This is *not* guaranteed to be present and may need manual adjustment.
    if command -v brew >/dev/null 2>&1 && brew list --cask >/dev/null 2>&1; then
      if brew list --cask | grep -q "^gcc-arm-embedded$"; then
        version=$(brew list --cask --versions gcc-arm-embedded | awk '{print $2}' | head -n1)
        echo "Found Homebrew cask gcc-arm-embedded, version: $version"

        # Typical layout for the ARM GNU Toolchain GUI installer on macOS.
        candidate="/Applications/ArmGNUToolchain/${version}/arm-none-eabi/arm-none-eabi/include"
        if [ -d "$candidate" ]; then
          if [ -n "${C_INCLUDE_PATH:-}" ]; then
            export C_INCLUDE_PATH="${candidate}:${C_INCLUDE_PATH}"
          else
            export C_INCLUDE_PATH="${candidate}"
          fi
        else
          echo "WARNING: ARM include directory not found at:"
          echo "         $candidate"
          echo "         You may need to adjust C_INCLUDE_PATH manually."
        fi
      else
        echo "NOTE: Homebrew cask gcc-arm-embedded is not installed."
      fi
    fi
  fi

elif [[ "$OSTYPE" == "cygwin" ]]; then
  echo "Detected platform: Cygwin on Windows (OSTYPE=$OSTYPE)"

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
    echo "NOTE: Could not auto-detect ARM include directory on Cygwin."
    echo "      You may need to set C_INCLUDE_PATH manually."
  fi

elif [[ "$OSTYPE" == "msys" ]]; then
  echo "Detected platform: MSYS / MinGW on Windows (OSTYPE=$OSTYPE)"

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
    echo "NOTE: Could not auto-detect ARM include directory on MSYS."
    echo "      You may need to set C_INCLUDE_PATH manually."
  fi

elif [[ "$OSTYPE" == "win32" ]]; then
  # This branch is rarely seen with modern bash installations. It is kept
  # only as a diagnostic in case OSTYPE is literally "win32".
  echo "Detected platform: Windows (OSTYPE=$OSTYPE)"
  echo "No default configuration implemented for plain win32."
  export MAKE_JOBS=""
  # We do not attempt to detect C_INCLUDE_PATH here because the installation
  # paths vary widely on native Windows environments.

elif [[ "$OSTYPE" == "freebsd"* ]]; then
  echo "Detected platform: FreeBSD (OSTYPE=$OSTYPE)"

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
    echo "NOTE: Could not auto-detect ARM include directory on FreeBSD."
    echo "      You may need to set C_INCLUDE_PATH manually."
  fi

else
  echo "Detected platform: Unknown (OSTYPE=$OSTYPE)"
  echo "No platform-specific configuration available."
  export MAKE_JOBS=""
  # We do not attempt C_INCLUDE_PATH auto-detection here.
fi

###############################################################################
# Final summary for the user
###############################################################################

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
