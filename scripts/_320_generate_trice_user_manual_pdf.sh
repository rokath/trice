#!/usr/bin/env bash
#
# Generate the User Manual PDF and a Reference Manual PDF from its chapter sources.
#
# This is the single PDF generation entry point used by developers, CI, and
# GoReleaser. It does not require VS Code and writes the PDF where the manual
# belongs: in docs/. Disposable RM inputs are rebuilt under temp/manual-pdf/.
# Use `setup` to validate inputs and prepare pinned tools without rendering PDFs.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

# Keep both manuals in the shared developer, CI and release rendering path.
MANUAL_FILES=("docs/TriceUserManual.md" "temp/manual-pdf/TriceReferenceManual.md")
REF_DIR="docs/ref"

cd "$REPO_ROOT"
# shellcheck source=./_105_setup_test_tools.sh
source "$SCRIPT_DIR/_105_setup_test_tools.sh"
MD_TO_PDF_PACKAGE="md-to-pdf@${TRICE_TOOL_MD_TO_PDF_VERSION:-5.2.4}"
PUPPETEER_PACKAGE="puppeteer@${TRICE_TOOL_PUPPETEER_VERSION:-24.31.0}"
STITCHMD_VERSION="${TRICE_TOOL_STITCHMD_VERSION:-0.9.0}"

# Resolve setup separately so callers can obtain installation consent before
# launching a release tool whose captured hooks cannot read interactive input.
MODE="${1:-generate}"
if [[ "$#" -gt 1 ]] || [[ "$MODE" != generate && "$MODE" != setup ]]; then
  echo "Usage: $0 [generate|setup]" >&2
  exit 2
fi

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

  echo "  renderer: $MD_TO_PDF_PACKAGE with $PUPPETEER_PACKAGE" >&2
  if command -v stitchmd >/dev/null 2>&1; then
    echo "  stitchmd path: $(command -v stitchmd)" >&2
    stitchmd -version >&2 || true
  else
    echo "  stitchmd: not found (required: $STITCHMD_VERSION)" >&2
  fi

  echo "  related files:" >&2
  find docs -maxdepth 1 \( -name 'Trice*Manual.md' -o -name 'Trice*Manual.pdf' \) -print 2>/dev/null | sort >&2 || true
  find temp/manual-pdf -maxdepth 2 -type f -print 2>/dev/null | sort >&2 || true
}

# Validate the maintained entry points before preparing or replacing either PDF.
for MANUAL_FILE in "docs/TriceUserManual.md" "docs/rm/README.md"; do
  PDF_FILE="${MANUAL_FILE%.md}.pdf"
  if [[ "$MANUAL_FILE" == docs/rm/README.md ]]; then
    PDF_FILE="docs/TriceReferenceManual.pdf"
  fi
  if [[ ! -s "$MANUAL_FILE" ]]; then
    show_context
    die "missing or empty manual file: $MANUAL_FILE"
  fi
done

if [[ ! -d "$REF_DIR" ]]; then
  show_context
  die "missing manual reference directory: $REF_DIR"
fi

if ! command -v npx >/dev/null 2>&1; then
  show_context
  die "npx is required. Install Node.js/npm."
fi

# Use the same pinned stitcher locally and in the GitHub release/snapshot jobs.
# Keep the existing build-information check for Go-installed dev binaries.
validate_stitchmd() {
  if ! command -v stitchmd >/dev/null 2>&1; then
    stitchmd_error="stitchmd $STITCHMD_VERSION is required; install with: go install go.abhg.dev/stitchmd@v$STITCHMD_VERSION"
    return 1
  fi
  if ! stitchmd_identity="$(stitchmd -version 2>&1)"; then
    stitchmd_error="cannot read stitchmd version: $stitchmd_identity"
    return 1
  fi
  case "$(printf '%s\n' "$stitchmd_identity" | tr -d '\r' | sed -n '1p')" in
    "stitchmd $STITCHMD_VERSION") return 0 ;;
    "stitchmd dev")
      # go install preserves the pinned module version in build information, but
      # stitchmd's display version is only populated by its release linker flags.
      # Inspect the selected executable, including MSYS's implicit .exe suffix.
      stitchmd_binary="$(command -v stitchmd)"
      case "${OSTYPE:-}" in
        msys* | cygwin*)
          # MSYS's -f also accepts an implicit suffix; Go needs it explicitly.
          if [[ -f "$stitchmd_binary.exe" ]]; then stitchmd_binary="$stitchmd_binary.exe"; fi
          ;;
      esac
      if ! command -v go >/dev/null 2>&1; then
        stitchmd_error="Go is required to verify stitchmd dev; install with: go install go.abhg.dev/stitchmd@v$STITCHMD_VERSION"
        return 1
      fi
      if ! stitchmd_build="$(go version -m "$stitchmd_binary" 2>&1)"; then
        stitchmd_error="cannot read stitchmd build information for $stitchmd_binary: $stitchmd_build"
        return 1
      fi
      # A matching dependency is insufficient: both the main package and module
      # must match, and replacements would invalidate the pinned source identity.
      if ! printf '%s\n' "$stitchmd_build" | tr -d '\r' | awk \
        -v module=go.abhg.dev/stitchmd -v version="v$STITCHMD_VERSION" '
          $1 == "path" { package_ok = ($2 == module && NF == 2) }
          $1 == "mod" { module_ok = ($2 == module && $3 == version) }
          $1 == "=>" { replaced = 1 }
          END { exit !(package_ok && module_ok && !replaced) }
        '; then
        printf '%s\n' "$stitchmd_build" >&2
        stitchmd_error="stitchmd dev must be built from go.abhg.dev/stitchmd@v$STITCHMD_VERSION without replacements; install with: go install go.abhg.dev/stitchmd@v$STITCHMD_VERSION"
        return 1
      fi
      echo "Verified stitchmd v$STITCHMD_VERSION from Go build information: $stitchmd_binary"
      return 0
      ;;
    *)
      stitchmd_error="stitchmd $STITCHMD_VERSION is required for reproducible PDF assembly; install with: go install go.abhg.dev/stitchmd@v$STITCHMD_VERSION"
      return 1
      ;;
  esac
}

if ! validate_stitchmd; then
  if ! confirm_tool_install "stitchmd $STITCHMD_VERSION"; then
    show_context
    die "$stitchmd_error"
  fi
  install_go_test_tool "go.abhg.dev/stitchmd@v$STITCHMD_VERSION" || die "stitchmd installation failed"
  if ! validate_stitchmd; then
    show_context
    die "$stitchmd_error"
  fi
fi

if [[ "$MODE" == setup ]]; then
  echo "Manual PDF prerequisites ready: stitchmd $STITCHMD_VERSION and npx."
  exit 0
fi

# Preparation and verification run inside the renderer's npx environment so they
# reuse its Markdown parser/highlighter. Only the fixed staging tree is reset.
if ! npx -y --package="$MD_TO_PDF_PACKAGE" --package="$PUPPETEER_PACKAGE" \
  -c 'node scripts/_prepare_reference_manual.cjs'; then
  show_context
  die "reference manual preparation failed"
fi
if ! stitchmd -no-toc -o temp/manual-pdf/TriceReferenceManual.md temp/manual-pdf/SUMMARY.md; then
  show_context
  die "stitchmd failed to assemble temp/manual-pdf/TriceReferenceManual.md"
fi
if ! npx -y --package="$MD_TO_PDF_PACKAGE" --package="$PUPPETEER_PACKAGE" \
  -c 'node scripts/_prepare_reference_manual.cjs --verify'; then
  show_context
  die "reference manual link/anchor verification failed"
fi

generated_at="$(date '+%Y-%m-%d %H:%M %Z')"

# The User Manual keeps its table of contents inside <details> for HTML reading.
# Print it visibly without editing its source. The compiled RM has one global TOC.
css='details > summary { display: none; }
details:not([open]) > :not(summary) { display: block; }
img { max-width: 100%; height: auto; }'

for MANUAL_FILE in "${MANUAL_FILES[@]}"; do
  PDF_FILE="${MANUAL_FILE%.md}.pdf"
  OUTPUT_FILE="docs/${PDF_FILE##*/}"
  # Scope the global contents styling to the assembled RM; the UM keeps its layout.
  manual_css="$css"
  if [[ "$MANUAL_FILE" == temp/manual-pdf/TriceReferenceManual.md ]]; then
    manual_css="$css
body { font-size: 11pt; overflow-wrap: anywhere; }
pre, code { overflow-wrap: anywhere; }
pre code { font-size: 0.9em; }
table { display: table; table-layout: fixed; }
table th, table td { padding: 0.35em 0.5em; }
h2#contents { break-before: page; }
h2:has(> a.rm-main-chapter) { break-before: page; }
.book-contents > li > a { font-weight: bold; }
.book-contents .toc-depth-3 { margin-left: 1em; }
.book-contents .toc-depth-4 { margin-left: 2em; }
h2, h3, h4, h5, h6 { break-after: avoid-page; }"
  fi
  pdf_options="$(
    cat <<EOF_JSON
{
  "format": "A4",
  "outline": true,
  "printBackground": true,
  "displayHeaderFooter": true,
  "margin": {
    "top": "22mm",
    "right": "15mm",
    "bottom": "18mm",
    "left": "15mm"
  },
  "headerTemplate": "<div style=\"width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:space-between; align-items:center;\"><span>${MANUAL_FILE##*/}</span><span>${generated_at}</span></div>",
  "footerTemplate": "<div style=\"width:100%; font-size:8px; color:#666; padding:0 10mm; display:flex; justify-content:flex-end; align-items:center;\"><span><span class=\"pageNumber\"></span>/<span class=\"totalPages\"></span></span></div>"
}
EOF_JSON
  )"

  # npm's Windows command shim does not reliably preserve embedded newlines.
  # Keep CSS and JSON as single arguments so later flags, including outlines and
  # header/footer options, are not silently lost when launching md-to-pdf.
  manual_css="$(printf '%s' "$manual_css" | tr '\n' ' ')"
  pdf_options="$(printf '%s' "$pdf_options" | tr '\n' ' ')"

  launch_options='{"args":["--no-sandbox"]}'

  rm -f -- "$PDF_FILE"

  echo "Generating manual PDF from $MANUAL_FILE"
  echo "Output PDF: $OUTPUT_FILE"

  # md-to-pdf 5.2.4 declares Puppeteer as an open-ended >=8 dependency. Letting
  # npx resolve that range implicitly makes an otherwise pinned release step
  # depend on the newest Puppeteer, its Node.js floor, and its Chrome revision.
  # Supplying both exact packages at the npx root keeps npm's satisfying package
  # selection deterministic. Puppeteer 24.31.0 supports Node.js 18 and newer and
  # installs its matching browser in Puppeteer's normal cross-platform cache.
  if ! npx -y \
    --package="$MD_TO_PDF_PACKAGE" \
    --package="$PUPPETEER_PACKAGE" \
    md-to-pdf \
    "$MANUAL_FILE" \
    --config-file scripts/_manual_pdf_config.cjs \
    --basedir "$REPO_ROOT" \
    --css "$manual_css" \
    --pdf-options "$pdf_options" \
    --launch-options "$launch_options"; then
    show_context
    die "md-to-pdf failed"
  fi

  if [[ ! -s "$PDF_FILE" ]]; then
    show_context
    die "missing or empty generated PDF: $PDF_FILE"
  fi

  # Publish the compiled RM only after rendering succeeds. Its Markdown/HTML
  # intermediates stay in temp; existing release asset names remain unchanged.
  if [[ "$PDF_FILE" != "$OUTPUT_FILE" ]]; then
    if ! cp -- "$PDF_FILE" "$OUTPUT_FILE"; then
      show_context
      die "cannot publish generated manual PDF: $OUTPUT_FILE"
    fi
  fi
  echo "Generated manual PDF: $OUTPUT_FILE ($(pdf_size "$OUTPUT_FILE") bytes)"
done
