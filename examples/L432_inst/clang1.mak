# Defer the vendor runtime's GNU-stack warning while measuring logging speed.
CLANG_LDFLAGS = $(CPU) -specs=nano.specs -T$(LDSCRIPT) $(LIB_DIR) $(LIBS) -Wl,-Map=$(CLANG_BUILD)/$(TARGET).map,--cref   -Wl,--gc-sections -Wl,--no-warn-rwx-segments -Wl,--no-warn-execstack #

CLANG_OBJECTS = 

# list of clang ASM program objects
CLANG_OBJECTS += $(addprefix $(CLANG_BUILD)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# list of clang objects
CLANG_OBJECTS += $(addprefix $(CLANG_BUILD)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# assembler
$(CLANG_BUILD)/%.o: %.s $(BUILDFILES) | $(CLANG_BUILD)
	@echo $<
	@$(CLANG_AS) $(VERBOSE) $(CLANG_C_FLAGS) $(CLANG_ONLY_FLAGS) -c $< -o $@
#	@echo -

# compiler
$(CLANG_BUILD)/%.o: %.c $(BUILDFILES) | $(CLANG_BUILD)
	@echo $<
	@$(CLANG_CC) $(VERBOSE) $(CLANG_C_FLAGS) $(C_DEFS) $(C_INCLUDES) $(CLANG_ONLY_FLAGS) -c $< -o $@
#	@echo -e

# linker # https://www.redhat.com/en/blog/linkers-warnings-about-executable-stacks-and-segments
$(CLANG_BUILD)/$(TARGET).elf: $(CLANG_OBJECTS) $(BUILDFILES)
	@echo linking...
	@$(GCC_CC) $(CLANG_OBJECTS) $(MCU) $(CLANG_LDFLAGS) $(VERBOSE) -o $@ 2> gccLinksClangWarning.txt || { status=$$?; cat gccLinksClangWarning.txt >&2; exit $$status; }
# Keep the existing diagnostic file and also show warnings/errors in the terminal.
	@cat gccLinksClangWarning.txt >&2
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
# Forced builds also rerun this rule; preserve an existing output directory.
	mkdir -p "$@"

# dependencies
-include $(wildcard $(CLANG_BUILD)/*.d)
