#!/usr/bin/env bash

# file name: _120_setup_trice_environment.sh

TRICE_TIL_JSON=${TRICE_TIL_JSON:-./demoTIL.json}
TRICE_LI_JSON=${TRICE_LI_JSON:-./demoLI.json}
TRICE_LI_ROOT=${TRICE_LI_ROOT:-.}
TRICE_BIND_DIR=${TRICE_BIND_DIR:-./build/triceIDs}
TRICE_ID_OPTIONS=${TRICE_ID_OPTIONS:-"-IDMin 13000 -IDMax 16383"}

# trice command line common part
if [ -z "${TRICE_DEFAULTS:-}" ]; then
  TRICE_DEFAULTS+="-i ${TRICE_TIL_JSON} "     # Use a common til.json for all examples and tests.
  TRICE_DEFAULTS+="-li ${TRICE_LI_JSON} "     # Use a common li.json for all examples and tests.
  TRICE_DEFAULTS+="-liRoot ${TRICE_LI_ROOT} " # Store portable location paths relative to this root.
  #TRICE_DEFAULTS+="-cache "                  # Create ~/.trice/cache before enabling this line.
fi

# Source exclusions are kept separately so the safe re-migration helper can
# reuse the exact ownership scope without having to interpret alias options.
if [ -z "${TRICE_EXCLUDES:-}" ]; then
  TRICE_EXCLUDES+="-exclude ./examples/G0B1_inst/Core/Inc/triceCustomAliases.h "
  TRICE_EXCLUDES+="-exclude ./_test/aliasassert_dblB_de_tcobs_ua/triceConfig.h "
  TRICE_EXCLUDES+="-exclude ./_test/alias_dblB_de_tcobs_ua/triceConfig.h "
fi

# trice user aliases and their shared source exclusions
if [ -z "${TRICE_ALIASES:-}" ]; then
  TRICE_ALIASES+="-alias CUSTOM_PRINT "
  TRICE_ALIASES+="-salias CUSTOM_ASSERT "
  TRICE_ALIASES+="-alias printi "
  TRICE_ALIASES+="-salias prints "
  TRICE_ALIASES+="${TRICE_EXCLUDES}"
fi

# trice user project files
if [ -z "${TRICE_PRJ_FILES:-}" ]; then
  TRICE_PRJ_FILES+="-src ./_test "
  TRICE_PRJ_FILES+="-src ./examples/exampleData "
  TRICE_PRJ_FILES+="-src ./examples/F030_inst/Core "
  TRICE_PRJ_FILES+="-src ./examples/G0B1_inst/Core "
  TRICE_PRJ_FILES+="-src ./examples/L432_inst/Core "
  TRICE_PRJ_FILES+="-src ./examples/DemoData_Trice "
fi
