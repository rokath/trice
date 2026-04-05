#!/bin/bash

# Recursively remove stray macOS .DS_Store files.
# Usage:
#   ./scripts/_clean-dsstore.sh [TARGET_DIR] [--dry-run] [--silent]
#
# Options:
#   TARGET_DIR  Optional scan root. Defaults to the current directory.
#   --dry-run   Print matches without deleting them.
#   --silent    Suppress normal progress output. Errors still go to stderr.
#
# Notes:
# - Excludes selected macOS system folders from the search.
# - Quits and restarts Finder only if it was running before the cleanup.

# === Default settings ===
TARGET_DIR="."
DRY_RUN=false
SILENT=false

log_info() {
  if [ "$SILENT" = false ]; then
    printf '%s\n' "$1"
  fi
}

log_error() {
  printf '%s\n' "$1" >&2
}

# === Parse arguments ===
for arg in "$@"; do
  case "$arg" in
  --dry-run) DRY_RUN=true ;;
  --silent) SILENT=true ;;
  --*)
    log_error "Unknown option: $arg"
    log_error "Usage: ./scripts/_clean-dsstore.sh [TARGET_DIR] [--dry-run] [--silent]"
    exit 1
    ;;
  *)
    TARGET_DIR="$arg"
    ;;
  esac
done

COUNT=0
OS_TYPE="$(uname)"

if [[ $OS_TYPE == "Darwin" ]]; then
  # === Check if Finder is running ===
  FINDER_WAS_RUNNING=false
  if pgrep -xq "Finder"; then
    FINDER_WAS_RUNNING=true
    log_info "🛑 Quitting Finder to prevent interference..."
    osascript -e 'tell application "Finder" to quit'
  fi
fi

if [ ! -d "$TARGET_DIR" ]; then
  log_error "Target directory does not exist: $TARGET_DIR"
  exit 1
fi

log_info "🔍 Scanning for .DS_Store files..."

# === Main find/delete loop ===
while read -r file; do
  [ -z "$file" ] && continue
  log_info "🗑️  Found: $file"
  if [ "$DRY_RUN" = false ]; then
    if rm -f "$file"; then
      ((COUNT++))
      log_info "✔️  Deleted"
    else
      log_error "Failed to delete: $file"
    fi
  else
    ((COUNT++))
  fi
done < <(find "$TARGET_DIR" \
  -path "*/.Spotlight-V100" -prune -o \
  -path "*/.DocumentRevisions-V100" -prune -o \
  -path "*/.TemporaryItems" -prune -o \
  -name '.DS_Store' -type f -print)

# === Summary ===
log_info "✅ Done. Processed $COUNT file(s)."

if [[ $OS_TYPE == "Darwin" ]]; then
  # === Restart Finder if it was previously running ===
  if [ "$FINDER_WAS_RUNNING" = true ]; then
    log_info "🔄 Restarting Finder..."
    open -a Finder
  else
    log_info "📎 Finder was not running before; not restarted."
  fi
fi
