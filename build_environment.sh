#!/bin/bash

echo "# Adapt build_environment.sh file to your system eventually."

# Put ARM Clang first in path temporary to avoid compiler variants issues.
# llvm-size path under Darwin mabe not set after cklang install.
# ARM Clang uses the ARM GNU toolchain libraries and finds them over C_INCLUDE_PATH.

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo $OSTYPE
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo $OSTYPE # Mac OSX
    export PATH="/Library/Developer/CommandLineTools/usr/bin:$PATH"
    export C_INCLUDE_PATH="/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi/arm-none-eabi/include"
    export PATH="/opt/homebrew/opt/binutils/bin:$PATH"
elif [[ "$OSTYPE" == "cygwin" ]]; then
    echo $OSTYPE # POSIX compatibility layer and Linux environment emulation for Windows
elif [[ "$OSTYPE" == "msys" ]]; then
    echo $OSTYPE # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    export PATH="C:\\bin\\ArmClang\\bin:$PATH"
    export C_INCLUDE_PATH="C:\\bin\\ArmGNUToolchain\\arm-none-eabi\\include"
elif [[ "$OSTYPE" == "win32" ]]; then
    echo $OSTYPE # I'm not sure this can happen.
elif [[ "$OSTYPE" == "freebsd"* ]]; then
     echo $OSTYPE # ...
else
     echo $OSTYPE # Unknown.
fi
