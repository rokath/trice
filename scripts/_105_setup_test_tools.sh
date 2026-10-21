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

# Build the pinned Cppcheck release from official source into the local tools
# tree, keeping the test independent of system package-manager versions.
install_cppcheck_test_tool() {
  local version="2.22.0"
  local tools_dir
  tools_dir="$(pwd)/temp/tools"
  local source_dir="$tools_dir/src/cppcheck-$version"
  local build_dir="$tools_dir/build/cppcheck-$version"
  local install_dir="$tools_dir/cppcheck/$version"
  local archive="$tools_dir/cppcheck-$version.tar.gz"
  local tool

  for tool in cmake curl tar; do
    if ! command -v "$tool" >/dev/null 2>&1; then
      printf 'Cannot install Cppcheck %s locally: %s is required. Install it and rerun.\n' "$version" "$tool" >&2
      return 1
    fi
  done

  mkdir -p "$tools_dir/src" "$tools_dir/build" || return 1
  if [ ! -d "$source_dir" ]; then
    if ! curl -fL --retry 3 "https://codeload.github.com/cppcheck-opensource/cppcheck/tar.gz/refs/tags/$version" -o "$archive"; then
      printf 'Could not download the official Cppcheck %s source archive.\n' "$version" >&2
      return 1
    fi
    if ! tar -xzf "$archive" -C "$tools_dir/src"; then
      printf 'Could not extract the Cppcheck %s source archive.\n' "$version" >&2
      return 1
    fi
  fi

  if ! cmake -S "$source_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$install_dir" -DBUILD_GUI=OFF; then
    printf 'Cppcheck %s configuration failed.\n' "$version" >&2
    return 1
  fi
  if ! cmake --build "$build_dir" --config Release --parallel; then
    printf 'Cppcheck %s build failed.\n' "$version" >&2
    return 1
  fi
  if ! cmake --install "$build_dir" --config Release; then
    printf 'Cppcheck %s local installation failed.\n' "$version" >&2
    return 1
  fi
  PATH="$install_dir/bin:$PATH"
  export PATH
  hash -r
}

# Ensure test 520 uses exactly the pinned analyzer, asking before any download.
ensure_cppcheck_test_tool() {
  local version="2.22.0"
  local cppcheck_bin=""
  local cppcheck_version=""
  if command -v cppcheck >/dev/null 2>&1; then
    cppcheck_bin="$(command -v cppcheck)"
  elif [ -x "/c/Program Files/Cppcheck/cppcheck.exe" ]; then
    cppcheck_bin="/c/Program Files/Cppcheck/cppcheck.exe"
  fi

  if [ -n "$cppcheck_bin" ]; then
    cppcheck_version="$("$cppcheck_bin" --version 2>/dev/null || true)"
    cppcheck_version="${cppcheck_version#Cppcheck }"
  fi
  if [ "$cppcheck_version" = "$version" ]; then
    return 0
  fi

  if ! confirm_tool_install "Cppcheck $version"; then
    printf 'Cppcheck %s is required for exhaustive target-code linting.\n' "$version" >&2
    return 1
  fi
  if ! install_cppcheck_test_tool; then
    return 1
  fi
  cppcheck_bin="$(command -v cppcheck 2>/dev/null || true)"
  if [ -z "$cppcheck_bin" ] || [ "$("$cppcheck_bin" --version 2>/dev/null || true)" != "Cppcheck $version" ]; then
    printf 'Installed Cppcheck does not report the required version %s.\n' "$version" >&2
    return 1
  fi
}
