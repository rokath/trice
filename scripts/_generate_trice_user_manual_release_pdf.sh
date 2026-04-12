#!/usr/bin/env bash
#
# Generates the release PDF for the Trice user manual without touching
# docs/TriceUserManual.pdf in the working tree. A dedicated temp/ tree is used
# so the release-PDF preparation can adjust print-specific details such as an
# expanded table of contents, header/footer, and image paths without modifying
# the repository sources.

set -eu

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

TEMP_ROOT="temp/release"
TEMP_DOC_DIR="$TEMP_ROOT/docs"
TEMP_MD="$TEMP_DOC_DIR/TriceUserManual.md"
TEMP_DOC_PDF="$TEMP_DOC_DIR/TriceUserManual.pdf"
TEMP_PDF="$TEMP_ROOT/TriceUserManual.pdf"
TEMP_REF_LINK="$TEMP_DOC_DIR/ref"
TEMP_CONFIG_JS="$TEMP_ROOT/md-to-pdf.config.js"
INPUT_MD="docs/TriceUserManual.md"

cd "$REPO_ROOT"

if ! command -v npx >/dev/null 2>&1; then
  echo "ERROR: npx is required to generate the release manual PDF." >&2
  echo "Install Node.js locally or run this step in the GitHub release workflow." >&2
  exit 1
fi

mkdir -p -- "$TEMP_DOC_DIR"
cp -f -- "$INPUT_MD" "$TEMP_MD"
ln -sfn "../../../docs/ref" "$TEMP_REF_LINK"

# The repository manual keeps the TOC collapsible for HTML reading. For the
# generated release PDF the same TOC should be visible without user interaction.
perl -0pi -e 's#<details markdown="1">#<details open markdown="1">#' "$TEMP_MD"

GENERATED_AT="$(date '+%Y-%m-%d %H:%M %Z')"
cat >"$TEMP_CONFIG_JS" <<EOF
module.exports = {
  css: [
    'details[open] > summary { display: none; }',
    'img { max-width: 100%; height: auto; }',
  ].join('\\n'),
  pdf_options: {
    format: 'A4',
    printBackground: true,
    displayHeaderFooter: true,
    margin: {
      top: '22mm',
      right: '15mm',
      bottom: '18mm',
      left: '15mm',
    },
    headerTemplate: '<div style="width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:space-between; align-items:center;"><span>TriceUserManual.md</span><span>${GENERATED_AT}</span></div>',
    footerTemplate: '<div style="width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:flex-end; align-items:center;"><span><span class="pageNumber"></span>/<span class="totalPages"></span></span></div>',
  },
};
EOF

# md-to-pdf is used here because it works in headless CI and does not require
# VS Code or GUI automation. The repository keeps the VS Code based workflow for
# local comparison PDFs in docs/, but release assets are generated
# independently. The temp tree prevents accidental overwrites of any local,
# git-ignored comparison PDF under docs/ while still allowing release-specific
# print tuning.
npx -y md-to-pdf@5.2.4 \
  "$TEMP_MD" \
  --basedir "$TEMP_ROOT" \
  --config-file "$TEMP_CONFIG_JS" \
  --launch-options '{"args":["--no-sandbox"]}'

cp -f -- "$TEMP_DOC_PDF" "$TEMP_PDF"
