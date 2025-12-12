#!/bin/bash

# Adapt build_environment.sh file to your system eventually.

if ! command -v arm-none-eabi-gcc; then
  echo arm-none-eabi-gcc not found
fi
if ! command -v clang; then
  echo clang not found
fi

# Put ARM Clang first in path temporary to avoid compiler variants issues.
# llvm-size path under Darwin mabe not set after clang install.
# ARM Clang uses the ARM GNU toolchain libraries and finds them over C_INCLUDE_PATH.

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  loc=$(arm-none-eabi-gcc -print-file-name=include) && export C_INCLUDE_PATH="${loc}:/usr/include/newlib/"
  export MAKE_JOBS="-j"

elif [[ "$OSTYPE" == "darwin"* ]]; then
  echo '$OSTYPE = darwin: MacOS'
  # Check if Homebrew is installed
  if ! command -v brew >/dev/null 2>&1; then
    echo "Homebrew is not installed. Please install Homebrew first."
    exit 1
  fi

  # Check if gcc-arm-embedded is installed via cask
  if brew list --cask | grep -q "^gcc-arm-embedded$"; then
    version=$(brew list --cask --versions gcc-arm-embedded | awk '{print $2}')
    echo "Installed gcc-arm-embedded version: $version"
  else
    echo "gcc-arm-embedded is not installed via Homebrew Cask."
  fi

  export PATH="/Library/Developer/CommandLineTools/usr/bin:$PATH" # needed for llvm-size
  export PATH="/opt/homebrew/opt/binutils/bin:$PATH"
  export C_INCLUDE_PATH="/Applications/ArmGNUToolchain/${version}/arm-none-eabi/arm-none-eabi/include"
  export MAKE_JOBS="-j"

elif [[ "$OSTYPE" == "cygwin" ]]; then
  echo '$OSTYPE = cygwin: POSIX compatibility layer and Linux environment emulation for Windows'
  export MAKE_JOBS="" # Enabling -j here can cause blocking.

elif [[ "$OSTYPE" == "msys" ]]; then
  echo '$OSTYPE msys: Lightweight shell and GNU utilities compiled for Windows (part of MinGW)'
  # which returns s.th. like /...-arm-none-eabi/bin/arm-none-eabi-gcc # 1 slash
  #                                            <---   22 chars   --->
  loc=$(which arm-none-eabi-gcc) && export C_INCLUDE_PATH=${loc:0:${#loc}-22}/arm-none-eabi/include
  export MAKE_JOBS="" # Enabling -j here can cause blocking.

elif [[ "$OSTYPE" == "win32" ]]; then
  echo $OSTYPE # I'm not sure this can happen.
  echo '$OSTYPE is win32'

elif [[ "$OSTYPE" == "freebsd"* ]]; then
  echo $OSTYPE # ...
  echo '$OSTYPE is freebsd'

else
  echo $OSTYPE # Unknown.
  echo '$OSTYPE is unknown'
fi

echo "C_INCLUDE_PATH set to: $C_INCLUDE_PATH"
