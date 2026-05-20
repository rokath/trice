#!/usr/bin/env bash
#
# Build the release PDF for docs/TriceUserManual.md.
#
# The generated release asset is:
#
#   temp/release/TriceUserManual.pdf
#
# The script intentionally works in temp/release/ so it does not overwrite the
# local comparison PDF docs/TriceUserManual.pdf.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

INPUT_MD="docs/TriceUserManual.md"
INPUT_REF_DIR="docs/ref"

TEMP_ROOT="temp/release"
TEMP_DOC_DIR="$TEMP_ROOT/docs"
TEMP_MD="$TEMP_DOC_DIR/TriceUserManual.md"
TEMP_REF_DIR="$TEMP_DOC_DIR/ref"
TEMP_DOC_PDF="$TEMP_DOC_DIR/TriceUserManual.pdf"
TEMP_PDF="$TEMP_ROOT/TriceUserManual.pdf"
TEMP_CONFIG_JS="$TEMP_ROOT/md-to-pdf.config.js"

cd "$REPO_ROOT"

die() {
  echo "ERROR: $*" >&2
  exit 1
}

pdf_size() {
  wc -c <"$1" | tr -d '[:space:]'
}

require_file() {
  local file="$1"

  if [[ ! -s "$file" ]]; then
    die "missing or empty file: $file"
  fi
}

prepare_manual_sources() {
  rm -rf -- "$TEMP_DOC_DIR"
  mkdir -p -- "$TEMP_DOC_DIR"

  cp -f -- "$INPUT_MD" "$TEMP_MD"

  if [[ -d "$INPUT_REF_DIR" ]]; then
    cp -R -- "$INPUT_REF_DIR" "$TEMP_REF_DIR"
  fi

  # The repository manual keeps the TOC collapsible for HTML reading. In the
  # release PDF the same TOC should be visible without user interaction.
  perl -0pi -e 's#<details markdown="1">#<details open markdown="1">#' "$TEMP_MD"
}

write_pdf_config() {
  local generated_at="$1"

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
    headerTemplate: '<div style="width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:space-between; align-items:center;"><span>TriceUserManual.md</span><span>${generated_at}</span></div>',
    footerTemplate: '<div style="width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:flex-end; align-items:center;"><span><span class="pageNumber"></span>/<span class="totalPages"></span></span></div>',
  },
};
EOF
}

copy_finished_pdf() {
  require_file "$TEMP_DOC_PDF"

  cp -f -- "$TEMP_DOC_PDF" "$TEMP_PDF"
  require_file "$TEMP_PDF"

  echo "Generated release manual PDF: $TEMP_PDF ($(pdf_size "$TEMP_PDF") bytes)"
}

if [[ -s "$TEMP_PDF" && "$TEMP_PDF" -nt "$INPUT_MD" ]]; then
  mkdir -p -- "$TEMP_DOC_DIR"
  cp -f -- "$TEMP_PDF" "$TEMP_DOC_PDF"
  echo "Reusing existing release manual PDF: $TEMP_PDF ($(pdf_size "$TEMP_PDF") bytes)"
  exit 0
fi

command -v npx >/dev/null 2>&1 ||
  die "npx is required. Install Node.js or run this in the GitHub release workflow."

require_file "$INPUT_MD"

echo "Generating release manual PDF from $INPUT_MD"
echo "Output PDF: $TEMP_PDF"

prepare_manual_sources
write_pdf_config "$(date '+%Y-%m-%d %H:%M %Z')"

# md-to-pdf works in headless CI and does not require VS Code or GUI automation.
npx -y md-to-pdf@5.2.4 \
  "$TEMP_MD" \
  --basedir "$TEMP_ROOT" \
  --config-file "$TEMP_CONFIG_JS" \
  --launch-options '{"args":["--no-sandbox"]}'

copy_finished_pdf
