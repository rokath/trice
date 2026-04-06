#!/usr/bin/env bash
# buildTriceTool.ba – Local build helper for the trice tool
#
# This script performs a local build and injects Git metadata into the
# Go binary using -ldflags, similar to what GoReleaser does for release builds.
#
# Injected metadata includes:
#   - version
#   - commit
#   - branch
#   - build date
#   - gitState  ("clean" or "dirty")
#   - gitStatus (list of modified/untracked files, "|" separated)
#
# If the repository is dirty, the affected file list is stored so the
# application can print it when run with Verbose=true.
#
# This script is intended for developer builds only.
# For releases, GoReleaser injects the metadata instead.
#
# Install behaviour:
# - The script now resolves the final install target before running go install.
# - A custom output target can be specified explicitly for isolated test builds.
# - If a trice binary already exists there, it is no longer replaced silently.
# - By default, an existing binary is backed up before it is overwritten.
# - Use --no-backup to opt out of that default.
# - Use --force to skip the interactive overwrite prompt.
# - Use --silent to suppress normal progress output.
# - Use --verbose for additional diagnostic output.

set -euo pipefail

# Ensure we always operate in the repository root.
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.." || exit 1

##############################################
# Command line handling and logging helpers
##############################################
#
# The build logic below is intentionally kept unchanged as far as the
# actual metadata collection and go install invocation are concerned.
# The added logic only improves the global install behaviour around an
# already existing trice binary.

BACKUP=true
BACKUP_EXPLICIT=false
FORCE=false
SILENT=false
VERBOSE=false
CUSTOM_TARGET_FILE=""

usage() {
  cat <<'EOF'
Usage:
  ./scripts/buildTriceTool.sh [--force] [--backup|--no-backup] [--silent] [--verbose] [--target-file <path>]

Options:
  --force      Overwrite an existing target without interactive confirmation.
  --backup     Explicitly keep the default behaviour and create a backup.
  --no-backup  Do not create a backup before overwriting an existing target.
  --silent     Suppress normal progress messages.
  --verbose    Print additional diagnostic details.
  --target-file
               Write the built trice binary to the given file path instead of
               using go install's GOBIN/GOPATH target resolution.
  --help       Show this help text.

Notes:
  A backup is created by default when an existing trice binary would be replaced.
  The backup file name keeps the original executable name and prefixes it with
  Backup-<timestamp>_ so that trice.exe stays trice.exe on Windows.
EOF
}

log_info() {
  if [ "$SILENT" = false ]; then
    printf '%s\n' "$1"
  fi
}

log_verbose() {
  if [ "$SILENT" = false ] && [ "$VERBOSE" = true ]; then
    printf '%s\n' "$1"
  fi
}

log_error() {
  printf '%s\n' "$1" >&2
}

log_always() {
  printf '%s\n' "$1"
}

# Parse only the global behaviour flags here.
# The script intentionally keeps the functional build parameters fixed and only
# adds switches for install/output behaviour.
while [ "$#" -gt 0 ]; do
  arg="$1"
  case "$arg" in
  --backup)
    BACKUP=true
    BACKUP_EXPLICIT=true
    ;;
  --no-backup)
    BACKUP=false
    BACKUP_EXPLICIT=true
    ;;
  --force) FORCE=true ;;
  --silent) SILENT=true ;;
  --verbose) VERBOSE=true ;;
  --target-file)
    shift
    if [ "$#" -eq 0 ]; then
      log_error "Missing value for --target-file"
      usage >&2
      exit 1
    fi
    CUSTOM_TARGET_FILE="$1"
    ;;
  --help)
    usage
    exit 0
    ;;
  *)
    log_error "Unknown option: $arg"
    usage >&2
    exit 1
    ;;
  esac
  shift
done

##############################################
# Collect Git information
##############################################

origin=$(git remote get-url origin 2>&1 || echo "unknown")

# Current branch (fallback to "unknown" if not in a repo)
branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")

# Short commit hash
commit=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")

# Build timestamp (ISO-8601)
date=$(date -Iseconds)

# Determine Git tree state
git_state="clean"
git_status=""

# Check whether there are local modifications
# git diff --quiet exits non-zero if there are changes
if ! git diff --quiet --ignore-submodules HEAD 2>/dev/null; then
  git_state="dirty"
  # Collect short status lines, e.g. " M file.go"
  # We store them "|" separated so Go can split them easily
  git_status=$(git status --short | tr '\n' '|' | sed 's/|$//')
fi

##############################################
# Determine "version" for local builds
##############################################
# Logic:
#   - If HEAD is exactly on a tag → use the tag as version
#   - Otherwise → version = <branch>[ -dirty ] (similar to GoReleaser)

if git describe --tags --exact-match >/dev/null 2>&1; then
  version=$(git describe --tags --exact-match)
else
  if [[ "$git_state" == "dirty" ]]; then
    version="branch dirty"
  else
    version="branch clean"
  fi
fi

##############################################
# Print build metadata summary
##############################################

log_info "----------------------------------------"
log_info "Building trice with embedded Git metadata:"
log_info "  origin:     $origin"
log_info "  branch:     $branch"
log_info "  version:    $version"
log_info "  commit:     $commit"
log_info "  date:       $date"
log_info "  git_state:  $git_state"
if [[ "$git_state" == "dirty" ]]; then
  log_info "  git_status: $(echo "$git_status" | tr '|' ' ')"
fi
log_info "----------------------------------------"

##############################################
# Resolve the final install target
##############################################
#
# By default, go install writes to:
#   1. GOBIN, when set, otherwise
#   2. the first GOPATH entry plus /bin
#
# For test automation we also support --target-file, which bypasses go install
# target resolution completely and writes to an explicit file path via go build.
# This keeps disposable test binaries clearly separated from any user install.
#
go_exe_suffix="$(go env GOEXE)"
if [ -n "$CUSTOM_TARGET_FILE" ]; then
  install_target="$CUSTOM_TARGET_FILE"
else
  go_bin_dir="${GOBIN:-}"
  if [ -z "$go_bin_dir" ]; then
    go_bin_dir="$(go env GOBIN)"
  fi

  if [ -z "$go_bin_dir" ]; then
    go_path="$(go env GOPATH)"
    go_bin_dir="${go_path%%:*}/bin"
  fi

  target_name="trice${go_exe_suffix}"
  install_target="$go_bin_dir/$target_name"
fi

install_dir="$(cd -- "$(dirname -- "$install_target")" 2>/dev/null && pwd || true)"
if [ -z "$install_dir" ]; then
  install_dir="$(dirname -- "$install_target")"
fi

if [ -n "$CUSTOM_TARGET_FILE" ]; then
  target_name="$(basename -- "$install_target")"
else
  target_name="trice${go_exe_suffix}"
fi

backup_target=""

log_info "Install target: $install_target"
log_verbose "Resolved GOBIN: ${GOBIN:-<unset>}"
log_verbose "Resolved GOEXE: ${go_exe_suffix:-<none>}"
if [ -n "$CUSTOM_TARGET_FILE" ]; then
  log_verbose "Custom target file: $CUSTOM_TARGET_FILE"
fi

##############################################
# Protect an existing installation
##############################################
#
# Previous behaviour overwrote an existing target without notice.
# The new behaviour is:
# - no existing binary: continue silently
# - existing binary + --force: continue with the selected backup policy
# - existing binary + interactive terminal: ask what to do
# - existing binary + no terminal + no --force: abort with guidance
#
if [ -e "$install_target" ]; then
  log_info "Existing installation detected at: $install_target"

  # Version output is best-effort only. It must never block the build.
  if [ -x "$install_target" ]; then
    existing_version="$("$install_target" version -v 2>/dev/null || true)"
    if [ -n "$existing_version" ]; then
      log_verbose "Existing version information:"
      if [ "$SILENT" = false ] && [ "$VERBOSE" = true ]; then
        printf '%s\n' "$existing_version"
      fi
    fi
  fi

  if [ "$FORCE" = false ]; then
    if [ -t 0 ] && [ -t 1 ]; then
      if [ "$BACKUP_EXPLICIT" = false ]; then
        log_always "An existing trice installation would be overwritten:"
        log_always "  $install_target"
        log_always "Choose: [b]ackup and overwrite, overwrite [w]ithout backup, [a]bort"
        read -r -p "> " overwrite_choice
        case "${overwrite_choice:-b}" in
        b | B | "")
          BACKUP=true
          ;;
        w | W)
          BACKUP=false
          ;;
        a | A)
          log_always "Build cancelled."
          exit 0
          ;;
        *)
          log_error "Invalid choice. Build cancelled."
          exit 1
          ;;
        esac
      else
        if [ "$BACKUP" = true ]; then
          prompt_suffix="with backup"
        else
          prompt_suffix="without backup"
        fi
        read -r -p "Overwrite existing trice ${prompt_suffix}? [y/N] " overwrite_confirm
        case "${overwrite_confirm:-N}" in
        y | Y) ;;
        *)
          log_always "Build cancelled."
          exit 0
          ;;
        esac
      fi
    else
      log_error "Refusing to overwrite existing target in non-interactive mode: $install_target"
      log_error "Use --force to overwrite, optionally together with --no-backup."
      exit 1
    fi
  fi

  if [ "$BACKUP" = true ]; then
    timestamp="$(date +%Y%m%d-%H%M%S)"
    backup_target="$install_dir/Backup-${timestamp}_${target_name}"
    log_info "Creating backup: $backup_target"
    cp -p "$install_target" "$backup_target"
  else
    log_info "Overwriting existing installation without backup."
  fi
fi

##############################################
# Build command
##############################################
# ldflags injects the variables from main.go:
#
# var (
#     version string
#     commit  string
#     date    string
#     branch  string
#     gitState  string
#     gitStatus string
# )
#
# Make sure the package path ("main.") matches your actual package.
#
# Additionally:
#   -s —> strip the symbol table
#   -w —> strip DWARF debugging info
#
if [ -n "$CUSTOM_TARGET_FILE" ]; then
  mkdir -p "$install_dir"
  go build -o "$install_target" -ldflags "\
  -X 'main.version=$version' \
  -X 'main.commit=$commit' \
  -X 'main.date=$date' \
  -X 'main.branch=$origin - $branch' \
  -X 'main.gitState=$git_state' \
  -X 'main.gitStatus=$git_status' \
  -s \
  -w \
" ./cmd/trice
else
  go install -ldflags "\
  -X 'main.version=$version' \
  -X 'main.commit=$commit' \
  -X 'main.date=$date' \
  -X 'main.branch=$origin - $branch' \
  -X 'main.gitState=$git_state' \
  -X 'main.gitStatus=$git_status' \
  -s \
  -w \
" ./cmd/trice/...
fi

##############################################
# Final summary
##############################################
#
# In silent mode, suppress the final success summary as well so that only
# errors remain visible on stderr. In normal modes, the final target path is
# printed explicitly because it is the key result of the build.
log_info "Build complete."
log_info "Installed trice target: $install_target"
if [ -n "$backup_target" ]; then
  log_info "Backup created: $backup_target"
fi
