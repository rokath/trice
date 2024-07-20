
# Build path
CLANG_BUILD = out.clang

VERSION = # -17 # When a default version is used, this should be empty.

CLANG_CC = clang$(VERSION)
CLANG_AS = clang$(VERSION) -x assembler-with-cpp
CLANG_CP = llvm-objcopy$(VERSION)
CLANG_SZ = llvm-size$(VERSION)
CLANG_HEX = $(CLANG_CP) -O ihex
CLANG_BIN = $(CLANG_CP) -O binary -S

CLANG_ONLY_FLAGS = #
#CLANG_ONLY_FLAGS += -Weverything #           # clang only: enable the warning but liberally disable warnings from it that are not useful.
 CLANG_ONLY_FLAGS += --target=arm-none-eabi #
#CLANG_CC_FLAGS += -Wa,-a,-ad,-alms=$(CLANG_BUILD)/$(notdir $(<:.c=.lst))
#CLANG_ONLY_FLAGS += -Wa,--noexecstack
