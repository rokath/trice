# OS_Darwin.mak

export PATH := /usr/bin:/opt/homebrew/bin:$(PATH)

export C_INCLUDE_PATH := \
/opt/homebrew/Cellar/arm-none-eabi-gcc/14.2.0/lib/gcc/arm-none-eabi/14.2.0/include:\
/opt/homebrew/Cellar/arm-none-eabi-gcc/14.2.0/lib/gcc/arm-none-eabi/14.2.0/include-fixed

include main.mak

