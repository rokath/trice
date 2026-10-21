#!/usr/bin/env bash

# Shared consent and checkout-local installation for pinned test tools.
# Callers resolve their repository root before sourcing this helper.

# Keep installed tools visible to child processes, including release hooks.
TRICE_TOOL_BIN_DIR="$(pwd)/temp/tools/bin"
export PATH="$TRICE_TOOL_BIN_DIR:$PATH"

# Ask before downloading a missing version. EOF, an empty answer and CI decline
# installation. A terminal stays visible even when the caller redirects logs.
confirm_tool_install() {
  local description="$1"
  local answer=""
  if [ -n "${CI:-}" ] && [ "${CI:-}" != "false" ]; then
    printf 'SKIP: %s installation needs confirmation; CI cannot prompt.\n' "$description" >&2
    return 1
  fi
  if [ -t 0 ] && { true >/dev/tty; } 2>/dev/null; then
    printf '\nInstall %s locally under temp/tools? [y/N] ' "$description" >/dev/tty
    IFS= read -r answer </dev/tty || return 1
  else
    printf 'Install %s locally under temp/tools? [y/N] ' "$description" >&2
    IFS= read -r answer || return 1
  fi
  case "$answer" in
    y | Y | yes | YES) return 0 ;;
    *) return 1 ;;
  esac
}

# Install only the requested pinned Go package; never overwrite system tools.
# The caller checks the resulting executable's identity before using it.
install_go_test_tool() {
  local package="$1"
  if ! command -v go >/dev/null 2>&1; then
    printf 'Cannot install %s: Go is required. Install Go and rerun.\n' "$package" >&2
    return 1
  fi
  mkdir -p "$TRICE_TOOL_BIN_DIR" || return 1
  if ! GOBIN="$TRICE_TOOL_BIN_DIR" go install "$package"; then
    printf 'Installation failed: %s into %s. Check the output above and rerun.\n' "$package" "$TRICE_TOOL_BIN_DIR" >&2
    return 1
  fi
  hash -r
}
