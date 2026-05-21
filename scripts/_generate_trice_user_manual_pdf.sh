#!/usr/bin/env bash
#
# Generate docs/TriceUserManual.pdf from docs/TriceUserManual.md.
#
# This is the single PDF generation entry point used by developers, CI, and
# GoReleaser. It does not require VS Code and writes the PDF where the manual
# belongs: next to the Markdown source in docs/.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

MANUAL_FILE="docs/TriceUserManual.md"
PDF_FILE="docs/TriceUserManual.pdf"
REF_DIR="docs/ref"

cd "$REPO_ROOT"

die() {
  echo "ERROR: $*" >&2
  exit 1
}

pdf_size() {
  wc -c <"$1" | tr -d '[:space:]'
}

show_context() {
  echo "PDF generation context:" >&2
  echo "  repo:   $REPO_ROOT" >&2
  echo "  input:  $MANUAL_FILE" >&2
  echo "  output: $PDF_FILE" >&2
  echo "  os:     ${OSTYPE:-unknown}" >&2

  if command -v node >/dev/null 2>&1; then
    echo "  node:   $(node --version 2>/dev/null || true)" >&2
  else
    echo "  node:   not found" >&2
  fi

  if command -v npm >/dev/null 2>&1; then
    echo "  npm:    $(npm --version 2>/dev/null || true)" >&2
  else
    echo "  npm:    not found" >&2
  fi

  if command -v npx >/dev/null 2>&1; then
    echo "  npx:    $(npx --version 2>/dev/null || true)" >&2
  else
    echo "  npx:    not found" >&2
  fi

  echo "  related files:" >&2
  find docs -maxdepth 3 \( -name 'TriceUserManual.md' -o -name 'TriceUserManual.pdf' \) -print 2>/dev/null | sort >&2 || true
}

if [[ ! -s "$MANUAL_FILE" ]]; then
  show_context
  die "missing or empty manual file: $MANUAL_FILE"
fi

if [[ ! -d "$REF_DIR" ]]; then
  show_context
  die "missing manual reference directory: $REF_DIR"
fi

if ! command -v npx >/dev/null 2>&1; then
  show_context
  die "npx is required. Install Node.js/npm."
fi

generated_at="$(date '+%Y-%m-%d %H:%M %Z')"

# The Markdown manual keeps its table of contents inside <details> for HTML
# reading. For the PDF we render that content visibly by CSS instead of editing
# the source file or building a temporary Markdown copy.
css='details > summary { display: none; }
details:not([open]) > :not(summary) { display: block; }
img { max-width: 100%; height: auto; }'

pdf_options="$(
  cat <<EOF_JSON
{
  "format": "A4",
  "printBackground": true,
  "displayHeaderFooter": true,
  "margin": {
    "top": "22mm",
    "right": "15mm",
    "bottom": "18mm",
    "left": "15mm"
  },
  "headerTemplate": "<div style=\"width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:space-between; align-items:center;\"><span>TriceUserManual.md</span><span>${generated_at}</span></div>",
  "footerTemplate": "<div style=\"width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:flex-end; align-items:center;\"><span><span class=\"pageNumber\"></span>/<span class=\"totalPages\"></span></span></div>"
}
EOF_JSON
)"

launch_options='{"args":["--no-sandbox"]}'

rm -f -- "$PDF_FILE"

echo "Generating manual PDF from $MANUAL_FILE"
echo "Output PDF: $PDF_FILE"

if ! npx -y md-to-pdf@5.2.4 \
  "$MANUAL_FILE" \
  --basedir "$REPO_ROOT" \
  --css "$css" \
  --pdf-options "$pdf_options" \
  --launch-options "$launch_options"; then
  show_context
  die "md-to-pdf failed"
fi

if [[ ! -s "$PDF_FILE" ]]; then
  show_context
  die "missing or empty generated PDF: $PDF_FILE"
fi

echo "Generated manual PDF: $PDF_FILE ($(pdf_size "$PDF_FILE") bytes)"
