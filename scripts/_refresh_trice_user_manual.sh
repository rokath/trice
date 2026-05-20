#!/usr/bin/env bash
#
# _refresh_trice_user_manual.sh
#
# Purpose
# -------
# Maintenance wrapper for docs/TriceUserManual.md.
#
# In format mode this script:
#   1. Regenerates the manual TOC, numbering, and anchors with mdtoc.
#   2. Generates a local developer PDF from the command line by default.
#
# The Markdown source formatting remains owned by mdtoc. The PDF generation
# path is intentionally independent from VS Code so it also works in headless
# environments and from Git Bash on Windows, provided Node.js/npm are available.
#
# Default PDF engine
# ------------------
# The default engine is "cli" and uses md-to-pdf through npx:
#
#   npx -y md-to-pdf@5.2.4 ...
#
# This matches the release-PDF toolchain instead of depending on the local
# VS Code extension 'yzane.markdown-pdf'. The old VS Code automation is still
# available explicitly with '--pdf-engine vscode' for local comparison only.
#
# The generated local PDF is a developer-facing comparison artifact and is
# expected to stay git-ignored:
#
#   docs/TriceUserManual.pdf
#
# Usage
# -----
#   ./scripts/_refresh_trice_user_manual.sh
#   ./scripts/_refresh_trice_user_manual.sh check
#   ./scripts/_refresh_trice_user_manual.sh format
#   ./scripts/_refresh_trice_user_manual.sh format --verbose
#   ./scripts/_refresh_trice_user_manual.sh format --pdf-engine cli
#   ./scripts/_refresh_trice_user_manual.sh format --pdf-engine vscode
#   ./scripts/_refresh_trice_user_manual.sh format --no-pdf
#   ./scripts/_refresh_trice_user_manual.sh format --require-pdf
#   ./scripts/_refresh_trice_user_manual.sh format --pdf-output docs/TriceUserManual.pdf
#   ./scripts/_refresh_trice_user_manual.sh format docs/TriceUserManual.md

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

MODE="format"
VERBOSE=0
MANUAL_FILE="docs/TriceUserManual.md"
PDF_ENGINE="${TRICE_USER_MANUAL_PDF_ENGINE:-cli}"
PDF_OUTPUT_FILE="${TRICE_USER_MANUAL_PDF_OUTPUT:-}"
PDF_REQUIRED="${TRICE_USER_MANUAL_REQUIRE_PDF:-0}"
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

log_error() {
  printf 'ERROR: %s\n' "$*" >&2
}

usage() {
  cat <<'EOF_USAGE'
Usage:
  ./scripts/_refresh_trice_user_manual.sh
  ./scripts/_refresh_trice_user_manual.sh check
  ./scripts/_refresh_trice_user_manual.sh format
  ./scripts/_refresh_trice_user_manual.sh format --verbose
  ./scripts/_refresh_trice_user_manual.sh format --pdf-engine cli
  ./scripts/_refresh_trice_user_manual.sh format --pdf-engine vscode
  ./scripts/_refresh_trice_user_manual.sh format --no-pdf
  ./scripts/_refresh_trice_user_manual.sh format --require-pdf
  ./scripts/_refresh_trice_user_manual.sh format --pdf-output docs/TriceUserManual.pdf
  ./scripts/_refresh_trice_user_manual.sh format docs/TriceUserManual.md

Options:
  --pdf-engine cli|vscode|none
    Selects the PDF generation path. Default: cli.

  --no-pdf
    Shortcut for --pdf-engine none.

  --require-pdf
    Make PDF generation failures fatal. By default, local refresh keeps the
    historical best-effort behavior and reports PDF failures as warnings.

  --pdf-output <path>
    Output path for the generated local PDF. Default: <manual>.pdf.

  --pdf-wait-seconds <seconds>
    Timeout used only by the legacy VS Code automation path.

Environment:
  TRICE_USER_MANUAL_PDF_ENGINE=cli|vscode|none
  TRICE_USER_MANUAL_PDF_OUTPUT=<path>
  TRICE_USER_MANUAL_REQUIRE_PDF=0|1
  TRICE_USER_MANUAL_PDF_WAIT_SECONDS=<seconds>
EOF_USAGE
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
    --pdf-engine)
      shift
      if [[ "$#" -eq 0 ]]; then
        log_error "Option --pdf-engine expects one of: cli, vscode, none."
        exit 1
      fi
      PDF_ENGINE="$1"
      ;;
    --no-pdf)
      PDF_ENGINE="none"
      ;;
    --require-pdf)
      PDF_REQUIRED=1
      ;;
    --pdf-output)
      shift
      if [[ "$#" -eq 0 ]]; then
        log_error "Option --pdf-output expects a path."
        exit 1
      fi
      PDF_OUTPUT_FILE="$1"
      ;;
    --pdf-wait-seconds)
      shift
      if [[ "$#" -eq 0 || ! "$1" =~ ^[0-9]+$ ]]; then
        log_error "Option --pdf-wait-seconds expects a positive integer."
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

  case "$PDF_ENGINE" in
  cli | vscode | none)
    ;;
  *)
    log_error "Unsupported PDF engine: $PDF_ENGINE"
    log_error "Expected one of: cli, vscode, none."
    exit 1
    ;;
  esac

  if [[ -z "$PDF_OUTPUT_FILE" ]]; then
    PDF_OUTPUT_FILE="${MANUAL_FILE%.md}.pdf"
  fi
}

require_manual_file() {
  if [[ ! -f "$MANUAL_FILE" ]]; then
    log_error "Manual file not found: $MANUAL_FILE"
    exit 1
  fi
}

require_mdtoc() {
  if ! command -v mdtoc >/dev/null 2>&1; then
    log_error "Required formatter 'mdtoc' is not installed or not in PATH."
    exit 1
  fi
}

handle_pdf_failure() {
  local message="$1"

  if [[ "$PDF_REQUIRED" == "1" ]]; then
    log_error "$message"
    exit 1
  fi

  log_warn "$message"
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

dump_pdf_context() {
  log_info "PDF context:"
  log_info "  os: ${OSTYPE:-unknown}"
  log_info "  pwd: $(pwd)"
  log_info "  manual: $MANUAL_FILE"
  log_info "  output: $PDF_OUTPUT_FILE"
  log_info "  engine: $PDF_ENGINE"

  if command -v node >/dev/null 2>&1; then
    log_info "  node: $(node --version 2>/dev/null || true)"
  else
    log_info "  node: not found"
  fi

  if command -v npm >/dev/null 2>&1; then
    log_info "  npm: $(npm --version 2>/dev/null || true)"
  else
    log_info "  npm: not found"
  fi

  if command -v npx >/dev/null 2>&1; then
    log_info "  npx: $(npx --version 2>/dev/null || true)"
  else
    log_info "  npx: not found"
  fi

  log_info "  relevant generated files:"
  find docs temp -maxdepth 4 \( -name 'TriceUserManual.pdf' -o -name 'TriceUserManual.md' -o -name 'md-to-pdf.config.js' \) -print 2>/dev/null | sort || true
}

prepare_cli_pdf_workspace() {
  local temp_root="$1"
  local temp_doc_dir="$2"
  local temp_md="$3"
  local ref_source="docs/ref"
  local ref_target="$temp_doc_dir/ref"

  rm -rf -- "$temp_root"
  mkdir -p -- "$temp_doc_dir"
  cp -f -- "$MANUAL_FILE" "$temp_md"

  if [[ -d "$ref_source" ]]; then
    mkdir -p -- "$ref_target"
    cp -R -- "$ref_source/." "$ref_target/"
  else
    log_warn "Reference directory not found: $ref_source"
  fi

  # The repository manual keeps the TOC collapsible for HTML reading. For a
  # generated PDF the TOC should be visible without user interaction.
  perl -0pi -e 's#<details markdown="1">#<details open markdown="1">#g' "$temp_md"
}

write_cli_pdf_config() {
  local config_file="$1"
  local generated_at

  generated_at="$(date '+%Y-%m-%d %H:%M %Z')"

  cat >"$config_file" <<EOF_CONFIG
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
EOF_CONFIG
}

export_manual_pdf_cli() {
  local temp_root="temp/manual-pdf"
  local temp_doc_dir="$temp_root/docs"
  local temp_md="$temp_doc_dir/TriceUserManual.md"
  local temp_doc_pdf="$temp_doc_dir/TriceUserManual.pdf"
  local temp_config_js="$temp_root/md-to-pdf.config.js"
  local output_dir
  local -a cmd

  if ! command -v npx >/dev/null 2>&1; then
    dump_pdf_context
    handle_pdf_failure "Skipping manual PDF export: 'npx' is required for CLI PDF generation. Install Node.js/npm or use --pdf-engine vscode."
    return 0
  fi

  prepare_cli_pdf_workspace "$temp_root" "$temp_doc_dir" "$temp_md"
  write_cli_pdf_config "$temp_config_js"

  log_info "Generating manual PDF with md-to-pdf CLI..."
  log_info "  input:  $temp_md"
  log_info "  output: $PDF_OUTPUT_FILE"

  cmd=(
    npx -y md-to-pdf@5.2.4
    "$temp_md"
    --basedir "$temp_root"
    --config-file "$temp_config_js"
    --launch-options '{"args":["--no-sandbox"]}'
  )

  log_verbose "Command: ${cmd[*]}"

  if ! "${cmd[@]}"; then
    dump_pdf_context
    handle_pdf_failure "Manual PDF export failed while running md-to-pdf."
    return 0
  fi

  if [[ ! -s "$temp_doc_pdf" ]]; then
    dump_pdf_context
    handle_pdf_failure "Manual PDF export did not create expected file: $temp_doc_pdf"
    return 0
  fi

  output_dir="$(dirname -- "$PDF_OUTPUT_FILE")"
  mkdir -p -- "$output_dir"
  cp -f -- "$temp_doc_pdf" "$PDF_OUTPUT_FILE"

  if [[ ! -s "$PDF_OUTPUT_FILE" ]]; then
    dump_pdf_context
    handle_pdf_failure "Manual PDF export did not create expected output file: $PDF_OUTPUT_FILE"
    return 0
  fi

  log_info "Generated manual PDF: $PDF_OUTPUT_FILE ($(wc -c <"$PDF_OUTPUT_FILE") bytes)"
}

have_vscode_extension() {
  local extension_id="$1"

  if ! command -v code >/dev/null 2>&1; then
    return 1
  fi

  code --list-extensions 2>/dev/null | grep -Fxq "$extension_id"
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
    handle_pdf_failure "Skipping ${description}: VS Code CLI 'code' is not installed."
    return 0
  fi

  if ! have_vscode_extension "$extension_id"; then
    handle_pdf_failure "Skipping ${description}: VS Code extension '$extension_id' is not installed."
    return 0
  fi

  if ! can_automate_vscode; then
    handle_pdf_failure "Skipping ${description}: GUI automation is currently only available on macOS with osascript."
    return 0
  fi

  log_verbose "Opening $file_path in VS Code for ${description}..."
  code -r "$file_path" >/dev/null 2>&1 || {
    handle_pdf_failure "Skipping ${description}: failed to open '$file_path' in VS Code."
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
    handle_pdf_failure "Skipping ${description}: VS Code command palette automation failed. Check macOS Accessibility permissions for Terminal/VS Code."
    return 0
  fi

  log_verbose "Triggered '${command_title}' for '$file_path'."
  PDF_EXPORT_TRIGGERED=1
  return 0
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

  handle_pdf_failure "Timed out after ${timeout_seconds}s waiting for PDF export to finish: $file_path"
  return 0
}

export_manual_pdf_vscode() {
  local previous_mtime=""
  local previous_size=""

  if [[ -f "$PDF_OUTPUT_FILE" ]]; then
    previous_mtime="$(get_file_mtime "$PDF_OUTPUT_FILE" || true)"
    previous_size="$(wc -c <"$PDF_OUTPUT_FILE" | tr -d '[:space:]')"
  fi

  run_vscode_palette_command \
    "$MANUAL_FILE" \
    "yzane.markdown-pdf" \
    "Markdown PDF: Export (pdf)" \
    "manual PDF export"

  if [[ "$PDF_EXPORT_TRIGGERED" -eq 1 ]]; then
    log_verbose "Waiting up to ${PDF_WAIT_TIMEOUT_SECONDS}s for '$PDF_OUTPUT_FILE' to be regenerated..."
    wait_for_file_update "$PDF_OUTPUT_FILE" "$previous_mtime" "$previous_size" "$PDF_WAIT_TIMEOUT_SECONDS"
  fi
}

update_manual_toc() {
  require_mdtoc

  if [[ "$MODE" == "check" ]]; then
    log_verbose "Checking manual TOC, numbering, and anchors with mdtoc..."
    mdtoc check --file "$MANUAL_FILE"
    return
  fi

  log_verbose "Regenerating manual TOC, numbering, and anchors with mdtoc..."
  mdtoc generate --file "$MANUAL_FILE"
}

export_manual_pdf() {
  case "$PDF_ENGINE" in
  cli)
    export_manual_pdf_cli
    ;;
  vscode)
    export_manual_pdf_vscode
    ;;
  none)
    log_info "Skipping manual PDF export because PDF engine is 'none'."
    ;;
  esac
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
