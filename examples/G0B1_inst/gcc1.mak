GCC_LDFLAGS = $(CPU) -specs=nano.specs -T$(LDSCRIPT) $(LIB_DIR) $(LIBS) -Wl,-Map=$(GCC_BUILD)/$(TARGET).map,--cref   -Wl,--gc-sections -Wl,--no-warn-rwx-segments #

GCC_OBJECTS =

# list of gcc ASM program objects
GCC_OBJECTS += $(addprefix $(GCC_BUILD)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# list of gcc objects
GCC_OBJECTS += $(addprefix $(GCC_BUILD)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# assembler
$(GCC_BUILD)/%.o: %.s $(BUILDFILES) | $(GCC_BUILD)
	@echo $<
	@$(GCC_AS) $(VERBOSE) $(C_FLAGS) -c $< -o $@
#	@echo -e

# compiler
$(GCC_BUILD)/%.o: %.c $(BUILDFILES) | $(GCC_BUILD)
	@echo $<
	@$(GCC_CC) $(VERBOSE) $(C_FLAGS) $(C_DEFS) $(C_INCLUDES) $(GCC_ONLY_FLAGS) -c $< -o $@
#	@echo -e

# linker #
$(GCC_BUILD)/$(TARGET).elf: $(GCC_OBJECTS) $(BUILDFILES)
#	@echo linking...
	@$(GCC_CC) $(GCC_OBJECTS) $(MCU) $(GCC_LDFLAGS) $(VERBOSE) -o $@
	@$(GCC_SZ) $@

# hex
$(GCC_BUILD)/%.hex: $(GCC_BUILD)/%.elf | $(GCC_BUILD)
	@$(GCC_HEX) $< $@

# bin
$(GCC_BUILD)/%.bin: $(GCC_BUILD)/%.elf | $(GCC_BUILD)
	@$(GCC_BIN) $< $@
#	@ls -l $(GCC_BUILD)/*.o
#	@ls -l $(GCC_BUILD)/$(TARGET).*
	
$(GCC_BUILD):
	mkdir $@

# dependencies
-include $(wildcard $(GCC_BUILD)/*.d)
