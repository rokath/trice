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
  # Native Windows terminals can leave CR at the end of an otherwise valid reply.
  answer="${answer%$'\r'}"
  case "$answer" in
    y | Y | yes | YES) return 0 ;;
    *) return 1 ;;
  esac
}

# Download release archives only after caller consent and verify their pinned
# SHA-256 before extraction. A verified cache also supports subsequent offline runs.
download_pinned_test_tool() {
  local url="$1" archive="$2" expected="$3" actual
  local checksum_command=()
  if command -v sha256sum >/dev/null 2>&1; then
    checksum_command=(sha256sum)
  elif command -v shasum >/dev/null 2>&1; then
    checksum_command=(shasum -a 256)
  else
    printf 'FAIL: Tool download requires sha256sum or shasum to verify %s.\n' "$archive" >&2
    return 1
  fi
  if [ -f "$archive" ]; then
    actual="$("${checksum_command[@]}" "$archive" | awk '{print $1}')"
    [ "$actual" != "$expected" ] || return 0
  fi
  if ! command -v curl >/dev/null 2>&1; then
    printf 'FAIL: Tool download requires curl: %s.\n' "$url" >&2
    return 1
  fi
  mkdir -p "$(dirname "$archive")" || return 1
  if ! curl -fL --retry 3 --connect-timeout 20 "$url" -o "$archive"; then
    printf 'FAIL: Could not download %s into %s.\n' "$url" "$archive" >&2
    return 1
  fi
  actual="$("${checksum_command[@]}" "$archive" | awk '{print $1}')"
  if [ "$actual" != "$expected" ]; then
    printf 'FAIL: SHA-256 mismatch for %s; expected %s, got %s.\n' "$archive" "$expected" "$actual" >&2
    return 1
  fi
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

# Extract the official Windows package into the checkout. Administrative MSI
# extraction creates a source image; it does not install or register the product.
install_windows_cppcheck_test_tool() {
  local install_dir="$1"
  local archive="./temp/tools/downloads/cppcheck-2.22.0-x64-Setup.msi"
  local tool runtime_dll
  for tool in msiexec.exe cygpath; do
    if ! command -v "$tool" >/dev/null 2>&1; then
      printf 'FAIL: Local Windows Cppcheck setup requires %s.\n' "$tool" >&2
      return 1
    fi
  done
  download_pinned_test_tool \
    "https://github.com/cppcheck-opensource/cppcheck/releases/download/2.22.0/cppcheck-2.22.0-x64-Setup.msi" \
    "$archive" "3929162bab7e71d1b9b18708ec054fb0188bfc8ea1660979ed95837f5a9f547b" || return 1
  mkdir -p "$install_dir" || return 1
  # Disable MSYS argument rewriting only for this native command: /a and /qn
  # are installer options, while all filesystem arguments are converted explicitly.
  if ! MSYS_NO_PATHCONV=1 msiexec.exe /a "$(cygpath -aw "$archive")" /qn \
    "TARGETDIR=$(cygpath -aw "$install_dir")" /L*v "$(cygpath -aw "$install_dir/extract.log")"; then
    printf 'FAIL: Cppcheck extraction failed; inspect %s/extract.log.\n' "$install_dir" >&2
    return 1
  fi
  # The source image stores the bundled VC runtime separately. Keep these DLLs
  # beside the local executable so a global VC redistributable is unnecessary.
  for runtime_dll in vcruntime140.dll msvcp140.dll; do
    if [ ! -f "$install_dir/PFiles/System64/$runtime_dll" ]; then
      printf 'FAIL: Cppcheck package is missing %s/PFiles/System64/%s.\n' "$install_dir" "$runtime_dll" >&2
      ls -l "$install_dir/PFiles/System64" >&2 || true
      return 1
    fi
  done
  for runtime_dll in "$install_dir"/PFiles/System64/*.dll; do
    if ! cp "$runtime_dll" "$install_dir/PFiles/Cppcheck/"; then
      printf 'FAIL: Cannot copy %s beside the local Cppcheck executable.\n' "$runtime_dll" >&2
      return 1
    fi
  done
}

# Use official Windows binaries or build the pinned release on Unix. Both paths
# keep tools local and independent of the system package-manager version.
install_cppcheck_test_tool() {
  local version="2.22.0"
  local tools_dir
  tools_dir="$(pwd)/temp/tools"
  local source_dir="$tools_dir/src/cppcheck-$version"
  local build_dir="$tools_dir/build/cppcheck-$version"
  local install_dir="$tools_dir/cppcheck/$version"
  local archive="$tools_dir/cppcheck-$version.tar.gz"
  local tool

  case "$(uname -s):$(uname -m)" in
    MINGW*:x86_64 | MSYS*:x86_64 | CYGWIN*:x86_64)
      install_windows_cppcheck_test_tool "$install_dir"
      return $?
      ;;
  esac

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

# Compare normalized native output so CRLF does not reject a Windows release.
cppcheck_test_tool_has_required_version() {
  local version_output
  [ -x "$1" ] || return 1
  version_output="$("$1" --version 2>/dev/null)" || return 1
  [ "${version_output%$'\r'}" = "Cppcheck 2.22.0" ]
}

# Ensure test 520 uses exactly the pinned analyzer, asking before any download.
ensure_cppcheck_test_tool() {
  local version="2.22.0"
  local cppcheck_bin=""
  local candidate
  if command -v cppcheck >/dev/null 2>&1; then
    cppcheck_bin="$(command -v cppcheck)"
  elif [ -x "/c/Program Files/Cppcheck/cppcheck.exe" ]; then
    cppcheck_bin="/c/Program Files/Cppcheck/cppcheck.exe"
  fi

  if [ -n "$cppcheck_bin" ] && cppcheck_test_tool_has_required_version "$cppcheck_bin"; then
    return 0
  fi

  # Reuse previous installations before asking, including when an older system
  # executable precedes them in PATH. Child lint profiles inherit this selection.
  for candidate in "./temp/tools/cppcheck/$version/bin/cppcheck" \
    "./temp/tools/cppcheck/$version/PFiles/Cppcheck/cppcheck.exe"; do
    if cppcheck_test_tool_has_required_version "$candidate"; then
      PATH="$(pwd)/$(dirname "$candidate"):$PATH"
      export PATH
      hash -r
      return 0
    fi
  done

  if ! confirm_tool_install "Cppcheck $version"; then
    printf 'Cppcheck %s is required for exhaustive target-code linting.\n' "$version" >&2
    return 1
  fi
  if ! install_cppcheck_test_tool; then
    return 1
  fi
  cppcheck_bin="$(command -v cppcheck 2>/dev/null || true)"
  # Windows extraction and Unix installation have different executable layouts.
  for candidate in "./temp/tools/cppcheck/$version/bin/cppcheck" \
    "./temp/tools/cppcheck/$version/PFiles/Cppcheck/cppcheck.exe"; do
    if cppcheck_test_tool_has_required_version "$candidate"; then
      PATH="$(pwd)/$(dirname "$candidate"):$PATH"
      export PATH
      hash -r
      return 0
    fi
  done
  if [ -z "$cppcheck_bin" ] || ! cppcheck_test_tool_has_required_version "$cppcheck_bin"; then
    printf 'Installed Cppcheck does not report the required version %s.\n' "$version" >&2
    return 1
  fi
}
