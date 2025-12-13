#!/usr/bin/env bash
set -euo pipefail

# ==============================================================================
# format_dumeng_toc.sh.sh
#
# Purpose
# -------
# Cross-platform (macOS/Linux, Windows via Git Bash) formatter for a Markdown file
# (default: docs/TriceUserManual.md).
#
# It performs two independent operations:
#
#   (1) TOC formatting (ONLY within the dumeng TOC markers):
#         <!-- vscode-markdown-toc -->
#         ...
#         <!-- /vscode-markdown-toc -->
#       Removes any leading indentation + bullet marker ("*" or "-") from each TOC line.
#       This keeps your numbering exactly as written (e.g. "1.", "4.1.", "6.5.1.").
#
#   (2) Anchor modernization (ENTIRE DOCUMENT):
#       Converts legacy anchors inside <a ...> opening tags:
#         <a name="foo">  -> <a id="foo">
#         <a name='foo'>  -> <a id='foo'>
#       This is done conservatively:
#       - Only the attribute "name=" is replaced when it is an actual attribute token
#         preceded by whitespace, i.e. " name=".
#       - It will NOT touch attributes like "data-name=" (because that has "-name=").
#
# Usage
# -----
#   tools/format_dumeng_toc.sh.sh
#   tools/format_dumeng_toc.sh.sh path/to/file.md
# ==============================================================================

FILE="${1:-docs/TriceUserManual.md}"

if [[ ! -f "$FILE" ]]; then
  echo "ERROR: File not found: $FILE" >&2
  exit 1
fi

DIR="$(cd "$(dirname "$FILE")" && pwd)"
BASENAME="$(basename "$FILE")"
TMP="${DIR}/.${BASENAME}.tmp"

awk '
BEGIN { in_toc = 0 }

# ------------------------------------------------------------------------------
# Replace " name=" with " id=" ONLY inside <a ...> opening tags.
# No GNU extensions, no \b word-boundaries (POSIX awk compatible).
# ------------------------------------------------------------------------------
function fix_anchor_tags(line,    out, pos, aRel, aPos, gtRel, gtPos, tag) {
  out = ""
  pos = 1

  # Iterate through all "<a ...>" occurrences on the line (if multiple).
  while ((aRel = index(substr(line, pos), "<a")) > 0) {
    aPos = aRel + pos - 1

    # Copy text before "<a"
    out = out substr(line, pos, aPos - pos)

    # Find end of the opening tag ">"
    gtRel = index(substr(line, aPos), ">")
    if (gtRel == 0) {
      # No closing ">" found; append rest unchanged
      out = out substr(line, aPos)
      return out
    }
    gtPos = gtRel + aPos - 1

    # Extract the opening tag
    tag = substr(line, aPos, gtPos - aPos + 1)

    # Replace attribute token " name=" (with optional spaces around "=").
    # We do this conservatively: the attribute must be preceded by whitespace.
    #
    # Handle cases like:
    #   <a name="top">
    #   <a  name = "top" class="x">
    #   <a href="..." name="top">
    #
    # Approach:
    # - First, normalize cases where name is the *first* attribute after <a:
    #     "<a name" -> "<a id"
    sub(/^<a[[:space:]]+name[[:space:]]*=/, "<a id=", tag)

    # - Then replace any other occurrences of whitespace + name= inside the tag:
    #     " name=" -> " id="
    # This will not match "data-name=" because there is no leading whitespace before "name".
    gsub(/[[:space:]]+name[[:space:]]*=/, " id=", tag)

    # Append fixed tag
    out = out tag

    # Continue after the end of this tag
    pos = gtPos + 1
  }

  # Append remainder of the line
  out = out substr(line, pos)
  return out
}

{
  line = $0

  # (2) Anchor modernization for the whole document
  # Fast pre-check: only do the heavier parsing if "name=" is present at all.
  if (index(line, "name=") > 0 && index(line, "<a") > 0) {
    line = fix_anchor_tags(line)
  }

  # TOC block detection
  if (line ~ /<!-- vscode-markdown-toc -->/) {
    in_toc = 1
    print line
    next
  }
  if (line ~ /<!-- \/vscode-markdown-toc -->/) {
    in_toc = 0
    print line
    next
  }

  # (1) TOC formatting only inside TOC block:
  # Remove indentation + bullet "* " or "- "
  if (in_toc) {
    sub(/^[[:space:]]*[*-][[:space:]]+/, "", line)
  }

  print line
}
' "$FILE" > "$TMP"

mv "$TMP" "$FILE"
echo "Updated: $FILE"
