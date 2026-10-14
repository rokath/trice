#!/bin/bash

# Deliberate legacy maintenance: Clean/Insert/Clean, then copy the canonical
# CGO harness into the configured test packages and clean example build outputs.
# Usage: ./scripts/_330_renew_ids_and_refresh_tests.sh [keepHistory]
# Without keepHistory this discards the shared TIL/LI history and reseeds IDs.
# For ordinary Bind maintenance use trice_bindIDs_in_examples_and_test_folder.sh.
# Never run either source-changing maintenance command alongside testAll.sh.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1
source "$SCRIPT_DIR/_120_setup_trice_environment.sh"
TD="./_test/testdata"

KEEP_HISTORY=0
case "${1:-}" in
  "") ;;
  keepHistory) KEEP_HISTORY=1 ;;
  *)
    echo "usage: $0 [keepHistory]" >&2
    exit 2
    ;;
esac

# Keep the legacy ID order and ranges unchanged. Preflight below must finish
# before this function may discard metadata or modify any instrumented source.
renew_id_data() {
  mkdir -p "$(dirname "$TRICE_TIL_JSON")" "$(dirname "$TRICE_LI_JSON")"
  if [ "$KEEP_HISTORY" -eq 0 ]; then
    rm -f "$TRICE_TIL_JSON" "$TRICE_LI_JSON" # forget history (users usually should not do that in their projects, deleted here to avoid potential ID conflict messages)
  fi
  touch "$TRICE_TIL_JSON" "$TRICE_LI_JSON"                    # create files, or keep history when keepHistory is used
  trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES # wipe out all IDs from the sources
  if [ "$KEEP_HISTORY" -eq 0 ]; then
    rm -f "$TRICE_TIL_JSON" "$TRICE_LI_JSON" # forget history (in case the sources contained IDs, these are now removed from there, but are kept in the *.json files, so delete them again.)
    touch "$TRICE_TIL_JSON" "$TRICE_LI_JSON" # new life
  fi

  # Next steps are done separately to get the same IDs continuously, in case we deleted the history - normally all files and folders can be done parallel in one shot.
  # We do not use -cache here to force the li.json generation.
  # The Trice tool per default chooses IDs randomly between 1000 and 7999.
  # On changing the test and example projects adapt $TRICE_PRJ_FILES too.
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceLogDiagData.c
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceExamples.c
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src ./examples/F030_inst/Core
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src ./examples/G0B1_inst/Core
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src ./examples/L432_inst/Core
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src $TD/triceCheck.c
  trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src $TD/..
  trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES # IDs are now inside the til.json (and li.json) files.
}

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.

# Add a new maintained CGO configuration to this explicit list. Normal test
# runs use overlays; these on-disk copies are refreshed only by this helper.
CGOTESTDIRS="
 alias_dblB_de_tcobs_ua/
 aliasassert_dblB_de_tcobs_ua/
 be_dblB_de_tcobs_ua/
 be_staticB_di_xtea_cobs_rtt32/
 dblB_de_protect_tcobs_ua/
 dblB_de_multi_cobs_ua/
 dblB_de_multi_nopf_ua/
 dblB_de_multi_tcobs_ua/
 dblB_de_multi_xtea_cobs_ua/
 dblB_de_multi_xtea_tcobs_ua/
 dblB_de_single_cobs_ua/
 dblB_de_single_nopf_ua/
 dblB_de_single_tcobs_ua/
 dblB_de_single_xtea_cobs_ua/
 dblB_de_single_xtea_tcobs_ua/
 dblB_di_cobs_rtt32__de_xtea_cobs_ua/
 dblB_di_nopf_rtt32__de_cobs_ua/
 dblB_di_nopf_rtt32__de_multi_cobs_ua/
 dblB_di_nopf_rtt32__de_multi_tcobs_ua/
 dblB_di_nopf_rtt32__de_tcobs_ua/
 dblB_di_nopf_rtt32__de_xtea_cobs_ua/
 dblB_di_nopf_rtt8__de_cobs_ua/
 dblB_di_nopf_rtt8__de_multi_cobs_ua/
 dblB_di_nopf_rtt8__de_multi_tcobs_ua/
 dblB_di_nopf_rtt8__de_tcobs_ua/
 modify_for_debug/
 ringB_de_protect_tcobs_ua/
 ringB_de_multi_cobs_ua/
 ringB_de_multi_nopf_ua/
 ringB_de_multi_tcobs_ua/
 ringB_de_multi_xtea_cobs_ua/
 ringB_de_multi_xtea_tcobs_ua/
 ringB_de_single_cobs_ua/
 ringB_de_single_nopf_ua/
 ringB_de_single_tcobs_ua/
 ringB_de_single_xtea_cobs_ua/
 ringB_de_single_xtea_tcobs_ua/
 ringB_di_cobs_rtt32__de_tcobs_ua/
 ringB_di_cobs_rtt32__de_xtea_cobs_ua/
 ringB_di_cobs_rtt8__de_tcobs_ua/
 ringB_di_nopf_rtt32__de_tcobs_ua/
 ringB_di_nopf_rtt32__de_xtea_cobs_ua/
 ringB_di_nopf_rtt8__de_tcobs_ua/
 ringB_di_tcobs_rtt32__de_tcobs_ua/
 ringB_di_xtea_cobs_rtt32__de_cobs_ua/
 ringB_di_xtea_cobs_rtt32__de_tcobs_ua/
 ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua/
 stackB_di_nopf_aux32_specific/
 stackB_di_nopf_aux32/
 stackB_di_nopf_aux8/
 stackB_di_nopf_rtt32/
 stackB_di_nopf_rtt8/
 stackB_di_xtea_cobs_rtt8/
 staticB_di_nopf_aux32/
 staticB_di_nopf_aux8/
 staticB_di_nopf_rtt32/
 staticB_di_nopf_rtt8/
 staticB_di_tcobs_rtt32/
 staticB_di_tcobs_rtt8/
 staticB_di_xtea_cobs_rtt32/
 userprint_dblB_de_tcobs_ua/
 "

# Reject missing inputs before the intentional history reset or source writes.
if ! command -v trice >/dev/null 2>&1; then
  echo "ERROR: trice is required; build it with ./scripts/buildTriceTool.sh first." >&2
  exit 1
fi
for template in "$TD/cgoPackage.go" "$TD/cgoPackage_test.go"; do
  if [ ! -s "$template" ]; then
    echo "ERROR: canonical CGO template not found or empty: $template" >&2
    exit 1
  fi
done
for d in $CGOTESTDIRS; do
  if [ ! -d "$TD/../$d" ]; then
    echo "ERROR: configured CGO test directory not found: $TD/../$d; check CGOTESTDIRS in $0." >&2
    exit 1
  fi
done
if [ "$KEEP_HISTORY" -eq 0 ]; then
  echo "WARNING: legacy ID re-seeding discards $TRICE_TIL_JSON and $TRICE_LI_JSON history."
else
  echo "INFO: legacy ID refresh retains shared TIL/LI history."
fi
renew_id_data

# Refresh test folders.
for d in $CGOTESTDIRS; do
  cp $TD/cgoPackage.go $TD/../$d/generated_cgoPackage.go
  cp $TD/cgoPackage_test.go $TD/../$d/generated_cgoPackage_test.go
done

# Remove legacy build artifacts if existent.
cd ./examples || exit 1
./cleanAllTargets.sh
cd - >/dev/null || exit 1

####################
