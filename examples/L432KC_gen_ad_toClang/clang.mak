
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

clang: $(CLANG_BUILD)/$(TARGET).elf $(CLANG_BUILD)/$(TARGET).hex $(CLANG_BUILD)/$(TARGET).bin
	@$(MAKE) --no-print-directory post-build

pre-build:
	@echo PRE ...
	@echo PRE ... done
	@echo -e

post-build:
	@echo POST ...
	@echo POST ... done
	@echo -e

main-build: clang

CLANG_LDFLAGS = $(CPU) -specs=nano.specs -T$(LDSCRIPT) $(LIB_DIR) $(LIBS) -Wl,-Map=$(CLANG_BUILD)/$(TARGET).map,--cref   -Wl,--gc-sections -Wl,--no-warn-rwx-segments #

CLANG_OBJECTS = 

# list of clang ASM program objects
CLANG_OBJECTS += $(addprefix $(CLANG_BUILD)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# list of clang objects
CLANG_OBJECTS += $(addprefix $(CLANG_BUILD)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# assembler
$(CLANG_BUILD)/%.o: %.s $(MAKEFILE) | $(CLANG_BUILD)
	@echo $<
	@$(CLANG_AS) $(VERBOSE) $(C_FLAGS) $(CLANG_ONLY_FLAGS) -c $< -o $@
#	@echo -

# compiler
$(CLANG_BUILD)/%.o: %.c $(MAKEFILE) | $(CLANG_BUILD) pre-build
	@echo $<
	@$(CLANG_CC) $(VERBOSE) $(C_FLAGS) $(C_DEFS) $(C_INCLUDES) $(CLANG_ONLY_FLAGS) -c $< -o $@
#	@echo -e

# linker # https://www.redhat.com/en/blog/linkers-warnings-about-executable-stacks-and-segments
$(CLANG_BUILD)/$(TARGET).elf: $(CLANG_OBJECTS) $(MAKEFILE)
	@echo linking...
	@$(CC) $(CLANG_OBJECTS) $(MCU) $(CLANG_LDFLAGS) $(VERBOSE) -o $@ 2> gccLinksClangWarning.txt
	@echo -e
	@$(CLANG_SZ) $@

# hex
$(CLANG_BUILD)/%.hex: $(CLANG_BUILD)/%.elf | $(CLANG_BUILD)
	@$(CLANG_HEX) $< $@

# bin
$(CLANG_BUILD)/%.bin: $(CLANG_BUILD)/%.elf | $(CLANG_BUILD)
	@$(CLANG_BIN) $< $@
#	@ls -l $(GCC_BUILD)/*.o
#	@ls -l $(CLANG_BUILD)/$(TARGET).*

$(CLANG_BUILD):
	mkdir $@

# dependencies
-include $(wildcard $(CLANG_BUILD)/*.d)

include mcu.mak
#include flags.mak
