#!/usr/bin/env bash
#
# Generates the release PDF for the Trice user manual without touching
# docs/TriceUserManual.pdf in the working tree. The PDF is rendered into a
# temp/ path. The GitHub release uploads that file directly, while local helper
# scripts may copy it into dist/ after GoReleaser finished.

set -eu

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

TEMP_MD="temp/release/TriceUserManual.md"
TEMP_PDF="temp/release/TriceUserManual.pdf"
INPUT_MD="docs/TriceUserManual.md"

cd "$REPO_ROOT"

if ! command -v npx >/dev/null 2>&1; then
  echo "ERROR: npx is required to generate the release manual PDF." >&2
  echo "Install Node.js locally or run this step in the GitHub release workflow." >&2
  exit 1
fi

mkdir -p -- "$(dirname -- "$TEMP_PDF")"
cp -f -- "$INPUT_MD" "$TEMP_MD"

# md-to-pdf is used here because it works in headless CI and does not require
# VS Code or GUI automation. The repository keeps the VS Code based workflow for
# local comparison PDFs in docs/, but release assets are generated
# independently. The temp path prevents accidental overwrites of any local,
# git-ignored comparison PDF under docs/.
npx -y md-to-pdf@5.2.4 \
  "$TEMP_MD" \
  --basedir . \
  --launch-options '{"args":["--no-sandbox"]}' \
  --pdf-options '{"format":"A4","printBackground":true}'
