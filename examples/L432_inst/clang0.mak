
# Build path
CLANG_BUILD = out.clang

VERSION = # -17 # When a default version is used, this should be empty.

CLANG_CC = clang$(VERSION)
CLANG_AS = clang$(VERSION) -x assembler-with-cpp
# Linking already requires Arm GNU tools. Reuse their image utilities so a
# separate (possibly version-suffixed) LLVM objcopy/size installation is optional.
CLANG_CP = $(GCC_CP)
CLANG_SZ = $(GCC_SZ)
CLANG_HEX = $(CLANG_CP) -O ihex
CLANG_BIN = $(CLANG_CP) -O binary -S

# The shared flags preserve the GCC build's existing diagnostics and options.
# Remove switches that Clang does not implement from both its C and assembly
# commands, rather than disabling unknown-option warnings or source diagnostics.
# Patterns also cover different stack limits and identifier-normalization modes.
CLANG_EXCLUDED_FLAGS = \
-funsafe-loop-optimizations \
-Wno-builtin-declaration-mismatch \
-Wstack-usage=% \
-Wunsafe-loop-optimizations \
-Wlogical-op \
-Wnormalized=% \
-Wno-discarded-qualifiers \
-Wno-suggest-attribute=noreturn
CLANG_C_FLAGS = $(filter-out $(CLANG_EXCLUDED_FLAGS),$(C_FLAGS))

CLANG_ONLY_FLAGS = #
#CLANG_ONLY_FLAGS += -Weverything #           # clang only: enable the warning but liberally disable warnings from it that are not useful.
 CLANG_ONLY_FLAGS += --target=arm-none-eabi #
# Keep the script's detected paths quoted, including paths with spaces. Other
# callers can still provide additional cross-compiler options through the shared
# environment variables used by the repository's other Clang examples.
CLANG_ONLY_FLAGS += $(if $(CLANG_GCC_INCLUDE),-isystem "$(CLANG_GCC_INCLUDE)")
CLANG_ONLY_FLAGS += $(if $(CLANG_LIBC_INCLUDE),-isystem "$(CLANG_LIBC_INCLUDE)")
CLANG_ONLY_FLAGS += $(CLANG_SYS_INCLUDES)
ifneq ($(strip $(CLANG_GCC_TOOLCHAIN)),)
CLANG_ONLY_FLAGS += --gcc-toolchain="$(CLANG_GCC_TOOLCHAIN)"
endif
#CLANG_CC_FLAGS += -Wa,-a,-ad,-alms=$(CLANG_BUILD)/$(notdir $(<:.c=.lst))
#CLANG_ONLY_FLAGS += -Wa,--noexecstack
