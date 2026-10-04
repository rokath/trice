
# Build path
GCC_BUILD = out.gcc

GCC_CC = arm-none-eabi-gcc
GCC_AS = arm-none-eabi-gcc -x assembler-with-cpp
GCC_CP = arm-none-eabi-objcopy
GCC_SZ = arm-none-eabi-size
GCC_HEX = $(GCC_CP) -O ihex
GCC_BIN = $(GCC_CP) -O binary -S

GCC_ONLY_FLAGS = #
# Listings are useful for manual inspection, but do not change target code.
# The full configuration test matrix disables this expensive text output only.
GCC_LISTINGS ?= 1
ifeq ($(GCC_LISTINGS),1)
GCC_ONLY_FLAGS += -Wa,-a,-ad,-alms=$(GCC_BUILD)/$(notdir $(<:.c=.lst)) #
endif
