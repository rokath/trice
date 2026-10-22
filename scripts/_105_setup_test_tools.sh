#!/usr/bin/env bash

# Shared consent and installation helpers for test tools.
# Callers resolve their repository root before sourcing this helper.

# Keep installed tools visible to child processes, including release hooks.
TRICE_TOOL_BIN_DIR="$(pwd)/temp/tools/bin"
export PATH="$TRICE_TOOL_BIN_DIR:$PATH"

# Ask before downloading a missing version. EOF, an empty answer and CI decline
# installation. A terminal stays visible even when the caller redirects logs.
confirm_tool_install() {
  local description="$1"
  # System package managers must name their destination instead of promising
  # a checkout-local installation. Existing tool callers keep their default.
  local destination="${2:-locally under temp/tools}"
  local answer=""
  if [ -n "${CI:-}" ] && [ "${CI:-}" != "false" ]; then
    printf 'SKIP: %s installation needs confirmation; CI cannot prompt.\n' "$description" >&2
    return 1
  fi
  if [ -t 0 ] && { true >/dev/tty; } 2>/dev/null; then
    printf '\nInstall %s %s? [y/N] ' "$description" "$destination" >/dev/tty
  else
    printf 'Install %s %s? [y/N] ' "$description" "$destination" >&2
  fi
  # Read the caller's input descriptor, not a reopened Git Bash terminal.
  # Keep EOF distinct from an explicit refusal so quiet logs explain failures.
  if ! IFS= read -r answer; then
    printf 'FAIL: Cannot read confirmation for %s; rerun from an interactive terminal.\n' "$description" >&2
    return 1
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

# Select a complete native Windows compiler bundle, never an ARM cross-toolchain.
# Match Go's target architecture when available so the same GCC also serves CGO.
# Release URLs and SHA-256 values come from the publisher's pinned release:
# https://github.com/brechtsanders/winlibs_mingw/releases/tag/16.2.0posix-23.1.3-14.0.0-ucrt-r3
select_windows_host_test_package() {
  local architecture go_architecture=""
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) ;;
    *) return 1 ;;
  esac
  architecture="$(uname -m)"
  if command -v go >/dev/null 2>&1; then
    go_architecture="$(go env GOARCH </dev/null 2>/dev/null)" || return 1
    architecture="${go_architecture%$'\r'}"
  fi
  case "$architecture" in
    amd64 | x86_64)
      TRICE_HOST_PACKAGE_ARCH=x86_64-posix-seh
      TRICE_HOST_PACKAGE_ROOT=mingw64
      TRICE_HOST_PACKAGE_SHA256=1f04e482797dc5f71fe2875cf2b743bfb9f5fd1de0955fdd3118438c0432ba5f
      ;;
    386 | i686 | i386)
      TRICE_HOST_PACKAGE_ARCH=i686-posix-dwarf
      TRICE_HOST_PACKAGE_ROOT=mingw32
      TRICE_HOST_PACKAGE_SHA256=9353f386f7a678ab41b0746db43211c4b6b8ab2811d2edb289f0496711fb1c9b
      ;;
    *)
      printf 'FAIL: No bundled Windows GCC/Clang toolchain for architecture %s. Provide matching native compilers in PATH.\n' "$architecture" >&2
      return 1
      ;;
  esac
  TRICE_HOST_PACKAGE="winlibs-$TRICE_HOST_PACKAGE_ARCH-gcc-16.2.0-llvm-23.1.3-mingw-w64ucrt-14.0.0-r3"
  TRICE_HOST_PACKAGE_DIR="./temp/tools/host/$TRICE_HOST_PACKAGE"
  TRICE_HOST_PACKAGE_BIN="$TRICE_HOST_PACKAGE_DIR/$TRICE_HOST_PACKAGE_ROOT/bin"
}

# Reuse only an installation whose compiler/linker/runtime probes succeeded.
# Each test step loads this selection independently; a child's PATH changes do
# not propagate back into the runner. No shell code is read from the cache.
activate_cached_host_test_tools() {
  local bin=""
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      # A fresh checkout needs no package/Go architecture lookup at this point.
      [ -d ./temp/tools/host ] || return 0
      select_windows_host_test_package || return 0
      if [ -f "$TRICE_HOST_PACKAGE_DIR/.ready" ]; then
        bin="$TRICE_HOST_PACKAGE_BIN"
      fi
      ;;
    *)
      if [ -f ./temp/tools/host-gcc/.ready ]; then
        bin=./temp/tools/host-gcc/bin
      fi
      ;;
  esac
  [ -n "$bin" ] || return 0
  { [ -x "$bin/gcc" ] || [ -x "$bin/gcc.exe" ]; } || return 0
  { [ -x "$bin/g++" ] || [ -x "$bin/g++.exe" ]; } || return 0
  PATH="$(cd "$bin" && pwd):$PATH"
  export PATH
  hash -r
}

# Unpack GCC, Clang, their headers and runtime libraries as one verified unit.
# Extraction is local, needs no administrator rights, and leaves an incomplete
# cache inactive until the caller has successfully run the host compiler probes.
install_windows_host_test_tools() {
  local archive compiler alias_name
  select_windows_host_test_package || return 1
  if ! command -v unzip >/dev/null 2>&1; then
    printf 'FAIL: Local GCC/Clang setup requires unzip (included with Git Bash).\n' >&2
    return 1
  fi
  archive="./temp/tools/downloads/$TRICE_HOST_PACKAGE.zip"
  download_pinned_test_tool \
    "https://github.com/brechtsanders/winlibs_mingw/releases/download/16.2.0posix-23.1.3-14.0.0-ucrt-r3/$TRICE_HOST_PACKAGE.zip" \
    "$archive" "$TRICE_HOST_PACKAGE_SHA256" || return 1
  mkdir -p "$TRICE_HOST_PACKAGE_DIR" || return 1
  if ! unzip -q -o "$archive" -d "$TRICE_HOST_PACKAGE_DIR"; then
    printf 'FAIL: Cannot extract %s into %s; check free disk space and archive diagnostics.\n' "$archive" "$TRICE_HOST_PACKAGE_DIR" >&2
    return 1
  fi
  for compiler in gcc g++ clang clang++ clangd; do
    if [ ! -x "$TRICE_HOST_PACKAGE_BIN/$compiler.exe" ]; then
      printf 'FAIL: Compiler bundle is missing %s/%s.exe.\n' "$TRICE_HOST_PACKAGE_BIN" "$compiler" >&2
      ls -l "$TRICE_HOST_PACKAGE_BIN" >&2 || true
      return 1
    fi
  done
  # Native Go subprocesses cannot execute shell aliases on Windows. Keep the
  # generic frontends in the same bin directory as the packaged GCC drivers.
  for alias_name in cc c++; do
    compiler=gcc
    [ "$alias_name" != c++ ] || compiler=g++
    if [ ! -x "$TRICE_HOST_PACKAGE_BIN/$alias_name.exe" ]; then
      cp "$TRICE_HOST_PACKAGE_BIN/$compiler.exe" "$TRICE_HOST_PACKAGE_BIN/$alias_name.exe" || return 1
    fi
  done
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
  version_output="$("$1" --version </dev/null 2>/dev/null)" || return 1
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
