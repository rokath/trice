#!/usr/bin/env bash


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

set -euo pipefail

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

# --- Replace <a name="..."> -> <a id="..."> inside <a ...> opening tags (whole doc)
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
    if (gtRel == 0) { out = out substr(line, aPos); return out }
    gtPos = gtRel + aPos - 1

    tag = substr(line, aPos, gtPos - aPos + 1)

    sub(/^<a[[:space:]]+name[[:space:]]*=/, "<a id=", tag)
    gsub(/[[:space:]]+name[[:space:]]*=/, " id=", tag)

    # Append fixed tag
    out = out tag

    # Continue after the end of this tag
    pos = gtPos + 1
  }
  out = out substr(line, pos)
  return out
}

# --- TOC line transform (only inside TOC block):
# "* 4.1. [Title](#x)" -> "* [4.1. Title](#x)"
function move_number_into_link(line,    leadLen, lead, rest, cut, num, afterNum, br) {
  # Must be a list-item bullet line (keep indentation + bullet exactly as-is)
  if (match(line, /^[[:space:]]*[*-][[:space:]]+/) == 0) return line

  leadLen = RLENGTH
  lead = substr(line, 1, leadLen)
  rest = substr(line, leadLen + 1)

  # Find first ". " which separates number token and the link
  cut = index(rest, ". ")
  if (cut == 0) return line

  num = substr(rest, 1, cut - 1)          # "1" or "4.1" or "6.5.1"
  afterNum = substr(rest, cut + 2)        # should start with "[Title](#...)"

  # Locate the first "[" in afterNum
  br = index(afterNum, "[")
  if (br == 0) return line

  # Remove any leading spaces before "[" (defensive)
  # (dumeng normally has none, but harmless)
  while (substr(afterNum, 1, 1) ~ /[[:space:]]/) afterNum = substr(afterNum, 2)

  # Now inject "num. " right after the first "["
  # afterNum is like: "[Title](#x)"
  # We want: "[num. Title](#x)"
  return lead "[" num ". " substr(afterNum, 2)
}

{
  line = $0

  # Whole document: normalize <a name=...> to <a id=...>
  if (index(line, "name=") > 0 && index(line, "<a") > 0) {
    line = fix_anchor_tags(line)
  }

  # TOC block detection
  if (line ~ /<!-- vscode-markdown-toc -->/) { in_toc = 1; print line; next }
  if (line ~ /<!-- \/vscode-markdown-toc -->/) { in_toc = 0; print line; next }

  # Only in TOC: transform numbering into link text
  if (in_toc) {
    line = move_number_into_link(line)
  }

  print line
}
' "$FILE" > "$TMP"

mv "$TMP" "$FILE"
echo "Updated: $FILE"
