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

set -euo pipefail

# Ensure we always operate in the repository root.
cd "$(dirname "${BASH_SOURCE[0]}")"

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

echo "----------------------------------------"
echo "Building trice with embedded Git metadata:"
echo "  origin:     $origin"
echo "  branch:     $branch"
echo "  version:    $version"
echo "  commit:     $commit"
echo "  date:       $date"
echo "  git_state:  $git_state"
if [[ "$git_state" == "dirty" ]]; then
  echo "  git_status: $(echo "$git_status" | tr '|' ' ')"
fi
echo "----------------------------------------"

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

echo "Build complete."
