#!/bin/bash

cd ../../ # operate in main project root folder (here it is the trice root folder)

# Trice command line (common part)
CL+="-cache "                # Do not forget to create ~/.trice/cache folder, if the Trice cache should work.
CL+="-i ./demoTIL.json "     # Use a common til.json for all examples and tests.
CL+="-li ./demoLI.json "     # Use a common  li.json for all examples and tests.
CL+="-liPath relative "      # Prefix base filenames in li.json with relative path for new IDs.
CL+="-src ./_test/testdata " # Do not include all files in ./_test/.
CL+="-src ./examples/exampleData "
CL+="-src ./examples/F030_inst/Core "

trice clean  $CL

cd - > /dev/null
