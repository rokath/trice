#!/usr/bin/env bash
#
# _310_refresh_trice_user_manual.sh
#
# Purpose
# -------
# Maintenance wrapper for the User Manual and Reference Manual chapters in docs/.
#
# This script only checks or regenerates the manual TOC, numbering, and anchors
# with mdtoc. PDF generation is intentionally handled by the separate script
# scripts/_320_generate_trice_user_manual_pdf.sh.
#
# Usage
# -----
#   ./scripts/_310_refresh_trice_user_manual.sh
#   ./scripts/_310_refresh_trice_user_manual.sh check
#   ./scripts/_310_refresh_trice_user_manual.sh format
#   ./scripts/_310_refresh_trice_user_manual.sh check --verbose
#   ./scripts/_310_refresh_trice_user_manual.sh format --verbose

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"
# shellcheck source=./_105_setup_test_tools.sh
source "$SCRIPT_DIR/_105_setup_test_tools.sh"

MODE="format"
VERBOSE=0
MDTOC_REQUIRED_VERSION="${TRICE_TOOL_MDTOC_VERSION:-1.0.3}"
# Keep the User Manual first; managed RM chapters are discovered before any writes.
MANUAL_FILES=("docs/TriceUserManual.md")

usage() {
  sed -n '2,22p' "$0" | sed 's/^# \{0,1\}//'
}

log_verbose() {
  if [[ "$VERBOSE" -eq 1 ]]; then
    printf '%s\n' "$*"
  fi
}

die() {
  printf 'ERROR: %s\n' "$*" >&2
  exit 1
}

parse_args() {
  while [[ "$#" -gt 0 ]]; do
    case "$1" in
      format | check)
        MODE="$1"
        ;;
      --verbose)
        VERBOSE=1
        ;;
      -h | --help)
        usage
        exit 0
        ;;
      *)
        die "unknown argument: $1"
        ;;
    esac
    shift
  done
}

require_manual_file() {
  if [[ ! -s "$1" ]]; then
    die "manual file not found or empty: $1"
  fi
}

# Short chapters without subsections need no empty TOC. Ignore example headings
# inside fenced code and HTML comments when deciding whether a TOC is missing.
chapter_has_sections() {
  awk '
    {
      line = $0
      if (fence != "") {
        if (line ~ "^[[:space:]]*" fence "+[[:space:]]*$") fence = ""
        next
      }
      if (!comment && line ~ /^[[:space:]]*```/) { fence = "```"; next }
      if (!comment && line ~ /^[[:space:]]*~~~/) { fence = "~~~"; next }
      visible = ""
      while (length(line)) {
        if (comment) {
          end = index(line, "-->")
          if (!end) { line = ""; break }
          line = substr(line, end + 3); comment = 0
        } else {
          start = index(line, "<!--")
          if (!start) { visible = visible line; break }
          visible = visible substr(line, 1, start - 1)
          line = substr(line, start + 4)
          comment = 1
        }
      }
      if (visible ~ /^##+[[:space:]]/) found = 1
    }
    END { exit !found }
  ' "$1"
}

# Discover the flat chapter sources without introducing another maintained list.
# Navigation files are not chapters, even if they later contain a TOC example.
# Validate README coverage and managed containers before invoking mdtoc so an
# incomplete chapter set cannot silently pass or cause partial formatting.
collect_reference_chapters() {
  local CHAPTER_FILE LINK_LINE LINK_FILE LINKED_FILE FOUND
  local LINK_PATTERN='^[[:space:]]*[-*][[:space:]]+\[[^]]+\]\(([^)]+\.md)\)[[:space:]]*$'
  local LINKED_CHAPTERS=()
  if [[ ! -d docs/rm ]]; then
    die "reference manual directory not found: docs/rm"
  fi
  require_manual_file "docs/rm/README.md"

  # Match the chapter-list form also used by the PDF assembler. Notes that are
  # neither linked nor mdtoc-managed remain outside the formatter's scope.
  while IFS= read -r LINK_LINE || [[ -n "$LINK_LINE" ]]; do
    [[ "$LINK_LINE" =~ $LINK_PATTERN ]] || continue
    LINK_FILE="${BASH_REMATCH[1]}"
    LINK_FILE="${LINK_FILE#./}"
    case "$LINK_FILE" in
      */* | README.md | INDEX.md) die "expected a flat RM chapter link in docs/rm/README.md: $LINK_FILE" ;;
    esac
    CHAPTER_FILE="docs/rm/$LINK_FILE"
    require_manual_file "$CHAPTER_FILE"
    case "$LINK_FILE" in HEADER.md | FOOTER.md) continue ;; esac
    # The default expansion also supports empty arrays under Bash 3.2 with -u.
    for LINKED_FILE in "${LINKED_CHAPTERS[@]:-}"; do
      [[ "$LINKED_FILE" != "$CHAPTER_FILE" ]] || die "duplicate chapter link in docs/rm/README.md: $CHAPTER_FILE"
    done
    if grep -Eq '^<!-- /?mdtoc -->' "$CHAPTER_FILE" || chapter_has_sections "$CHAPTER_FILE"; then
      if ! grep -qx '<!-- mdtoc -->' "$CHAPTER_FILE" || ! grep -qx '<!-- /mdtoc -->' "$CHAPTER_FILE"; then
        die "missing complete mdtoc container in reference chapter: $CHAPTER_FILE; expected <!-- mdtoc --> and <!-- /mdtoc -->"
      fi
    fi
    LINKED_CHAPTERS+=("$CHAPTER_FILE")
  done <docs/rm/README.md

  for CHAPTER_FILE in docs/rm/*.md; do
    # An unmatched glob stays literal; never pass it to mdtoc as a filename.
    [[ -f "$CHAPTER_FILE" ]] || continue
    case "$CHAPTER_FILE" in
      docs/rm/README.md | docs/rm/HEADER.md | docs/rm/FOOTER.md | docs/rm/INDEX.md)
        continue
        ;;
    esac
    require_manual_file "$CHAPTER_FILE"
    if grep -q '^<!-- mdtoc -->' "$CHAPTER_FILE"; then
      FOUND=0
      for LINKED_FILE in "${LINKED_CHAPTERS[@]:-}"; do
        [[ "$LINKED_FILE" != "$CHAPTER_FILE" ]] || FOUND=1
      done
      [[ "$FOUND" -eq 1 ]] || die "mdtoc-managed chapter is not linked from docs/rm/README.md: $CHAPTER_FILE"
      MANUAL_FILES+=("$CHAPTER_FILE")
    fi
  done

  if [[ "${#MANUAL_FILES[@]}" -eq 1 ]]; then
    die "no mdtoc-managed reference chapters found: docs/rm/*.md; expected <!-- mdtoc --> in a chapter, excluding README.md, HEADER.md, FOOTER.md and INDEX.md"
  fi
}

# A Go-installed mdtoc reports "dev" without release linker flags. Accept it
# only when Go metadata identifies the exact tagged module and main package.
mdtoc_has_required_version() {
  local mdtoc_binary
  local mdtoc_build
  local mdtoc_version
  if ! command -v mdtoc >/dev/null 2>&1; then
    MDTOC_VERSION_OUTPUT="not installed"
    return 1
  fi
  mdtoc_binary="$(command -v mdtoc)"
  if ! MDTOC_VERSION_OUTPUT="$(mdtoc --version 2>&1)"; then
    return 1
  fi
  mdtoc_version="$(printf '%s\n' "$MDTOC_VERSION_OUTPUT" | tr -d '\r' | sed -n '1p')"
  case "$mdtoc_version" in
    "mdtoc $MDTOC_REQUIRED_VERSION" | "mdtoc v$MDTOC_REQUIRED_VERSION") return 0 ;;
    "mdtoc dev")
      case "${OSTYPE:-}" in
        msys* | cygwin*)
          if [[ -f "$mdtoc_binary.exe" ]]; then mdtoc_binary="$mdtoc_binary.exe"; fi
          ;;
      esac
      if ! command -v go >/dev/null 2>&1 || ! mdtoc_build="$(go version -m "$mdtoc_binary" 2>&1)"; then
        return 1
      fi
      printf '%s\n' "$mdtoc_build" | tr -d '\r' | awk \
        -v package=github.com/rokath/mdtoc/cmd/mdtoc \
        -v module=github.com/rokath/mdtoc \
        -v version="v$MDTOC_REQUIRED_VERSION" '
          $1 == "path" { package_ok = ($2 == package && NF == 2) }
          $1 == "mod" { module_ok = ($2 == module && $3 == version) }
          $1 == "=>" { replaced = 1 }
          END { exit !(package_ok && module_ok && !replaced) }
        '
      ;;
    *) return 1 ;;
  esac
}

# Check identity before any managed file can be changed. A declined or failed
# local installation is an error rather than an unverified formatting skip.
require_mdtoc() {
  if mdtoc_has_required_version; then
    return 0
  fi
  if ! confirm_tool_install "mdtoc v$MDTOC_REQUIRED_VERSION"; then
    die "mdtoc v$MDTOC_REQUIRED_VERSION is required; detected $MDTOC_VERSION_OUTPUT. Install with: go install github.com/rokath/mdtoc/cmd/mdtoc@v$MDTOC_REQUIRED_VERSION"
  fi
  install_go_test_tool "github.com/rokath/mdtoc/cmd/mdtoc@v$MDTOC_REQUIRED_VERSION" || die "mdtoc v$MDTOC_REQUIRED_VERSION installation failed"
  mdtoc_has_required_version || die "mdtoc v$MDTOC_REQUIRED_VERSION installation did not produce a matching executable; detected $MDTOC_VERSION_OUTPUT"
}

main() {
  parse_args "$@"
  local MANUAL_FILE
  require_manual_file "docs/TriceUserManual.md"
  collect_reference_chapters
  require_mdtoc

  for MANUAL_FILE in "${MANUAL_FILES[@]}"; do
    if [[ "$MODE" == "check" ]]; then
      log_verbose "Checking $MANUAL_FILE TOC, numbering, and anchors with mdtoc..."
      mdtoc check --file "$MANUAL_FILE"
    else
      log_verbose "Regenerating $MANUAL_FILE TOC, numbering, and anchors with mdtoc..."
      mdtoc generate --file "$MANUAL_FILE"
    fi
  done
}

main "$@"
