#!/bin/bash

# trice command line common part
TRICE_DEFAULTS+="-i ./demoTIL.json "     # Use a common til.json for all examples and tests.
TRICE_DEFAULTS+="-li ./demoLI.json "     # Use a common  li.json for all examples and tests.
TRICE_DEFAULTS+="-liPath relative "      # Prefix base filenames in li.json with relative path for new IDs.
#TRICE_DEFAULTS+="-cache "                # Do not forget to create ~/.trice/cache folder, if the Trice cache should work or disable this line.

# trice user aliases
TRICE_ALIASES+="-alias CUSTOM_PRINT "
TRICE_ALIASES+="-salias CUSTOM_ASSERT "
TRICE_ALIASES+="-exclude ./examples/G0B1_inst/Core/Inc/triceCustomAliases.h "
TRICE_ALIASES+="-exclude ./_test/aliasassert_dblB_de_tcobs_ua/triceConfig.h "
TRICE_ALIASES+="-alias  printi "
TRICE_ALIASES+="-salias prints "
TRICE_ALIASES+="-exclude ./_test/alias_dblB_de_tcobs_ua/triceConfig.h "

# trice user project files
TRICE_PRJ_FILES+="-src ./_test "
TRICE_PRJ_FILES+="-src ./examples/exampleData "
TRICE_PRJ_FILES+="-src ./examples/F030_inst/Core "
TRICE_PRJ_FILES+="-src ./examples/G0B1_inst/Core "
TRICE_PRJ_FILES+="-src ./examples/L432_inst/Core "
