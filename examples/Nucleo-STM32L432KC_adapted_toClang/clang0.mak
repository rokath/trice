
# Put ARM Clang first in path temporary to avoid compiler variants issues.
export PATH := C:\bin\ArmClang\bin:$(PATH)

# ARM Clang uses the ARM GNU toolchain libraries and finds them over C_INCLUDE_PATH.
export C_INCLUDE_PATH := C:\bin\ArmGNUToolchain\arm-none-eabi\include

# Build path
CLANG_BUILD = build.clang

CLANG_CC = clang
CLANG_AS = clang -x assembler-with-cpp
CLANG_CP = llvm-objcopy
CLANG_SZ = llvm-size
CLANG_HEX = $(CLANG_CP) -O ihex
CLANG_BIN = $(CLANG_CP) -O binary -S

CLANG_ONLY_FLAGS = #
#CLANG_ONLY_FLAGS += -Weverything #           # clang only: enable the warning but liberally disable warnings from it that are not useful.
 CLANG_ONLY_FLAGS += --target=arm-none-eabi #
#CLANG_CC_FLAGS += -Wa,-a,-ad,-alms=$(CLANG_BUILD)/$(notdir $(<:.c=.lst))
#CLANG_ONLY_FLAGS += -Wa,--noexecstack
