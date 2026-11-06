#!/usr/bin/env bash
# Generate both EPUB 3 release assets from the same verified inputs as the PDFs.
# Install the manifest's Pandoc version before invocation; CI provisions it.
set -euo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
source scripts/test-tools.env

# Validate the converter before rebuilding inputs or replacing existing books.
if ! command -v pandoc >/dev/null 2>&1; then
  echo "ERROR: pandoc $TRICE_TOOL_PANDOC_VERSION is required for EPUB generation; install it from https://pandoc.org/installing.html" >&2
  exit 1
fi
if ! pandoc_version="$(pandoc --version | sed -n '1s/^pandoc //p' | tr -d '\r')"; then
  echo "ERROR: cannot read the Pandoc version; expected $TRICE_TOOL_PANDOC_VERSION" >&2
  exit 1
fi
if [ "$pandoc_version" != "$TRICE_TOOL_PANDOC_VERSION" ]; then
  echo "ERROR: expected pandoc $TRICE_TOOL_PANDOC_VERSION, found $pandoc_version" >&2
  exit 1
fi

# Use the selected release tag in CI. Local builds retain Git's distance and
# dirty marker so an unpublished checkout cannot masquerade as a tagged release.
source_version="${GORELEASER_CURRENT_TAG:-$(git describe --tags --always --dirty)}"
source_commit="$(git rev-parse --short=12 HEAD)"
source_date="$(git log -1 --format=%cs)"
generated_at="$(date -u '+%Y-%m-%dT%H:%M:%SZ')"

# The preparation mode assembles the RM and rewrites UM links, without PDFs.
bash scripts/_320_generate_trice_user_manual_pdf.sh prepare
css="$PWD/scripts/_manual_epub.css"
symbol_filter="$PWD/scripts/_manual_epub.lua"
for manual in TriceUserManual TriceReferenceManual; do
  title="Trice User Manual"
  input_dir=temp/manual-pdf/chapters
  if [ "$manual" = TriceReferenceManual ]; then
    title="Trice Reference Manual"
    input_dir=temp/manual-pdf
  fi
  # Run beside each HTML input so Pandoc embeds its local image resources.
  # Keep incomplete output in staging; publish only after both books validate.
  (
    cd "$input_dir"
    pandoc "$manual.html" --from=html --to=epub3 --standalone --fail-if-warnings \
      --toc --toc-depth=3 --split-level=2 --css="$css" --lua-filter="$symbol_filter" \
      --metadata="title:$title ($source_version)" --metadata=lang:en \
      --metadata="subtitle:Source commit: $source_commit ($source_date). Generated: $generated_at" \
      --metadata="date:${generated_at%%T*}" \
      --metadata=author:"Trice contributors" \
      --output="$manual.epub"
    if [ ! -s "$manual.epub" ]; then
      echo "ERROR: missing or empty EPUB: $input_dir/$manual.epub (pandoc $pandoc_version)" >&2
      exit 1
    fi
    # Read every generated chapter back, rejecting unreadable EPUB containers.
    pandoc "$manual.epub" --from=epub --to=native --output="$manual.roundtrip.txt"
  )
done

# A failed conversion leaves previously published local assets untouched.
cp temp/manual-pdf/chapters/TriceUserManual.epub docs/TriceUserManual.epub
cp temp/manual-pdf/TriceReferenceManual.epub docs/TriceReferenceManual.epub
echo 'Generated EPUB 3: docs/TriceUserManual.epub and docs/TriceReferenceManual.epub'
