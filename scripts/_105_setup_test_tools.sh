#!/usr/bin/env bash

# Shared consent and installation helpers for test tools.
# Callers resolve their repository root before sourcing this helper.

# Keep installed tools visible to child processes, including release hooks.
TRICE_TOOL_BIN_DIR="$(pwd)/temp/tools/bin"
export PATH="$TRICE_TOOL_BIN_DIR:$PATH"

# Keep exact versions in one tracked data file. Fixture repositories copy it
# with this helper so standalone checks use the same contract as the suite.
# shellcheck source=./test-tools.env
if [ -f "./scripts/test-tools.env" ]; then
  source "./scripts/test-tools.env"
fi

# Ask before downloading a missing version. EOF, an empty answer and CI decline
# installation. A terminal stays visible even when the caller redirects logs.
confirm_tool_install() {
  local description="$1"
  # System package managers must name their destination instead of promising
  # a checkout-local installation. Existing tool callers keep their default.
  local destination="${2:-locally under temp/tools}"
  local answer=""
  if [ "${TRICE_TEST_SETUP_APPROVED:-0}" = 1 ] && [ "$destination" = "locally under temp/tools" ]; then
    return 0
  fi
  # CI has no person available to approve a checkout-local download; its
  # workflow provisions required host packages separately.
  if [ "${TRICE_TEST_AUTO_SETUP:-0}" = 1 ]; then
    if [ -n "${CI:-}" ] && [ "${CI:-}" != "false" ] && [ "$destination" = "locally under temp/tools" ]; then
      printf 'INFO: Preparing %s %s.\n' "$description" "$destination" >&2
      return 0
    fi
  fi
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
    y | Y | yes | YES)
      # Reuse the first missing-tool approval for the rest of preparation.
      # System package consent never authorizes checkout-local setup implicitly.
      if [ "${TRICE_TEST_AUTO_SETUP:-0}" = 1 ] && [ "$destination" = "locally under temp/tools" ]; then
        export TRICE_TEST_SETUP_APPROVED=1
      fi
      return 0
      ;;
    *) return 1 ;;
  esac
}

# List the manifest's declared versions without probing or installing tools.
# The runner prints this before preparation, including on fully cached runs.
print_pinned_test_tools() {
  local gcc_version clang_version
  case "$(uname -s)" in
    Linux*)
      gcc_version="$TRICE_GCC_LINUX_VERSION"
      clang_version="$TRICE_CLANG_LINUX_VERSION"
      ;;
    Darwin*)
      gcc_version="$TRICE_GCC_DARWIN_VERSION"
      clang_version="$TRICE_CLANG_DARWIN_VERSION"
      ;;
    MINGW* | MSYS* | CYGWIN*)
      gcc_version="$TRICE_GCC_WINDOWS_VERSION"
      clang_version="$TRICE_CLANG_WINDOWS_VERSION"
      ;;
    *)
      gcc_version=undeclared
      clang_version=undeclared
      ;;
  esac
  printf 'Pinned tool versions for this checkout:\n'
  printf '  Go %s; Node.js %s; CMake %s\n' "$TRICE_GO_VERSION" "$TRICE_NODE_VERSION" "$TRICE_CMAKE_VERSION"
  printf '  GCC/G++ %s; Clang/Clang++/clangd %s; ARM GCC %s\n' "$gcc_version" "$clang_version" "$TRICE_ARM_GCC_VERSION"
  printf '  clang-format %s; Cppcheck %s; shfmt %s; ShellCheck %s\n' "$TRICE_TOOL_CLANG_FORMAT_VERSION" "$TRICE_TOOL_CPPCHECK_VERSION" "$TRICE_TOOL_SHFMT_VERSION" "$TRICE_TOOL_SHELLCHECK_VERSION"
  printf '  GoReleaser %s; actionlint %s; jq %s\n' "$TRICE_TOOL_GORELEASER_VERSION" "$TRICE_TOOL_ACTIONLINT_VERSION" "$TRICE_TOOL_JQ_VERSION"
  printf '  markdownlint %s; mdtoc %s; stitchmd %s\n' "$TRICE_TOOL_MARKDOWNLINT_VERSION" "$TRICE_TOOL_MDTOC_VERSION" "$TRICE_TOOL_STITCHMD_VERSION"
  printf '  PDF/link checks: md-to-pdf %s; Puppeteer %s; lychee %s\n' "$TRICE_TOOL_MD_TO_PDF_VERSION" "$TRICE_TOOL_PUPPETEER_VERSION" "$TRICE_TOOL_LYCHEE_VERSION"
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) printf '  Managed Python %s\n' "$TRICE_PYTHON_VERSION" ;;
  esac
  printf '  Valid cached tools are reused; installations stay local to this worktree.\n'
}

# Approve one preparation phase for the missing checkout-local test tools.
# Individual installers reuse this decision, while host package changes still
# require their own confirmation. Optional context names the requesting cache
# or tool group in the visible prompt even when diagnostics go to tools.log.
approve_pinned_test_setup() {
  [ "${TRICE_TEST_SETUP_APPROVED:-0}" = 1 ] && return 0
  local description="pinned test tools and compiler bundles"
  [ -z "${1:-}" ] || description="$description ($1)"
  if ! confirm_tool_install "$description"; then
    printf 'INCOMPLETE: project-local test setup was not approved.\n' >&2
    return 1
  fi
  export TRICE_TEST_SETUP_APPROVED=1
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

# Select the publisher archive and checksum for the pinned Node.js runtime.
# The runtime stays inside the checkout so this setup never changes system Node.
ensure_node_test_tool() {
  local version="${TRICE_NODE_VERSION:?}" platform architecture archive_name archive_url archive_sha sha_var
  local cache_dir="./temp/tools/node/$version" archive="./temp/tools/downloads"
  local executable
  if command -v node >/dev/null 2>&1 && [ "$(node --version 2>/dev/null)" = "v$version" ] &&
    command -v npm >/dev/null 2>&1 && command -v npx >/dev/null 2>&1; then
    return 0
  fi
  platform="$(uname -s)"
  architecture="$(uname -m)"
  case "$platform:$architecture" in
    Linux:x86_64 | Linux:amd64) archive_name="node-v$version-linux-x64.tar.xz" ;;
    Linux:aarch64 | Linux:arm64) archive_name="node-v$version-linux-arm64.tar.xz" ;;
    Darwin:x86_64) archive_name="node-v$version-darwin-x64.tar.gz" ;;
    Darwin:arm64) archive_name="node-v$version-darwin-arm64.tar.gz" ;;
    MINGW*:x86_64 | MSYS*:x86_64 | CYGWIN*:x86_64 | MINGW*:amd64 | MSYS*:amd64 | CYGWIN*:amd64)
      archive_name="node-v$version-win-x64.zip"
      ;;
    MINGW*:aarch64 | MSYS*:aarch64 | CYGWIN*:aarch64 | MINGW*:arm64 | MSYS*:arm64 | CYGWIN*:arm64)
      archive_name="node-v$version-win-arm64.zip"
      ;;
    *)
      printf 'FAIL: no pinned Node.js %s archive is available for %s/%s.\n' "$version" "$platform" "$architecture" >&2
      return 1
      ;;
  esac
  case "$archive_name" in
    *linux-x64*) sha_var=TRICE_NODE_SHA256_LINUX_X64 ;;
    *linux-arm64*) sha_var=TRICE_NODE_SHA256_LINUX_ARM64 ;;
    *darwin-x64*) sha_var=TRICE_NODE_SHA256_DARWIN_X64 ;;
    *darwin-arm64*) sha_var=TRICE_NODE_SHA256_DARWIN_ARM64 ;;
    *win-x64*) sha_var=TRICE_NODE_SHA256_WINDOWS_X64 ;;
    *win-arm64*) sha_var=TRICE_NODE_SHA256_WINDOWS_ARM64 ;;
  esac
  archive_sha="${!sha_var:?}"
  case "$platform" in
    MINGW* | MSYS* | CYGWIN*)
      executable="$cache_dir/node-v$version-win-$(case "$archive_name" in *arm64*) printf arm64 ;; *) printf x64 ;; esac)/node.exe"
      ;;
    *)
      executable="$cache_dir/node-v$version-$(case "$archive_name" in *linux*) printf linux ;; *) printf darwin ;; esac)-$(case "$archive_name" in *arm64*) printf arm64 ;; *x64*) printf x64 ;; esac)/bin/node"
      ;;
  esac
  if [ -x "$executable" ] && [ "$("$executable" --version 2>/dev/null)" = "v$version" ]; then
    PATH="$(dirname "$executable"):$PATH"
    export PATH
    hash -r
    if command -v npm >/dev/null 2>&1 && command -v npx >/dev/null 2>&1; then
      return 0
    fi
  fi
  if ! confirm_tool_install "Node.js $version (about 35 MB download)"; then
    printf 'FAIL: Node.js %s setup was not approved.\n' "$version" >&2
    return 1
  fi
  archive="$archive/$archive_name"
  archive_url="https://nodejs.org/download/release/v$version/$archive_name"
  download_pinned_test_tool "$archive_url" "$archive" "$archive_sha" || return 1
  mkdir -p "$cache_dir" || return 1
  case "$archive_name" in
    *.zip) unzip -q -o "$archive" -d "$cache_dir" || return 1 ;;
    *) tar -xf "$archive" -C "$cache_dir" || return 1 ;;
  esac
  if [ ! -x "$executable" ] || [ "$("$executable" --version 2>/dev/null)" != "v$version" ]; then
    printf 'FAIL: extracted Node.js runtime did not pass its version check: %s\n' "$executable" >&2
    return 1
  fi
  PATH="$(dirname "$executable"):$PATH"
  export PATH
  hash -r
  if [ "$(node --version)" != "v$version" ] || ! command -v npm >/dev/null 2>&1 || ! command -v npx >/dev/null 2>&1; then
    printf 'FAIL: checkout-local Node.js installation is missing npm or npx.\n' >&2
    return 1
  fi
  printf 'INFO: selected checkout-local Node.js %s from %s.\n' "$version" "$executable" >&2
}

# Select Python's official standard-library-only Windows runtime. Its private
# executable is never exposed as a general-purpose pip/venv installation.
# Checksums: https://www.python.org/downloads/release/python-3152/
select_windows_python_test_package() {
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) ;;
    *) return 1 ;;
  esac
  case "$(uname -m)" in
    x86_64 | amd64)
      TRICE_PYTHON_ARCH=amd64
      TRICE_PYTHON_SHA256=d012f3aa29cf07436dae8aea1fec0fd195c2612a0b059b1f915ae0afb9ad1f62
      ;;
    i686 | i386)
      TRICE_PYTHON_ARCH=win32
      TRICE_PYTHON_SHA256=8d20269d378040117c9cec11d1ad6c6d17c6b77774424955ee180a0f49c6b41e
      ;;
    aarch64 | arm64)
      TRICE_PYTHON_ARCH=arm64
      TRICE_PYTHON_SHA256=089eae5e8aba8284c2ce59df008c1dc915af46fd9e8b1227e3a7f315211567c9
      ;;
    *)
      printf 'FAIL: No local Python runtime for Windows architecture %s; install Python 3.9+ and rerun.\n' "$(uname -m)" >&2
      return 1
      ;;
  esac
  TRICE_PYTHON_PACKAGE="python-${TRICE_PYTHON_VERSION:?}-embed-$TRICE_PYTHON_ARCH.zip"
  TRICE_PYTHON_DIR="./temp/tools/python/$TRICE_PYTHON_VERSION-$TRICE_PYTHON_ARCH"
}

# Prove that the interpreter starts and provides the standard library used by
# the reports. Redirect stdin so Store aliases cannot consume a consent reply.
python_test_tool_works() {
  "$@" -c 'import sys, json, html, pathlib, urllib.parse; sys.exit(0 if sys.version_info.major == 3 and sys.version_info >= (3, 9) and pathlib.Path(".").is_relative_to(".") else 1)' </dev/null >/dev/null 2>&1
}

# Reuse existing Python first, including launcher-only Windows installations.
find_python_test_tool() {
  local candidate
  if [ -n "${TRICE_TEST_PYTHON:-}" ] && python_test_tool_works "$TRICE_TEST_PYTHON"; then
    return 0
  fi
  if [ -n "${TRICE_TEST_PYTHON:-}" ]; then
    # Do not reselect a damaged private cache in the next independently run step.
    unset TRICE_TEST_PYTHON
    if [ -n "${TRICE_PYTHON_DIR:-}" ]; then
      rm -f "$TRICE_PYTHON_DIR/.ready" || return 1
    fi
  fi
  for candidate in python3 python; do
    if command -v "$candidate" >/dev/null 2>&1 && python_test_tool_works "$candidate"; then
      return 0
    fi
  done
  command -v py >/dev/null 2>&1 && python_test_tool_works py -3
}

# A separate test-runtime variable avoids shadowing a complete Python install.
# Every suite child loads this verified cache independently, without downloads.
activate_cached_python_test_tool() {
  [ -d ./temp/tools/python ] || return 0
  select_windows_python_test_package || return 0
  if [ -f "$TRICE_PYTHON_DIR/.ready" ] && [ -x "$TRICE_PYTHON_DIR/python.exe" ]; then
    TRICE_TEST_PYTHON="$(cd "$TRICE_PYTHON_DIR" && pwd)/python.exe"
    # Go subprocesses on Windows cannot execute Git Bash's /c/... paths.
    if command -v cygpath >/dev/null 2>&1; then
      TRICE_TEST_PYTHON="$(cygpath -am "$TRICE_TEST_PYTHON")" || return 1
    fi
    export TRICE_TEST_PYTHON
  fi
}

# Install only after consent. An interrupted, corrupt or unrunnable extraction
# has no ready marker and cannot become a later step's interpreter.
ensure_python_test_tool() {
  local archive
  if find_python_test_tool; then
    return 0
  fi
  if ! select_windows_python_test_package; then
    printf 'FAIL: Python 3.9+ is required for full script tests; install it with its standard library and rerun.\n' >&2
    return 1
  fi
  rm -f "$TRICE_PYTHON_DIR/.ready" || return 1
  if ! confirm_tool_install "Python $TRICE_PYTHON_VERSION standard-library runtime (about 13 MB download)"; then
    printf 'FAIL: Python script tests require a working interpreter; local setup was not approved.\n' >&2
    return 1
  fi
  if ! command -v unzip >/dev/null 2>&1; then
    printf 'FAIL: Local Python setup requires unzip (included with Git Bash).\n' >&2
    return 1
  fi
  archive="./temp/tools/downloads/$TRICE_PYTHON_PACKAGE"
  download_pinned_test_tool "https://www.python.org/ftp/python/$TRICE_PYTHON_VERSION/$TRICE_PYTHON_PACKAGE" \
    "$archive" "$TRICE_PYTHON_SHA256" || return 1
  mkdir -p "$TRICE_PYTHON_DIR" || return 1
  if ! unzip -q -o "$archive" -d "$TRICE_PYTHON_DIR"; then
    printf 'FAIL: Cannot extract %s into %s.\n' "$archive" "$TRICE_PYTHON_DIR" >&2
    return 1
  fi
  if ! python_test_tool_works "$TRICE_PYTHON_DIR/python.exe" ||
    ! "$TRICE_PYTHON_DIR/python.exe" -c 'import sys; sys.exit(0 if sys.version_info[:3] == tuple(map(int, sys.argv[1].split("."))) else 1)' "$TRICE_PYTHON_VERSION" </dev/null; then
    printf 'FAIL: Extracted Python runtime cannot run with the required version and standard library: %s/python.exe.\n' "$TRICE_PYTHON_DIR" >&2
    return 1
  fi
  touch "$TRICE_PYTHON_DIR/.ready" || return 1
  activate_cached_python_test_tool
  printf 'INFO: verified checkout-local Python: %s\n' "$TRICE_TEST_PYTHON"
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
      TRICE_HOST_PACKAGE_SHA256="${TRICE_WINDOWS_HOST_SHA_AMD64:-1f04e482797dc5f71fe2875cf2b743bfb9f5fd1de0955fdd3118438c0432ba5f}"
      ;;
    386 | i686 | i386)
      TRICE_HOST_PACKAGE_ARCH=i686-posix-dwarf
      TRICE_HOST_PACKAGE_ROOT=mingw32
      TRICE_HOST_PACKAGE_SHA256="${TRICE_WINDOWS_HOST_SHA_386:-9353f386f7a678ab41b0746db43211c4b6b8ab2811d2edb289f0496711fb1c9b}"
      ;;
    *)
      printf 'FAIL: No bundled Windows GCC/Clang toolchain for architecture %s. Provide matching native compilers in PATH.\n' "$architecture" >&2
      return 1
      ;;
  esac
  TRICE_HOST_PACKAGE="winlibs-$TRICE_HOST_PACKAGE_ARCH-gcc-${TRICE_GCC_WINDOWS_VERSION:-16.2.0}-llvm-${TRICE_CLANG_WINDOWS_VERSION:-23.1.3}-mingw-w64ucrt-14.0.0-r3"
  TRICE_HOST_PACKAGE_DIR="./temp/tools/host/$TRICE_HOST_PACKAGE"
  TRICE_HOST_PACKAGE_BIN="$TRICE_HOST_PACKAGE_DIR/$TRICE_HOST_PACKAGE_ROOT/bin"
}

# Reuse only an installation whose compiler/linker/runtime probes succeeded.
# Each test step loads this selection independently; a child's PATH changes do
# not propagate back into the runner. No shell code is read from the cache.
activate_cached_host_test_tools() {
  local bin=""
  # A prepared native run has already frozen both compiler families. Legacy
  # cache discovery must not override that selection in a later child step.
  [ "${TRICE_TEST_FIXED_HOST:-0}" != 1 ] || return 0
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      # A fresh checkout needs no package/Go architecture lookup at this point.
      [ -d ./temp/tools/host ] || return 0
      select_windows_host_test_package || return 0
      if [ -f "$TRICE_HOST_PACKAGE_DIR/.ready" ]; then
        ensure_windows_host_test_aliases || return 1
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
  local archive compiler
  select_windows_host_test_package || return 1
  if ! command -v unzip >/dev/null 2>&1; then
    printf 'FAIL: Local GCC/Clang setup requires unzip (included with Git Bash).\n' >&2
    return 1
  fi
  if [ "${TRICE_TEST_AUTO_SETUP:-0}" = 1 ]; then
    approve_pinned_test_setup "Windows GCC $TRICE_GCC_WINDOWS_VERSION / Clang $TRICE_CLANG_WINDOWS_VERSION bundle" || return 1
  fi
  archive="./temp/tools/downloads/$TRICE_HOST_PACKAGE.zip"
  download_pinned_test_tool \
    "https://github.com/brechtsanders/winlibs_mingw/releases/download/${TRICE_WINDOWS_HOST_RELEASE:-16.2.0posix-23.1.3-14.0.0-ucrt-r3}/$TRICE_HOST_PACKAGE.zip" \
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
  ensure_windows_host_test_aliases
}

# Native subprocesses need executable names, not shell aliases. Reuse the
# bundle's current Make instead of an unrelated legacy make.exe later in PATH.
# Applying this to verified caches also repairs bundles extracted before Make
# was included in the generic-name mapping, without another download.
ensure_windows_host_test_aliases() {
  local alias_name compiler
  for alias_name in cc c++ make; do
    case "$alias_name" in
      cc) compiler=gcc ;;
      c++) compiler=g++ ;;
      make) compiler=mingw32-make ;;
    esac
    if [ ! -x "$TRICE_HOST_PACKAGE_BIN/$compiler.exe" ]; then
      printf 'FAIL: Compiler bundle is missing %s/%s.exe for %s.\n' "$TRICE_HOST_PACKAGE_BIN" "$compiler" "$alias_name" >&2
      return 1
    fi
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
    "$archive" "$TRICE_CPPCHECK_WINDOWS_SHA" || return 1
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
  local version="${TRICE_TOOL_CPPCHECK_VERSION:-2.22.0}"
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
    if ! download_pinned_test_tool "https://codeload.github.com/cppcheck-opensource/cppcheck/tar.gz/refs/tags/$version" "$archive" "$TRICE_CPPCHECK_SOURCE_SHA"; then
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
  [ "${version_output%$'\r'}" = "Cppcheck ${TRICE_TOOL_CPPCHECK_VERSION:-2.22.0}" ]
}

# Ensure test 520 uses exactly the pinned analyzer, asking before any download.
ensure_cppcheck_test_tool() {
  local version="${TRICE_TOOL_CPPCHECK_VERSION:-2.22.0}"
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

# Only verified private Python runtimes are inherited by subsequent test steps.
activate_cached_python_test_tool
