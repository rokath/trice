#!/usr/bin/env bash
#
# Internal runner for testAll.sh.
#
# Direct invocation:
# - Not intended. Use ./testAll.sh instead.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local selected
  selected="$(get_mode "${1:-quick}")"
  export SELECTED="$selected"

  ./_testAll_01_CleanDsStore.sh || exit $?
  ./_testAll_02_ClangFormat.sh || exit $?
  ./_testAll_03_BuildTriceTool.sh || exit $?
  ./_testAll_04_FormatManual.sh || exit $?
  ./_testAll_05_MarkdownLint.sh || exit $?
  ./_testAll_06_LinkCheck.sh || exit $?
  ./_testAll_07_GoCoverage.sh || exit $?
  ./_testAll_08_RuntimePrepare.sh || exit $?
  ./_testAll_09_GoTests.sh || exit $?
  ./_testAll_10_PcTargetTests.sh "$selected" || exit $?
  ./_testAll_11_ClangTranslation.sh || exit $?
  ./_testAll_12_GccExampleBuilds.sh || exit $?

  if [ "$selected" = "full" ]; then
    ./_testAll_13_L432Configs.sh || exit $?
  fi
}

main "$@"
