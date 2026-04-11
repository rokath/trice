#!/usr/bin/env bash
#
# Generates the release PDF for the Trice user manual without touching
# docs/TriceUserManual.pdf in the working tree. The PDF is first rendered into a
# temp/ path and then copied into dist/ as the final release artifact so local
# snapshot runs and the GitHub release workflow use the same generated file.

set -eu

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

OUTPUT_PDF="${1:-dist/TriceUserManual.pdf}"
TEMP_PDF="temp/release/TriceUserManual.pdf"
INPUT_MD="docs/TriceUserManual.md"

cd "$REPO_ROOT"

if ! command -v npx >/dev/null 2>&1; then
  echo "ERROR: npx is required to generate the release manual PDF." >&2
  echo "Install Node.js locally or run this step in the GitHub release workflow." >&2
  exit 1
fi

mkdir -p -- "$(dirname -- "$OUTPUT_PDF")" "$(dirname -- "$TEMP_PDF")"

# md-to-pdf is used here because it works in headless CI and does not require
# VS Code or GUI automation. The repository keeps the VS Code based workflow for
# local comparison PDFs in docs/, but release assets are generated
# independently. The temp path prevents accidental overwrites of any local,
# git-ignored comparison PDF under docs/.
npx -y md-to-pdf@5.2.4 \
  "$INPUT_MD" \
  --dest "$TEMP_PDF" \
  --basedir . \
  --launch-options '{"args":["--no-sandbox"]}' \
  --pdf-options '{"format":"A4","printBackground":true}'

cp -f -- "$TEMP_PDF" "$OUTPUT_PDF"
