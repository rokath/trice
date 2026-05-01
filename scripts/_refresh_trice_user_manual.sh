#!/usr/bin/env bash
#
# _refresh_trice_user_manual.sh
#
# Purpose
# -------
# Best-effort maintenance wrapper for docs/TriceUserManual.md.
#
# In format mode this script tries to:
#   1. Regenerate the manual TOC, numbering, and anchors with mdtoc.
#   2. Ask VS Code + yzane.markdown-pdf to export the manual PDF.
#
# The PDF export step is intentionally non-fatal:
#   - If VS Code is not installed, the required extension is missing,
#     or GUI automation is not available, the script prints an informational
#     message and continues.
#   - The repository-owned mdtoc step remains the authoritative formatting
#     step and still runs in both format and check mode.
#
# The script currently performs command-palette automation only on macOS,
# because that is the environment used in this repository and because the
# stock `code` CLI does not expose extension commands directly.
#
# Important workflow detail
# -------------------------
# The repository does not rely on editor extensions for TOC generation anymore.
# mdtoc is the authoritative formatter for the checked-in manual and owns:
#   - the TOC block
#   - heading numbering
#   - generated fragment anchors
#
# The VS Code extension step is kept only for local PDF export convenience.
#
# The local PDF generated here is only a developer-facing comparison artifact.
# It may differ from the release PDF toolchain and is intentionally allowed to
# stay local and git-ignored.
#
# Usage
# -----
#   ./scripts/_refresh_trice_user_manual.sh
#   ./scripts/_refresh_trice_user_manual.sh check
#   ./scripts/_refresh_trice_user_manual.sh format
#   ./scripts/_refresh_trice_user_manual.sh format --verbose
#   ./scripts/_refresh_trice_user_manual.sh format --pdf-wait-seconds 240
#   ./scripts/_refresh_trice_user_manual.sh format docs/TriceUserManual.md

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

MODE="format"
VERBOSE=0
MANUAL_FILE="docs/TriceUserManual.md"
PDF_WAIT_TIMEOUT_SECONDS="${TRICE_USER_MANUAL_PDF_WAIT_SECONDS:-300}"
PDF_EXPORT_TRIGGERED=0

log_info() {
  printf '%s\n' "$*"
}

log_verbose() {
  if [[ "$VERBOSE" -eq 1 ]]; then
    printf '%s\n' "$*"
  fi
}

log_warn() {
  printf 'WARN: %s\n' "$*" >&2
}

usage() {
  cat <<'EOF'
Usage:
  ./scripts/_refresh_trice_user_manual.sh
  ./scripts/_refresh_trice_user_manual.sh check
  ./scripts/_refresh_trice_user_manual.sh format
  ./scripts/_refresh_trice_user_manual.sh format --verbose
  ./scripts/_refresh_trice_user_manual.sh format --pdf-wait-seconds 240
  ./scripts/_refresh_trice_user_manual.sh format docs/TriceUserManual.md

Environment:
  TRICE_USER_MANUAL_PDF_WAIT_SECONDS
    Overrides the default timeout while waiting for the generated PDF file to
    appear and settle after the VS Code export command was triggered.
EOF
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
    --pdf-wait-seconds)
      shift
      if [[ "$#" -eq 0 || ! "$1" =~ ^[0-9]+$ ]]; then
        log_warn "Option --pdf-wait-seconds expects a positive integer."
        exit 1
      fi
      PDF_WAIT_TIMEOUT_SECONDS="$1"
      ;;
    -h | --help)
      usage
      exit 0
      ;;
    *)
      MANUAL_FILE="$1"
      ;;
    esac
    shift
  done
}

require_manual_file() {
  if [[ ! -f "$MANUAL_FILE" ]]; then
    log_warn "Manual file not found: $MANUAL_FILE"
    exit 1
  fi
}

have_vscode_extension() {
  local extension_id="$1"

  if ! command -v code >/dev/null 2>&1; then
    return 1
  fi

  code --list-extensions 2>/dev/null | grep -Fxq "$extension_id"
}

require_mdtoc() {
  if ! command -v mdtoc >/dev/null 2>&1; then
    log_warn "Required formatter 'mdtoc' is not installed or not in PATH."
    exit 1
  fi
}

can_automate_vscode() {
  [[ "$OSTYPE" == darwin* ]] && command -v osascript >/dev/null 2>&1
}

run_vscode_palette_command() {
  local file_path="$1"
  local extension_id="$2"
  local command_title="$3"
  local description="$4"

  PDF_EXPORT_TRIGGERED=0

  if ! command -v code >/dev/null 2>&1; then
    log_warn "Skipping ${description}: VS Code CLI 'code' is not installed."
    return 0
  fi

  if ! have_vscode_extension "$extension_id"; then
    log_warn "Skipping ${description}: VS Code extension '$extension_id' is not installed."
    return 0
  fi

  if ! can_automate_vscode; then
    log_warn "Skipping ${description}: GUI automation is currently only available on macOS with osascript."
    return 0
  fi

  log_verbose "Opening $file_path in VS Code for ${description}..."
  code -r "$file_path" >/dev/null 2>&1 || {
    log_warn "Skipping ${description}: failed to open '$file_path' in VS Code."
    return 0
  }

  if ! osascript - "$command_title" "$description" <<'APPLESCRIPT' >/dev/null 2>&1; then
on run argv
	set commandTitle to item 1 of argv

	tell application "Visual Studio Code" to activate
	delay 1.0

	tell application "System Events"
		keystroke "p" using {command down, shift down}
		delay 0.5
		keystroke commandTitle
		delay 0.5
		key code 36
		delay 2.0
		keystroke "s" using {command down}
	end tell
end run
APPLESCRIPT
    log_warn "Skipping ${description}: VS Code command palette automation failed. Check macOS Accessibility permissions for Terminal/VS Code."
    return 0
  fi

  log_verbose "Triggered '${command_title}' for '$file_path'."
  PDF_EXPORT_TRIGGERED=1
  return 0
}

get_file_mtime() {
  local file_path="$1"

  if [[ ! -e "$file_path" ]]; then
    return 1
  fi

  if stat -f '%m' "$file_path" >/dev/null 2>&1; then
    stat -f '%m' "$file_path"
    return 0
  fi

  stat -c '%Y' "$file_path"
}

wait_for_file_update() {
  local file_path="$1"
  local previous_mtime="$2"
  local previous_size="$3"
  local timeout_seconds="${4:-60}"
  local started_at
  local current_mtime=""
  local current_size=""
  local seen_update=0
  local stable_rounds=0

  started_at="$(date +%s)"

  while (("$(date +%s)" - started_at < timeout_seconds)); do
    if [[ -f "$file_path" ]]; then
      current_mtime="$(get_file_mtime "$file_path" || true)"
      current_size="$(wc -c <"$file_path" | tr -d '[:space:]')"

      if [[ -n "$current_mtime" ]] && [[ "$current_mtime" != "$previous_mtime" || "$current_size" != "$previous_size" ]]; then
        if ((seen_update == 0)); then
          log_verbose "Detected PDF update for '$file_path'. Waiting for file size to settle..."
        fi
        seen_update=1
        stable_rounds=0
        previous_mtime="$current_mtime"
        previous_size="$current_size"
      elif ((seen_update == 1)); then
        stable_rounds=$((stable_rounds + 1))
        if ((stable_rounds >= 3)); then
          log_verbose "PDF export finished: '$file_path'"
          return 0
        fi
      fi
    fi

    sleep 1
  done

  log_warn "Timed out after ${timeout_seconds}s waiting for PDF export to finish: $file_path"
  return 0
}

update_manual_toc() {
  require_mdtoc

  if [[ "$MODE" == "check" ]]; then
    log_verbose "Checking manual TOC, numbering, and anchors with mdtoc..."
    mdtoc check --file "$MANUAL_FILE"
    return
  fi

  log_verbose "Regenerating manual TOC, numbering, and anchors with mdtoc..."
  mdtoc regen --file "$MANUAL_FILE"
}

export_manual_pdf() {
  local pdf_file="${MANUAL_FILE%.md}.pdf"
  local previous_mtime=""
  local previous_size=""

  if [[ -f "$pdf_file" ]]; then
    previous_mtime="$(get_file_mtime "$pdf_file" || true)"
    previous_size="$(wc -c <"$pdf_file" | tr -d '[:space:]')"
  fi

  run_vscode_palette_command \
    "$MANUAL_FILE" \
    "yzane.markdown-pdf" \
    "Markdown PDF: Export (pdf)" \
    "manual PDF export"

  if [[ "$PDF_EXPORT_TRIGGERED" -eq 1 ]]; then
    log_verbose "Waiting up to ${PDF_WAIT_TIMEOUT_SECONDS}s for '$pdf_file' to be regenerated..."
    wait_for_file_update "$pdf_file" "$previous_mtime" "$previous_size" "$PDF_WAIT_TIMEOUT_SECONDS"
  fi
}

main() {
  parse_args "$@"
  require_manual_file

  update_manual_toc

  if [[ "$MODE" == "format" ]]; then
    export_manual_pdf
  fi
}

main "$@"
