#!/bin/bash

# Trice command line common part
#TRICE_CL+="-cache "                # Do not forget to create ~/.trice/cache folder, if the Trice cache should work or disable this line.
TRICE_CL+="-i ./demoTIL.json "     # Use a common til.json for all examples and tests.
TRICE_CL+="-li ./demoLI.json "     # Use a common  li.json for all examples and tests.
TRICE_CL+="-liPath relative "      # Prefix base filenames in li.json with relative path for new IDs.
TRICE_CL+="-src ./_test "
TRICE_CL+="-src ./examples/exampleData "
TRICE_CL+="-src ./examples/F030_inst/Core "
TRICE_CL+="-src ./examples/G0B1_inst/Core "
TRICE_CL+="-src ./examples/L432_inst/Core "
