
# Build path
CLANG_BUILD = out.clang

VERSION = # -17 # When a default version is used, this should be empty.

CLANG_CC = clang$(VERSION)
CLANG_AS = clang$(VERSION) -x assembler-with-cpp
CLANG_CP = arm-none-eabi-objcopy #llvm-objcopy$(VERSION)
#CLANG_CP = objcopy
CLANG_SZ = llvm-size$(VERSION)
CLANG_HEX = $(CLANG_CP) -O ihex
CLANG_BIN = $(CLANG_CP) -O binary -S

CLANG_ONLY_FLAGS = #
#CLANG_ONLY_FLAGS += -Weverything #           # clang only: enable the warning but liberally disable warnings from it that are not useful.
 CLANG_ONLY_FLAGS += --target=arm-none-eabi #
 
# CLANG_ONLY_FLAGS += --gcc-toolchain=/usr
# Optional: allow the environment/CI to provide the GCC toolchain root for clang.
# Example for GitHub Actions Ubuntu: export CLANG_GCC_TOOLCHAIN=/usr
ifneq ($(strip $(CLANG_GCC_TOOLCHAIN)),)
  CLANG_ONLY_FLAGS += --gcc-toolchain=$(CLANG_GCC_TOOLCHAIN)
endif

# Optional extra include dirs for clang cross builds (provided by CI/environment).
# Example:
#   CLANG_SYS_INCLUDES="-isystem /usr/arm-none-eabi/include -isystem /usr/lib/gcc/arm-none-eabi/..."
CLANG_ONLY_FLAGS += $(CLANG_SYS_INCLUDES)

#CLANG_CC_FLAGS += -Wa,-a,-ad,-alms=$(CLANG_BUILD)/$(notdir $(<:.c=.lst))
#CLANG_ONLY_FLAGS += -Wa,--noexecstack
