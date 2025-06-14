#!/bin/bash

# Trice command line common part
TRICE_CMD_LINE+="-cache "                # Do not forget to create ~/.trice/cache folder, if the Trice cache should work or disable this line.
TRICE_CMD_LINE+="-i ./demoTIL.json "     # Use a common til.json for all examples and tests.
TRICE_CMD_LINE+="-li ./demoLI.json "     # Use a common  li.json for all examples and tests.
TRICE_CMD_LINE+="-liPath relative "      # Prefix base filenames in li.json with relative path for new IDs.
TRICE_CMD_LINE+="-src ./_test "
TRICE_CMD_LINE+="-src ./examples/exampleData "
TRICE_CMD_LINE+="-src ./examples/F030_inst/Core "
TRICE_CMD_LINE+="-src ./examples/G0B1_inst/Core "
TRICE_CMD_LINE+="-src ./examples/L432_inst/Core "

# Aliases
TRICE_CMD_LINE+="-alias CUSTOM_PRINT "
TRICE_CMD_LINE+="-salias CUSTOM_ASSERT "
TRICE_CMD_LINE+="-exclude ./examples/G0B1_inst/Core/Inc/triceCustomAliases.h "
