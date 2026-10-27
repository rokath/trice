#!/usr/bin/env bash

# Shared native test preparation. Sourced by the common test environment;
# functions are inert until the runner (or CI setup) requests preparation.

# Probe the exact Go toolchain offline first. Go's launcher may fetch a missing
# toolchain, so that transition needs the same consent as other installations.
ensure_native_go_toolchain() {
  export GOTOOLCHAIN="go$TRICE_GO_VERSION"
  if GOPROXY=off go version >/dev/null 2>&1; then return 0; fi
  approve_pinned_test_setup || return 2
  go version >/dev/null || return 2
}

# ensure_native_report_tool also serves directly invoked lint/test steps.
# The exact Go toolchain builds the current helper without a global install.
ensure_native_report_tool() {
  if [ -n "${TRICE_TEST_REPORT_BIN:-}" ] && [ -x "$TRICE_TEST_REPORT_BIN" ]; then return 0; fi
  if ! command -v go >/dev/null 2>&1; then
    printf 'INCOMPLETE: Go %s is required to prepare the declared test tools.\n' "$TRICE_GO_VERSION" >&2
    return 2
  fi
  ensure_native_go_toolchain || return 2
  TRICE_TEST_REPORT_BIN="$LOG_DIR/testcheck$(go env GOEXE)"
  export TRICE_TEST_REPORT_BIN
  go build -o "$TRICE_TEST_REPORT_BIN" ./scripts/testcheck || return 2
}

# Resolve tools before asking to install. Keep approval in this caller rather
# than inside command substitution, so later tools reuse the same decision.
# The result variable belongs to the calling function's Bash scope.
prepare_pinned_test_selection() {
  local result_variable="$1" operation="$2" cache="$3" setup_output status
  shift 3
  if setup_output="$("$TRICE_TEST_REPORT_BIN" "$operation" --no-install scripts/test-tools.env "$cache" "$@")"; then
    :
  else
    status=$?
    [ "$status" -eq 3 ] || return "$status"
    approve_pinned_test_setup || return 2
    setup_output="$("$TRICE_TEST_REPORT_BIN" "$operation" scripts/test-tools.env "$cache" "$@")" || return 2
  fi
  printf -v "$result_variable" '%s' "$setup_output"
}

# ensure_pinned_test_tools prepares only the named standalone tools. It shares
# the same manifest/cache as the suite and never modifies a system installation.
ensure_pinned_test_tools() {
  local bin
  ensure_native_report_tool || return 2
  prepare_pinned_test_selection bin tools temp/tools/native "$@" || return 2
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) bin="$(cygpath -u "$bin")" ;;
  esac
  export PATH="$bin:$PATH"
  hash -r
}

# Select the generator matching the Windows GNU runtime for every child build.
# Official CMake otherwise defaults to NMake, which requires a different SDK.
prepare_native_cmake_environment() {
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      if ! command -v mingw32-make >/dev/null 2>&1; then
        printf 'MISSING TOOL: mingw32-make; the selected Windows compiler bundle must include its native Make.\n' >&2
        return 2
      fi
      export CMAKE_GENERATOR='MinGW Makefiles'
      # Platform/toolset overrides belong to Visual Studio generators.
      unset CMAKE_GENERATOR_PLATFORM CMAKE_GENERATOR_TOOLSET CMAKE_GENERATOR_INSTANCE
      printf 'Selected CMake generator: %s\n' "$CMAKE_GENERATOR"
      ;;
  esac
}

# prepare_native_test_tools fails before tests when a required prerequisite is
# missing. System packages/SDKs remain explicit prerequisites; project tools
# can be prepared noninteractively in their isolated caches.
prepare_native_test_tools() {
  local tool missing=0 compiler gcc_bin="" cpp_bin="" directory candidate platform
  export TRICE_TEST_AUTO_SETUP=1
  for tool in git go bash tar unzip make curl; do
    if ! command -v "$tool" >/dev/null 2>&1; then
      printf 'MISSING TOOL: %s; install the native prerequisite listed in docs/rm/target_tests.md\n' "$tool"
      missing=1
    fi
  done
  [ "$missing" -eq 0 ] || return 2
  ensure_node_test_tool || return 2
  if [ "$(node --version)" != "v$TRICE_NODE_VERSION" ]; then
    printf 'INCOMPLETE: Node.js %s is required; found %s. Select the declared version before running native tests.\n' "$TRICE_NODE_VERSION" "$(node --version)"
    return 2
  fi
  # The Go launcher resolves this exact supported toolchain when necessary.
  ensure_native_go_toolchain || return 2
  # The report helper is built from the current worktree before any tests.
  TRICE_TEST_REPORT_BIN="$LOG_DIR/testcheck$(go env GOEXE)"
  export TRICE_TEST_REPORT_BIN
  go build -o "$TRICE_TEST_REPORT_BIN" ./scripts/testcheck || return 2
  printf 'Commit: %s\nOS: %s\n' "$(git rev-parse HEAD)" "$(uname -a)"
  printf 'Runner image: %s %s\n' "${ImageOS:-local}" "${ImageVersion:-not supplied}"
  if [ "$(uname -s)" = Darwin ]; then
    printf 'macOS SDK: '
    xcrun --show-sdk-version || return 2
    # Upstream Clang must use the installed SDK too, independent of the SDK
    # path on the machine that built the downloaded LLVM distribution.
    SDKROOT="$(xcrun --show-sdk-path)" || return 2
    export SDKROOT
    printf '%s\n' "$SDKROOT"
  fi
  go version
  # Freeze the selected Go toolchain so nested coverage commands agree too.
  GOTOOLCHAIN="$(go env GOVERSION)" || return 2
  export GOTOOLCHAIN
  export CGO_ENABLED=1 TRICE_BIND_INTEGRATION=1 TRICE_RUN_WINDOWS_TCP_TESTS=1
  # LLVM's Intel macOS source package needs the pinned CMake before compilers
  # are selected. Restore this CMake at the end after all PATH changes too.
  local cmake_bin
  prepare_pinned_test_selection cmake_bin bundle temp/tools/bundles cmake || return 2
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) cmake_bin="$(cygpath -u "$cmake_bin")" ;;
  esac
  export PATH="$cmake_bin:$PATH"
  case "$(uname -s)" in
    Linux | Darwin)
      local clang_bin
      prepare_pinned_test_selection clang_bin bundle temp/tools/bundles clang || return 2
      export PATH="$clang_bin:$PATH"
      hash -r
      ;;
  esac
  if [ "$(go env GOHOSTOS)" = linux ]; then
    local native_gcc
    prepare_pinned_test_selection native_gcc bundle temp/tools/bundles gcc || return 2
    export PATH="$native_gcc:$PATH"
  fi

  # Select genuine versioned GCC on macOS without replacing Apple's commands.
  # Every selected compiler is subsequently compiled/linked/executed in 490.
  if [ "$(uname -s)" = Darwin ]; then
    local directories=()
    IFS=: read -r -a directories <<<"$PATH"
    for directory in "${directories[@]}"; do
      for candidate in "$directory"/gcc-[0-9]*; do
        [ -x "$candidate" ] || continue
        [ -x "${candidate%/gcc-*}/g++-${candidate##*/gcc-}" ] || continue
        [ "$("$candidate" -dumpfullversion -dumpversion)" = "$TRICE_GCC_DARWIN_VERSION" ] || continue
        gcc_bin="$candidate"
        cpp_bin="${candidate%/gcc-*}/g++-${candidate##*/gcc-}"
      done
    done
    if [ -n "$gcc_bin" ]; then
      mkdir -p "$LOG_DIR/compilers" || return 2
      ln -s "$gcc_bin" "$LOG_DIR/compilers/gcc" || return 2
      ln -s "$cpp_bin" "$LOG_DIR/compilers/g++" || return 2
      export PATH="$LOG_DIR/compilers:$PATH"
    fi
  fi
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      # A functional but differently versioned global compiler must not choose
      # the test environment. Select the complete declared runtime first.
      local host_matches=1 host_version host_path
      for compiler in gcc g++ clang clang++ clangd; do
        host_version="$TRICE_CLANG_WINDOWS_VERSION"
        case "$compiler" in gcc | g++) host_version="$TRICE_GCC_WINDOWS_VERSION" ;; esac
        host_path="$(command -v "$compiler" || true)"
        if [ -z "$host_path" ] || ! "$TRICE_TEST_REPORT_BIN" identity "$(cygpath -am "$host_path")" "$host_version"; then
          host_matches=0
        fi
      done
      if [ "$host_matches" -eq 0 ]; then
        install_windows_host_test_tools || return 2
        PATH="$(cd "$TRICE_HOST_PACKAGE_BIN" && pwd):$PATH" || return 2
        export PATH
        hash -r
      fi
      ;;
    *) export TRICE_TEST_FIXED_HOST=1 ;;
  esac
  # Probe both native compiler families before any dependent check. Windows can
  # reuse the verified complete bundle; SDK/system installs remain prerequisites.
  if ! SELECTED=full bash "$SCRIPTS_DIR/_490_test_runtime_prepare.sh" --probe; then
    return 2
  fi
  if [ "$(uname -s)" = Linux ] && [ -d "$LOG_DIR/compilers" ]; then
    export PATH="$LOG_DIR/compilers:$PATH"
    hash -r
  fi
  activate_cached_host_test_tools
  export TRICE_TEST_FIXED_HOST=1 CC=gcc CXX=g++
  platform="$(go env GOHOSTOS)"
  platform="$(printf '%s' "$platform" | tr '[:lower:]' '[:upper:]')"
  local gcc_key="TRICE_GCC_${platform}_VERSION" clang_key="TRICE_CLANG_${platform}_VERSION"
  if [ "$platform:$(go env GOHOSTARCH)" = DARWIN:amd64 ]; then clang_key=TRICE_CLANG_DARWIN_AMD64_VERSION; fi
  for compiler in gcc g++; do
    local binary_path
    binary_path="$(command -v "$compiler")"
    if [ "$platform" = WINDOWS ]; then binary_path="$(cygpath -am "$binary_path")"; fi
    "$TRICE_TEST_REPORT_BIN" identity "$binary_path" "${!gcc_key}" || return 2
  done
  # Require the selected C, C++ and clangd frontends to agree on the pinned
  # platform version. An older SDK Clang cannot silently replace this coverage.
  local supported="${!clang_key}" selected_clang="" expected
  case "$platform" in
    DARWIN) supported="$TRICE_CLANG_DARWIN_SUPPORTED" ;;
    LINUX) supported="$TRICE_CLANG_LINUX_SUPPORTED" ;;
  esac
  binary_path="$(command -v clang)"
  if [ "$platform" = WINDOWS ]; then binary_path="$(cygpath -am "$binary_path")"; fi
  for expected in $supported; do
    if "$TRICE_TEST_REPORT_BIN" identity "$binary_path" "$expected" >/dev/null 2>&1; then
      selected_clang="$expected"
      break
    fi
  done
  if [ -z "$selected_clang" ]; then
    printf 'INCOMPLETE: native Clang/SDK must be one of [%s]; found: %s\n' "$supported" "$(clang --version | head -1)"
    return 2
  fi
  for compiler in clang clang++ clangd; do
    binary_path="$(command -v "$compiler")"
    if [ "$platform" = WINDOWS ]; then binary_path="$(cygpath -am "$binary_path")"; fi
    "$TRICE_TEST_REPORT_BIN" identity "$binary_path" "$selected_clang" || return 2
  done
  local arm_bin
  prepare_pinned_test_selection arm_bin bundle temp/tools/bundles arm || return 2
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) arm_bin="$(cygpath -u "$arm_bin")" ;;
  esac
  export PATH="$arm_bin:$PATH"
  for compiler in gcc g++ clang clang++ clangd arm-none-eabi-gcc arm-none-eabi-objcopy arm-none-eabi-size; do
    if ! command -v "$compiler" >/dev/null 2>&1; then
      printf 'MISSING TOOL: %s; native tests require both compiler families and the complete ARM toolchain.\n' "$compiler"
      missing=1
    else
      printf 'Selected %s: %s\n' "$compiler" "$(command -v "$compiler")"
      "$compiler" --version | head -1
    fi
  done
  [ "$missing" -eq 0 ] || return 2
  # CMake needs its bundled modules, not just an executable from a host cache.
  # Select it after compiler PATH changes and before analyzer builds need it.
  export PATH="$cmake_bin:$PATH"
  hash -r
  printf 'Selected cmake: %s\n' "$(command -v cmake)"
  cmake --version || return 2
  prepare_native_cmake_environment || return 2
  ensure_python_test_tool || return 2
  # Parent-shell installers share consent before the formatter child starts.
  ensure_cppcheck_test_tool || return 2
  prepare_pinned_native_tools || return 2
  "$SCRIPTS_DIR/_280_format_c_code.sh" setup || return 2
}

# prepare_pinned_native_tools freezes the executable selection and prepares the
# existing npm tools once. The checked-in lockfile fixes transitive packages;
# an initial lock-generation run stops before running any product tests.
prepare_pinned_native_tools() {
  local tool_bin npm_dir lock_hash installed_hash=""
  ensure_node_test_tool || return 2
  prepare_pinned_test_selection tool_bin tools temp/tools/native \
    goreleaser actionlint shellcheck jq shfmt mdtoc stitchmd || return 2
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) tool_bin="$(cygpath -u "$tool_bin")" ;;
  esac
  export PATH="$tool_bin:$PATH"
  hash -r
  if [ "$(node --version)" != "v$TRICE_NODE_VERSION" ]; then
    printf 'INCOMPLETE: Node.js %s is required; found %s. Select the declared version before running native tests.\n' "$TRICE_NODE_VERSION" "$(node --version)"
    return 2
  fi
  npm_dir="$(pwd)/temp/tools/npm/$TRICE_NODE_VERSION"
  mkdir -p "$npm_dir" || return 2
  cp scripts/testcheck/package.json "$npm_dir/package.json" || return 2
  if [ ! -f scripts/testcheck/package-lock.json ]; then
    approve_pinned_test_setup || return 2
    PUPPETEER_SKIP_DOWNLOAD=true npm install --prefix "$npm_dir" --package-lock-only --ignore-scripts --no-audit --no-fund || return 2
    printf 'INCOMPLETE: generated initial npm lock at %s/package-lock.json; review and add it as scripts/testcheck/package-lock.json before testing.\n' "$npm_dir"
    return 2
  fi
  lock_hash="$(git hash-object --no-filters scripts/testcheck/package-lock.json)" || return 2
  npm_dir="$npm_dir/$lock_hash"
  mkdir -p "$npm_dir" || return 2
  [ ! -f "$npm_dir/.ready" ] || installed_hash="$(cat "$npm_dir/.ready")"
  if [ "$lock_hash" != "$installed_hash" ] || [ ! -x "$npm_dir/node_modules/.bin/markdownlint" ]; then
    approve_pinned_test_setup || return 2
    (
      if ! mkdir "$npm_dir/.install-lock"; then
        printf 'INCOMPLETE: npm setup is locked at %s; inspect the owning preparation process.\n' "$npm_dir"
        exit 2
      fi
      trap 'rmdir "$npm_dir/.install-lock"' EXIT
      cp scripts/testcheck/package.json "$npm_dir/package.json" || exit 2
      cp scripts/testcheck/package-lock.json "$npm_dir/package-lock.json" || exit 2
      PUPPETEER_SKIP_DOWNLOAD=true npm ci --prefix "$npm_dir" --no-audit --no-fund || exit 2
      printf '%s\n' "$lock_hash" >"$npm_dir/.ready" || exit 2
    ) || return 2
  fi
  export PATH="$npm_dir/node_modules/.bin:$PATH"
  export TRICE_TEST_NPM_DIR="$npm_dir"
  node --version
  markdownlint --version
}
