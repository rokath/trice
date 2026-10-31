#!/usr/bin/env bash

# Read-only startup discovery and one consent boundary for test preparation.
# Bash 3.2 is sufficient; no associative arrays, GNU readlink -f or downloads
# are needed to display the plan, including when Go itself is missing.

# Normalize native output before exact version checks and filesystem queries.
trice_tool_path() {
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*) cygpath -u "$1" ;;
    *) printf '%s\n' "$1" ;;
  esac
}

# A semantic version is a complete token, never a substring of another release.
# Go build metadata is needed only for tools whose display version is "dev".
trice_tool_matches() {
  local executable="$1" version="$2" module="${3:-}" output
  output="$("$executable" --version </dev/null 2>&1)" || return 1
  if printf '%s\n' "$output" | grep -Eq "(^|[[:space:]-])v?${version//./\\.}([[:space:]]|$)"; then return 0; fi
  [ -n "$module" ] && command -v go >/dev/null 2>&1 || return 1
  GOTOOLCHAIN=local go version -m "$executable" 2>/dev/null | awk -v module="$module" -v version="v$version" '
    $1 == "mod" && ($2 == module || index(module, $2 "/") == 1) && $3 == version { matched = 1 }
    $1 == "=>" { replaced = 1 }
    END { exit !(matched && !replaced) }'
}

# Resolve symlinks portably before finding adjacent compiler resources.
trice_resolve_tool() {
  local file="$1" target count=0
  while [ -L "$file" ]; do
    count=$((count + 1))
    [ "$count" -le 40 ] || return 1
    target="$(readlink "$file")" || return 1
    case "$target" in /*) file="$target" ;; *) file="$(dirname "$file")/$target" ;; esac
  done
  printf '%s/%s\n' "$(cd "$(dirname "$file")" && pwd -P)" "$(basename "$file")"
}

# Resource checks mirror testcheck's complete-bundle contract. Native compiler
# execution belongs to the tests; these queries never compile a probe program.
trice_bundle_complete() {
  local name="$1" executable="$2" version="$3" bin companion resource
  executable="$(trice_resolve_tool "$executable")" || return 1
  bin="$(dirname "$executable")"
  case "$name" in
    cmake)
      for companion in ctest cpack; do trice_tool_matches "$bin/$companion" "$version" || return 1; done
      resource="$("$executable" --help-module-list 2>/dev/null)" || return 1
      [ -n "$resource" ] || return 1
      ;;
    clang)
      for companion in clang++ clangd; do trice_tool_matches "$bin/$companion" "$version" || return 1; done
      if [ "${TRICE_PLAN_OS:-}" = windows ]; then
        resource="$("$executable" -dumpmachine 2>/dev/null)" || return 1
        case "$resource" in *windows-gnu* | *mingw*) ;; *) return 1 ;; esac
      fi
      resource="$("$executable" -print-resource-dir 2>/dev/null)" || return 1
      resource="$(trice_tool_path "${resource%$'\r'}")" || return 1
      [ -f "$resource/include/stddef.h" ] || return 1
      ;;
    gcc | arm)
      companion=g++
      [ "$name" != arm ] || companion=arm-none-eabi-g++
      trice_tool_matches "$bin/$companion" "$version" || return 1
      if [ "$name" = arm ]; then
        [ -x "$bin/arm-none-eabi-objcopy" ] && [ -x "$bin/arm-none-eabi-size" ] || return 1
        companion=libc.a
      else
        companion=libstdc++.a
      fi
      for companion in -print-libgcc-file-name "-print-file-name=$companion"; do
        resource="$("$executable" "$companion" 2>/dev/null)" || return 1
        resource="$(trice_tool_path "${resource%$'\r'}")" || return 1
        case "$resource" in /*) [ -f "$resource" ] || return 1 ;; *) return 1 ;; esac
      done
      ;;
    *) return 1 ;;
  esac
}

# Hashes identify the existing manifest caches; probing never creates them.
trice_tool_hash() {
  if command -v sha256sum >/dev/null 2>&1; then sha256sum "$1" | awk '{print $1}'; else shasum -a 256 "$1" | awk '{print $1}'; fi
}

# Keep the inventory visible while its caller redirects preparation to tools.log.
trice_plan_print() {
  printf '%s\n' "$*"
  if [ "${TRICE_PLAN_MIRROR_TTY:-0}" = 1 ]; then printf '%s\n' "$*" >/dev/tty; fi
}

# Classify paths before displaying inventory. Native npm returns Windows paths,
# while Bash and compiler discovery use POSIX paths, often relative to the repo.
trice_inventory_is_cache() {
  local candidate
  candidate="$(trice_tool_path "$1")" || return 1
  case "$candidate" in "$(pwd)/temp/tools" | "$(pwd)/temp/tools/"* | temp/tools/* | ./temp/tools/*) return 0 ;; esac
  return 1
}

# Inspect the first host command outside the checkout cache. A cache prepended
# by earlier setup must not be mislabeled as a system installation.
trice_inventory_system() {
  local executable="$1" directory candidate output
  local directories=()
  IFS=: read -r -a directories <<<"$PATH"
  for directory in "${directories[@]}"; do
    [ -n "$directory" ] || continue
    candidate="$directory/$executable"
    [ -x "$candidate" ] && ! trice_inventory_is_cache "$candidate" || continue
    if [ "$executable" = go ]; then
      output="$(GOTOOLCHAIN=local "$candidate" version </dev/null 2>/dev/null)" || {
        printf broken
        return
      }
      printf '%s\n' "$output" | sed -n 's/^go version go\([^ ]*\).*/\1/p'
    else trice_found_version "$candidate"; fi
    return
  done
  printf -- '--'
}

# Report cached versions independently of selection. The chosen pinned version
# takes priority, otherwise show an existing version; presence alone does not
# override the installer's integrity and complete-package checks.
trice_inventory_cache() {
  local name="$1" version="$2" selected="$3" candidate executable="$1" observed='' current
  if trice_inventory_is_cache "$selected"; then
    if [ "$name" = Python ]; then trice_found_version "$selected"; else printf '%s' "$version"; fi
    return
  fi
  case "$name" in
    Go) executable=go ;;
    Node.js) executable=node ;;
    GCC-G++) executable=gcc ;;
    Clang-clangd) executable=clang ;;
    arm) executable=arm-none-eabi-gcc ;;
    Python) executable=python ;;
  esac
  for candidate in temp/tools/native/"$TRICE_PLAN_HOST_DIR"/*/bin/"$executable" \
    temp/tools/native/"$TRICE_PLAN_HOST_DIR"/*/bin/"$executable.exe" \
    temp/tools/bundles/"$name"/"$TRICE_PLAN_HOST_DIR"/*/package/bin/"$executable" \
    temp/tools/bundles/"$name"/"$TRICE_PLAN_HOST_DIR"/*/package/CMake.app/Contents/bin/"$executable" \
    temp/tools/host/*/mingw64/bin/"$executable" \
    temp/tools/go/*/go/bin/"$executable" temp/tools/go/*/go/bin/"$executable.exe" \
    temp/tools/node/*/node*/bin/"$executable" temp/tools/node/*/node*/"$executable.exe" \
    temp/tools/"$name"/*/bin/"$executable" temp/tools/"$name"/*/PFiles/Cppcheck/"$executable.exe" \
    temp/tools/python/*/"$executable.exe" \
    temp/tools/clang-format-*/"$TRICE_PLAN_FORMAT_HOST/clang_format/data/bin/$executable.exe" \
    temp/tools/clang-format-*/"$TRICE_PLAN_FORMAT_HOST"/lib/python*/site-packages/clang_format/data/bin/"$executable" \
    temp/tools/clang-format-*/"$TRICE_PLAN_FORMAT_HOST/Lib/site-packages/clang_format/data/bin/$executable.exe"; do
    [ -x "$candidate" ] || continue
    # Shared globs must not attribute a Python/Node/Go runtime to another tool.
    case "$candidate" in */"$executable" | */"$executable.exe") ;; *) continue ;; esac
    if [ "$name" = Go ]; then
      current="$(GOTOOLCHAIN=local "$candidate" version 2>/dev/null | sed -n 's/^go version go\([^ ]*\).*/\1/p')"
    else current="$(trice_found_version "$candidate")"; fi
    case "$current" in '' | missing | 'cannot execute') continue ;; esac
    if [ "$current" = "$version" ]; then
      printf '%s' "$current"
      return
    fi
    [ -n "$observed" ] || observed="$current"
  done
  printf '%s' "${observed:---}"
}

# Convert internal MiB budgets to the approximate MB/GB labels used in the UI.
# Budget arithmetic and reserve checks retain their original units.
trice_plan_size() {
  awk -v mib="$1" 'BEGIN { if (mib >= 1024) printf "~%.1f GB",mib/1024; else printf "~%.0f MB",mib }'
}

# Keep the human overview separate from the detailed plan and setup budgets.
# Only missing prerequisites are interesting; successful host checks stay quiet.
trice_plan_table() {
  local name version found action selected target installed peak executable host cache location size row installs=''
  local missing=''
  trice_plan_print ''
  printf -v row '%-16s %-13s %-17s %-10s %-8s %8s' Tool Required System Cache Location Size
  trice_plan_print "$row"
  trice_plan_print '-------------------------------------------------------------------------------'
  while IFS=$'\t' read -r name version found action selected target installed peak; do
    # Shared build-cache allowances belong to disk checking, not the tool list.
    case "$name" in Go-build-cache | Go-module-cache) continue ;; esac
    if [ "$version" = prerequisite ] && [ "$action" = reuse ]; then continue; fi
    executable="$name"
    case "$name" in Go) executable=go ;; Node.js) executable=node ;; GCC-G++) executable=gcc ;; Clang-clangd) executable=clang ;; arm) executable=arm-none-eabi-gcc ;; Python) executable=python3 ;; esac
    host="$(trice_inventory_system "$executable")"
    cache="$(trice_inventory_cache "$name" "$version" "$selected")"
    location=System
    size=--
    case "$action" in
      reuse)
        if trice_inventory_is_cache "$selected"; then
          location=Cache
        elif [ "$host" != "$version" ] && [ "$name" != Python ]; then
          # An offline Go toolchain or a versioned system package may coexist
          # with an older default. Show both versions without a path paragraph.
          case "$host" in -- | missing | unknown) host="$version" ;; *) host="$host / $version" ;; esac
        fi
        ;;
      dependency)
        if [ "$host" != "$version" ]; then location=Cache; fi
        ;;
      install)
        location=Cache
        size="$(trice_plan_size "$installed")"
        installs="$installs${installs:+, }$name ($size)"
        ;;
      manual) missing="$missing${missing:+, }$name" ;;
    esac
    case "$name" in
      markdownlint-cli | md-to-pdf | puppeteer)
        if [ -f "$TRICE_PLAN_FILE.npm-inventory" ]; then
          host="$(awk -F '\t' -v name="$name" '$1==name {print $2}' "$TRICE_PLAN_FILE.npm-inventory")"
          cache="$(awk -F '\t' -v name="$name" '$1==name {print $3}' "$TRICE_PLAN_FILE.npm-inventory")"
        else
          host=unknown
          cache=unknown
        fi
        ;;
      WinLibs)
        version='GCC/Clang'
        host=--
        cache=--
        if trice_inventory_is_cache "$selected"; then cache=present; fi
        ;;
    esac
    if [ "$version" = prerequisite ]; then
      version=required
      host=--
      cache=--
    fi
    printf -v row '%-16s %-13s %-17s %-10s %-8s %8s' "$name" "$version" "$host" "$cache" "$location" "$size"
    trice_plan_print "$row"
  done <"$TRICE_PLAN_FILE"
  trice_plan_print ''
  if [ -n "$installs" ]; then
    trice_plan_print "Install: $installs"
  elif [ -z "$missing" ]; then trice_plan_print 'No installation needed.'; fi
  if [ -n "$missing" ]; then trice_plan_print "Missing prerequisite: $missing"; fi
}

# Name the actual host blockers whenever continuation is requested. System
# setup remains available, but a partial checkout install must never start.
trice_plan_can_continue() {
  local missing
  missing="$(awk -F '\t' '$4=="manual" {printf "%s%s",separator,$1; separator=", "}' "$TRICE_PLAN_FILE")"
  if [ -n "$missing" ]; then
    trice_plan_print "Cannot continue: $missing is missing or needs setup."
    trice_plan_print 'Install it yourself or choose u to check supported system upgrades.'
    return 2
  fi
}

# Each row names its actual destination and both installed and peak additional
# space estimates. Shared compiler/npm packages are counted once, not per binary.
trice_plan_row() {
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$1" "$2" "${3:--}" "$4" "${5:--}" "$6" "$7" "$8" >>"$TRICE_PLAN_FILE"
}

# Recheck discovery after consent. A reusable tool must not turn into an
# unbudgeted download if its installation disappears during preparation.
trice_plan_allows_install() {
  [ -f "${TRICE_PLAN_FILE:-}" ] || return 0
  if ! awk -F '\t' -v name="$1" '$1==name && $4=="install" {ok=1} END {exit !ok}' "$TRICE_PLAN_FILE"; then
    printf 'INCOMPLETE: installation of %s was not in the approved plan; rerun startup.\n' "$1" >&2
    return 2
  fi
}

# Inspect the first executable the shell would use. A failing program is visible
# as such rather than being confused with a missing command or a valid version.
trice_found_version() {
  local executable="$1" output
  if ! command -v "$executable" >/dev/null 2>&1; then
    printf missing
    return
  fi
  output="$("$executable" --version </dev/null 2>&1)" || {
    printf 'cannot execute'
    return
  }
  local version
  version="$(printf '%s\n' "$output" | grep -Eo '[0-9]+[.][0-9]+([.][0-9]+)?' | head -1)"
  if [ -n "$version" ]; then printf '%s' "$version"; else printf '%s\n' "$output" | tr -d '\r' | head -1 | tr '\t' ' '; fi
}

# Select a complete installed bundle or a valid worktree cache. This fallback
# also works before the pinned Go executable is available to build testcheck.
trice_find_bundle() {
  local name="$1" executable="$2" version="$3" prefix expected bin_dir directory candidate marker
  local paths=()
  IFS=: read -r -a paths <<<"$PATH"
  for directory in "${paths[@]}"; do
    [ -n "$directory" ] || continue
    candidate="$directory/$executable"
    if trice_tool_matches "$candidate" "$version" && trice_bundle_complete "$name" "$candidate" "$version"; then
      dirname "$(trice_resolve_tool "$candidate")"
      return 0
    fi
    if [ "$name" = clang ] && [ "$TRICE_PLAN_OS" = linux ]; then
      candidate="$directory/$executable-${version%%.*}"
      if trice_tool_matches "$candidate" "$version" && trice_bundle_complete "$name" "$candidate" "$version"; then
        dirname "$(trice_resolve_tool "$candidate")"
        return 0
      fi
    fi
  done
  if [ "$TRICE_PLAN_OS" = darwin ] && command -v brew >/dev/null 2>&1; then
    local formula="$name" prefix
    case "$name" in clang) formula=llvm ;; arm) formula=arm-none-eabi-gcc ;; esac
    prefix="$(HOMEBREW_NO_AUTO_UPDATE=1 brew --prefix "$formula" 2>/dev/null || true)"
    candidate="$prefix/bin/$executable"
    if [ -n "$prefix" ] && trice_tool_matches "$candidate" "$version" && trice_bundle_complete "$name" "$candidate" "$version"; then
      dirname "$(trice_resolve_tool "$candidate")"
      return 0
    fi
  fi
  prefix="TRICE_BUNDLE_$(printf '%s' "$name" | tr '[:lower:]' '[:upper:]')"
  marker="${prefix}_SHA_$TRICE_PLAN_PLATFORM"
  expected="${!marker:-}"
  [ "${#expected}" -eq 64 ] || return 1
  marker="${prefix}_BIN_DIRECTORY_$TRICE_PLAN_PLATFORM"
  bin_dir="${!marker:-bin}"
  directory="temp/tools/bundles/$name/$TRICE_PLAN_HOST_DIR/${expected:0:16}"
  [ -f "$directory/.ready" ] && [ "$(cat "$directory/.ready")" = "$expected" ] || return 1
  candidate="$directory/package/$bin_dir/$executable"
  trice_tool_matches "$candidate" "$version" && trice_bundle_complete "$name" "$candidate" "$version" || return 1
  printf '%s\n' "$directory/package/$bin_dir"
}

# Package managers expose genuine GCC under gcc-N/g++-N without replacing the
# host's default compiler. Inspect those resources before planning a download.
trice_find_versioned_gcc() {
  local version="$1" executable companion resource candidates prefix
  candidates="$(command -v "gcc-${version%%.*}" || true)"
  if [ "${TRICE_PLAN_OS:-}" = darwin ] && command -v brew >/dev/null 2>&1; then
    prefix="$(HOMEBREW_NO_AUTO_UPDATE=1 brew --prefix gcc 2>/dev/null || true)"
    [ -z "$prefix" ] || candidates="$candidates
$prefix/bin/gcc-${version%%.*}"
  fi
  while IFS= read -r executable; do
    [ -n "$executable" ] || continue
    companion="$(dirname "$executable")/g++-${version%%.*}"
    trice_tool_matches "$executable" "$version" && trice_tool_matches "$companion" "$version" || continue
    for resource in "$("$executable" -print-libgcc-file-name 2>/dev/null)" "$("$executable" -print-file-name=libstdc++.a 2>/dev/null)"; do
      resource="$(trice_tool_path "${resource%$'\r'}")" || continue 2
      [ -f "$resource" ] || continue 2
    done
    printf '%s\n' "$executable"
    return 0
  done <<<"$candidates"
  return 1
}

# Plan compiler packages with the same resource checks used by selection.
trice_plan_bundle() {
  local name="$1" executable="$2" version="$3" installed="$4" peak="$5" selected found
  found="$(trice_found_version "$executable")"
  if selected="$(trice_find_bundle "$name" "$executable" "$version")"; then
    trice_plan_row "$name" "$version" "$found" reuse "$selected" temp/tools 0 0
  elif [ "$name" = gcc ] && selected="$(trice_find_versioned_gcc "$version")"; then
    trice_plan_row "$name" "$version" "$version (versioned GCC)" reuse "$selected" temp/tools 0 0
  else
    local key method='complete release package'
    key="TRICE_BUNDLE_$(printf '%s' "$name" | tr '[:lower:]' '[:upper:]')_URL_$TRICE_PLAN_PLATFORM"
    if [ -z "${!key:-}" ]; then
      trice_plan_row "$name" "$version" "$found" manual 'no verified package for this OS/architecture' temp/tools 0 0
      return
    fi
    key="TRICE_BUNDLE_$(printf '%s' "$name" | tr '[:lower:]' '[:upper:]')_BUILD_$TRICE_PLAN_PLATFORM"
    [ -z "${!key:-}" ] || method='LLVM source build'
    trice_plan_row "$name" "$version" "$found" install "$method" temp/tools "$installed" "$peak"
  fi
}

# Verify old cache entries without changing their directory or metadata. A
# source-built tool needs the exact Go main package/tag; a release executable
# needs its retained archive to match the current publisher checksum.
trice_find_cli_cache() {
  local name="$1" version="$2" module="$3" prefix="$4" filename current candidate digest directory go_command metadata url key expected archive
  filename="$name"
  [ "$TRICE_PLAN_OS" != windows ] || filename="$filename.exe"
  current="temp/tools/native/$TRICE_PLAN_HOST_DIR/$TRICE_PLAN_HASH/bin"
  for candidate in "$current/$filename" temp/tools/native/"$TRICE_PLAN_HOST_DIR"/*/bin/"$filename"; do
    [ -f "$candidate.sha256" ] || continue
    digest="$(trice_tool_hash "$candidate" 2>/dev/null || true)"
    [ -n "$digest" ] && [ "$(cat "$candidate.sha256")" = "$digest" ] && trice_tool_matches "$candidate" "$version" "$module" || continue
    directory="$(dirname "$candidate")"
    if [ "$directory" = "$current" ]; then
      printf '%s\n' "$candidate"
      return 0
    fi
    go_command="${TRICE_PLAN_GO_BIN:-$(command -v go || true)}"
    if [ -n "$module" ] && [ -n "$go_command" ]; then
      metadata="$(GOTOOLCHAIN=local GOPROXY=off "$go_command" version -m "$candidate" 2>/dev/null || true)"
      if printf '%s\n' "$metadata" | awk -v package="$module" -v version="v$version" '
        $1=="path" {package_ok=($2==package)}
        $1=="mod" {module_ok=(($2==package || index(package,$2 "/")==1) && $3==version)}
        $1=="=>" {replaced=1}
        END {exit !(package_ok && module_ok && !replaced)}'; then
        printf '%s\n' "$candidate"
        return 0
      fi
    fi
    key="${prefix}_URL_$TRICE_PLAN_PLATFORM"
    url="${!key:-}"
    key="${prefix}_SHA_$TRICE_PLAN_PLATFORM"
    expected="${!key:-}"
    archive="$(dirname "$directory")/${url##*/}"
    if [ -n "$url" ] && [ "${#expected}" -eq 64 ] && [ -f "$archive" ] && [ "$(trice_tool_hash "$archive")" = "$expected" ]; then
      printf '%s\n' "$candidate"
      return 0
    fi
  done
  return 1
}

# A working launcher stays in place; no hardlink, symlink or executable copy is
# needed. Cached standalone binaries retain their original digest validation.
trice_plan_cli() {
  local name="$1" key version module candidate digest found method=release installed=32 peak=64
  case "$name" in goreleaser)
    installed=128
    peak=256
    ;;
  shellcheck)
    installed=64
    peak=128
    ;;
  esac
  key="TRICE_TOOL_$(printf '%s' "$name" | tr '[:lower:]-' '[:upper:]_')"
  version="${key}_VERSION"
  version="${!version:?}"
  module="${key}_MODULE"
  module="${!module:-}"
  found="$(trice_found_version "$name")"
  candidate="$(command -v "$name" || true)"
  if [ -n "$candidate" ] && trice_tool_matches "$candidate" "$version" "$module"; then
    trice_plan_row "$name" "$version" "$found" reuse "$candidate" temp/tools 0 0
    return
  fi
  if candidate="$(trice_find_cli_cache "$name" "$version" "$module" "$key")"; then
    trice_plan_row "$name" "$version" "$found" reuse "$candidate" temp/tools 0 0
    return
  fi
  key="${key}_URL_$TRICE_PLAN_PLATFORM"
  if [ -z "${!key:-}" ]; then
    if [ -z "$module" ]; then
      trice_plan_row "$name" "$version" "$found" manual 'no verified package for this OS/architecture' temp/tools 0 0
      return
    fi
    method='source build'
    installed=64
    peak=768
  fi
  trice_plan_row "$name" "$version" "$found" install "$method" temp/tools "$installed" "$peak"
}

# Match Go offline, including a previously downloaded launcher toolchain. An
# unavailable toolchain is planned as an official worktree-local binary package.
trice_find_go() {
  local root candidate
  if root="$(GOTOOLCHAIN="go$TRICE_GO_VERSION" GOPROXY=off go env GOROOT 2>/dev/null)"; then
    root="$(trice_tool_path "${root%$'\r'}")" || return 1
    candidate="$root/bin/go"
    if [ "$(GOTOOLCHAIN=local "$candidate" version 2>/dev/null)" = "go version go$TRICE_GO_VERSION $TRICE_PLAN_HOST" ]; then
      printf '%s\n' "$candidate"
      return
    fi
  fi
  # Distribution packages and Homebrew can coexist with an older default Go.
  if [ "$TRICE_PLAN_OS" = linux ] && command -v dpkg >/dev/null 2>&1; then
    while IFS= read -r candidate; do
      if [ "$(GOTOOLCHAIN=local "$candidate" version 2>/dev/null)" = "go version go$TRICE_GO_VERSION $TRICE_PLAN_HOST" ]; then
        printf '%s\n' "$candidate"
        return 0
      fi
    done < <(dpkg -L "golang-${TRICE_GO_VERSION%.*}-go" 2>/dev/null | grep '/bin/go$')
  elif [ "$TRICE_PLAN_OS" = darwin ] && command -v brew >/dev/null 2>&1; then
    root="$(HOMEBREW_NO_AUTO_UPDATE=1 brew --prefix go 2>/dev/null || true)"
    candidate="$root/bin/go"
    if [ -n "$root" ] && [ "$(GOTOOLCHAIN=local "$candidate" version 2>/dev/null)" = "go version go$TRICE_GO_VERSION $TRICE_PLAN_HOST" ]; then
      printf '%s\n' "$candidate"
      return 0
    fi
  fi
  candidate="temp/tools/go/$TRICE_GO_VERSION/go/bin/go"
  [ "$TRICE_PLAN_OS" != windows ] || candidate="$candidate.exe"
  GOTOOLCHAIN=local "$candidate" version 2>/dev/null | grep -Fq "go$TRICE_GO_VERSION $TRICE_PLAN_HOST" || return 1
  printf '%s\n' "$candidate"
}

# Node's complete archive supplies npm/npx. A cache or system executable is not
# enough when its adjacent npm distribution is missing.
trice_find_node() {
  local candidate
  candidate="$(command -v node || true)"
  if [ -n "$candidate" ] && trice_tool_matches "$candidate" "$TRICE_NODE_VERSION" && command -v npm >/dev/null 2>&1 && command -v npx >/dev/null 2>&1; then
    printf '%s\n' "$candidate"
    return
  fi
  if [ "${TRICE_PLAN_OS:-}" = darwin ] && command -v brew >/dev/null 2>&1; then
    local prefix
    prefix="$(HOMEBREW_NO_AUTO_UPDATE=1 brew --prefix node 2>/dev/null || true)"
    candidate="$prefix/bin/node"
    if [ -n "$prefix" ] && trice_tool_matches "$candidate" "$TRICE_NODE_VERSION" && [ -f "$prefix/bin/npm" ] && [ -f "$prefix/bin/npx" ]; then
      printf '%s\n' "$candidate"
      return
    fi
  fi
  for candidate in temp/tools/node/"$TRICE_NODE_VERSION"/node*/bin/node temp/tools/node/"$TRICE_NODE_VERSION"/node*/node.exe; do
    [ -x "$candidate" ] || continue
    if trice_tool_matches "$candidate" "$TRICE_NODE_VERSION" && [ -f "$(dirname "$candidate")/npm" ] && [ -f "$(dirname "$candidate")/npx" ]; then
      printf '%s\n' "$candidate"
      return
    fi
  done
  return 1
}

# Npm probes use Bash to launch npm (including Windows .cmd shims), and Node
# only to inspect module graphs. No npm package or browser is downloaded here.
trice_plan_npm() {
  # Discovery and installation must use the same browser cache in every process.
  export PUPPETEER_CACHE_DIR="${PUPPETEER_CACHE_DIR:-$(pwd)/temp/tools/puppeteer}"
  local global_root='' cache_root name version found action selected fallback='' renderer_root='' probe_node
  TRICE_PLAN_NPM_LOCK="$(git hash-object --no-filters scripts/testcheck/package-lock.json)" || return 2
  cache_root="$(pwd)/temp/tools/npm/$TRICE_NODE_VERSION/$TRICE_PLAN_NPM_LOCK"
  if [ -z "${CI:-}" ] || [ "${CI:-}" = false ]; then
    global_root="$(npm root -g 2>/dev/null || true)"
    global_root="${global_root%$'\r'}"
    [ -z "$global_root" ] || global_root="$(dirname "$(trice_tool_path "$global_root")")"
  fi
  probe_node="${TRICE_PLAN_NODE:-$(command -v node || true)}"
  if [ -z "$probe_node" ] || ! "$probe_node" scripts/_test_npm_tools.cjs inspect "$global_root" "$cache_root" "$TRICE_TOOL_MARKDOWNLINT_VERSION" "$TRICE_TOOL_MD_TO_PDF_VERSION" "$TRICE_TOOL_PUPPETEER_VERSION" "$TRICE_PLAN_FILE.npm-inventory" >"$TRICE_PLAN_FILE.npm"; then
    printf 'markdownlint-cli\t%s\tunverified without usable Node\tinstall\t-\nmd-to-pdf\t%s\tunverified without usable Node\tinstall\t-\npuppeteer\t%s\tunverified without usable Node\tinstall\t-\n' "$TRICE_TOOL_MARKDOWNLINT_VERSION" "$TRICE_TOOL_MD_TO_PDF_VERSION" "$TRICE_TOOL_PUPPETEER_VERSION" >"$TRICE_PLAN_FILE.npm"
  fi
  while IFS=$'\t' read -r name version found action selected; do
    if [ "${TRICE_PLAN_SCOPE:-native}" = pdf ] && [ "$name" = markdownlint-cli ]; then continue; fi
    if [ "$action" = reuse ]; then
      trice_plan_row "$name" "$version" "$found" reuse "$selected/node_modules/$name" temp/tools 0 0
      [ "$name" != md-to-pdf ] || renderer_root="$selected"
    else
      # The renderer's graph includes Puppeteer. Count its shared dependency
      # closure once; markdownlint can be installed independently.
      case "$name" in markdownlint-cli) trice_plan_row "$name" "$version" "$found" install 'locked npm graph' temp/tools 24 80 ;;
      md-to-pdf) trice_plan_row "$name" "$version" "$found" install 'locked renderer + Puppeteer' temp/tools 180 500 ;;
      puppeteer) trice_plan_row "$name" "$version" "$found" dependency 'included in renderer graph' temp/tools 0 0 ;; esac
      fallback="$fallback${fallback:+ }$name"
    fi
  done <"$TRICE_PLAN_FILE.npm"
  if [ "${TRICE_PLAN_SCOPE:-native}" = pdf ]; then
    if [ -n "$renderer_root" ] && [ -n "$probe_node" ] && "$probe_node" scripts/_test_npm_tools.cjs browser "$renderer_root" >"$TRICE_PLAN_FILE.browser" 2>/dev/null; then
      trice_plan_row Puppeteer-browser 'matching library revision' available reuse "$(cat "$TRICE_PLAN_FILE.browser")" temp/tools 0 0
    else
      trice_plan_row Puppeteer-browser 'matching library revision' 'missing or incompatible' install 'pinned Chrome release binaries' "${PUPPETEER_CACHE_DIR:-temp/tools/puppeteer}" 900 1800
    fi
  fi
  export TRICE_TEST_NPM_MISSING="$fallback" TRICE_TEST_NPM_PLAN="$TRICE_PLAN_FILE.npm"
}

# Install only the missing npm roots and their dependency closure. Local runs
# can reuse independently installed markdownlint or the complete PDF pair;
# CI deliberately installs the original, complete lockfile without rewriting it.
prepare_selected_npm_tools() {
  local name version found action selected cache_root subset names='' global_root='' prefix renderer_root='' markdown_root='' lock owner
  [ -f "${TRICE_TEST_NPM_PLAN:-}" ] || {
    printf 'INCOMPLETE: npm setup requires the startup tool plan.\n' >&2
    return 2
  }
  lock="$(git hash-object --no-filters scripts/testcheck/package-lock.json)" || return 2
  cache_root="$(pwd)/temp/tools/npm/$TRICE_NODE_VERSION/$lock"
  if [ -z "${CI:-}" ] || [ "${CI:-}" = false ]; then
    global_root="$(npm root -g 2>/dev/null || true)"
    global_root="${global_root%$'\r'}"
    [ -z "$global_root" ] || global_root="$(dirname "$(trice_tool_path "$global_root")")"
  fi
  # Node may itself have needed setup. Recheck before installing any npm root;
  # an initially unverifiable global installation must not be duplicated.
  node scripts/_test_npm_tools.cjs inspect "$global_root" "$cache_root" "$TRICE_TOOL_MARKDOWNLINT_VERSION" "$TRICE_TOOL_MD_TO_PDF_VERSION" "$TRICE_TOOL_PUPPETEER_VERSION" >"$TRICE_TEST_NPM_PLAN.rechecked" || return 2
  while IFS=$'\t' read -r name version found action selected; do
    if [ "${TRICE_PLAN_SCOPE:-native}" = pdf ] && [ "$name" = markdownlint-cli ]; then continue; fi
    if [ "$action" = install ]; then
      if awk -F '\t' -v name="$name" '$1==name && $4=="reuse" {found=1} END {exit !found}' "$TRICE_TEST_NPM_PLAN"; then
        printf 'INCOMPLETE: npm package %s changed since startup; rerun the plan before installing it.\n' "$name" >&2
        return 2
      fi
      names="$names${names:+ }$name"
    fi
  done <"$TRICE_TEST_NPM_PLAN.rechecked"
  if [ -n "$names" ]; then
    if [ -n "${CI:-}" ] && [ "${CI:-}" != false ]; then
      subset="$cache_root"
    else
      subset="$cache_root/subset-$(printf '%s' "$names" | git hash-object --stdin)"
    fi
    mkdir -p "$subset" || return 2
    if ! mkdir "$subset/.install-lock"; then
      printf 'INCOMPLETE: npm setup is locked at %s/.install-lock.\n' "$subset" >&2
      return 2
    fi
    # This subshell owns its lock even when npm fails. It never alters another
    # installation or runs Puppeteer's browser download implicitly.
    (
      trap 'rmdir "$subset/.install-lock"' EXIT
      if [ -n "${CI:-}" ] && [ "${CI:-}" != false ]; then
        cp scripts/testcheck/package.json scripts/testcheck/package-lock.json "$subset/" || exit 2
      else
        # Tool names are fixed manifest keys, not user-supplied shell syntax.
        # shellcheck disable=SC2086
        node scripts/_test_npm_tools.cjs lock scripts/testcheck/package-lock.json "$subset" $names || exit 2
      fi
      PUPPETEER_SKIP_DOWNLOAD=true trice_setup_with_progress "Installing npm tools ($names) into temp/tools" npm ci --prefix "$subset" --cache "$(pwd)/temp/tools/npm-cache" --ignore-scripts --no-audit --no-fund || exit 2
      printf '%s\n' "$lock" >"$subset/.ready" || exit 2
    ) || return 2
  fi
  if [ -z "${CI:-}" ] || [ "${CI:-}" = false ]; then
    global_root="$(npm root -g 2>/dev/null || true)"
    global_root="${global_root%$'\r'}"
    [ -z "$global_root" ] || global_root="$(dirname "$(trice_tool_path "$global_root")")"
  fi
  node scripts/_test_npm_tools.cjs inspect "$global_root" "$cache_root" "$TRICE_TOOL_MARKDOWNLINT_VERSION" "$TRICE_TOOL_MD_TO_PDF_VERSION" "$TRICE_TOOL_PUPPETEER_VERSION" >"$TRICE_TEST_NPM_PLAN.selected" || return 2
  while IFS=$'\t' read -r name version found action selected; do
    if [ "${TRICE_PLAN_SCOPE:-native}" = pdf ] && [ "$name" = markdownlint-cli ]; then continue; fi
    if [ "$action" != reuse ]; then
      printf 'INCOMPLETE: prepared npm selection cannot provide %s %s.\n' "$name" "$version" >&2
      return 2
    fi
    selected="$(trice_tool_path "$selected")" || return 2
    case "$name" in markdownlint-cli) markdown_root="$selected" ;; md-to-pdf) renderer_root="$selected" ;; esac
  done <"$TRICE_TEST_NPM_PLAN.selected"
  prefix="$(npm prefix -g 2>/dev/null || true)"
  prefix="${prefix%$'\r'}"
  if [ -n "$prefix" ]; then
    prefix="$(trice_tool_path "$prefix")" || return 2
    [ "$TRICE_PLAN_OS" = windows ] || prefix="$prefix/bin"
    PATH="$prefix:$PATH"
  fi
  # Local .bin directories exist for fallback packages. Global npm launchers
  # live in npm's prefix instead, so both layouts stay usable on Windows.
  [ ! -d "$markdown_root/node_modules/.bin" ] || PATH="$markdown_root/node_modules/.bin:$PATH"
  [ ! -d "$renderer_root/node_modules/.bin" ] || PATH="$renderer_root/node_modules/.bin:$PATH"
  export PATH
  hash -r
  TRICE_TEST_NPM_DIR="$renderer_root"
  case "$TRICE_PLAN_OS" in
    windows)
      TRICE_TEST_NPM_DIR="$(cygpath -am "$renderer_root")"
      NODE_PATH="$(cygpath -am "$renderer_root/node_modules")${NODE_PATH:+;$NODE_PATH}"
      ;;
    *) NODE_PATH="$renderer_root/node_modules${NODE_PATH:+:$NODE_PATH}" ;;
  esac
  export TRICE_TEST_NPM_DIR NODE_PATH
  TRICE_TEST_PDF_CLI="$(node scripts/_test_npm_tools.cjs cli "$renderer_root" md-to-pdf)" || return 2
  export TRICE_TEST_PDF_CLI
  if [ "${TRICE_PLAN_SCOPE:-native}" != pdf ]; then
    trice_tool_matches markdownlint "$TRICE_TOOL_MARKDOWNLINT_VERSION" || {
      printf 'INCOMPLETE: selected markdownlint launcher has the wrong version.\n' >&2
      return 2
    }
  fi
  if [ "${TRICE_PLAN_SCOPE:-native}" = pdf ] && ! node scripts/_test_npm_tools.cjs browser "$renderer_root" >/dev/null 2>&1; then
    export PUPPETEER_CACHE_DIR="${PUPPETEER_CACHE_DIR:-$(pwd)/temp/tools/puppeteer}"
    owner="$(node scripts/_test_npm_tools.cjs puppeteer-root "$renderer_root")" || return 2
    PUPPETEER_SKIP_DOWNLOAD=false trice_setup_with_progress 'Installing Puppeteer browser into temp/tools' node "$owner/install.mjs" || return 2
    node scripts/_test_npm_tools.cjs browser "$renderer_root" || return 2
  fi
}

# Check each destination's nearest existing directory. Group by df's device so
# worktree caches, separate Go caches and browser caches cannot borrow free space
# from another filesystem. Peak estimates deliberately include archive/staging.
trice_plan_space() {
  local name version found action selected target installed peak directory device available reserve
  local groups="$TRICE_PLAN_FILE.space" devices="$TRICE_PLAN_FILE.devices" required free failed=0
  reserve="${TRICE_TEST_DISK_RESERVE_MIB:-1024}"
  case "$reserve" in '' | *[!0-9]*)
    trice_plan_print 'INCOMPLETE: TRICE_TEST_DISK_RESERVE_MIB must be a nonnegative integer.'
    return 2
    ;;
  esac
  if [ "${#reserve}" -gt 9 ]; then
    trice_plan_print 'INCOMPLETE: disk reserve must be at most 999999999 MiB.'
    return 2
  fi
  reserve=$((10#$reserve))
  : >"$groups"
  while IFS=$'\t' read -r name version found action selected target installed peak; do
    [ "$action" = install ] || continue
    directory="$target"
    while [ ! -d "$directory" ]; do directory="$(dirname "$directory")"; done
    # POSIX -P output is available in GNU df, BSD df and Git Bash.
    device="$(df -Pk "$directory" 2>/dev/null | awk 'END {print $1}')"
    available="$(df -Pk "$directory" 2>/dev/null | awk 'END {print $4}')"
    case "$available" in '' | *[!0-9]*)
      trice_plan_print "INCOMPLETE: cannot measure free space for $target"
      return 2
      ;;
    esac
    printf '%s\t%s\t%s\t%s\n' "$device" "$((available / 1024))" "$peak" "$target" >>"$groups"
  done <"$TRICE_PLAN_FILE"
  # Keep staging and reserve details in the plan sidecars. Normal output needs
  # only free capacity and whether preparation fits; failures name the shortfall.
  if [ -s "$groups" ]; then
    awk -F '\t' '{sum[$1]+=$3; free[$1]=$2; target[$1]=$4} END {for(d in sum) printf "%s\t%d\t%d\t%s\n",d,sum[d],free[d],target[d]}' "$groups" >"$devices"
    while IFS=$'\t' read -r device required free target; do
      if [ "$free" -lt "$((required + reserve))" ]; then
        trice_plan_print "Disk [$device]: $(trice_plan_size "$free") free; insufficient."
        trice_plan_print "Free another $(trice_plan_size "$((required + reserve - free))") before setup."
        failed=1
      else trice_plan_print "Disk [$device]: $(trice_plan_size "$free") free - sufficient."; fi
    done <"$devices"
  else
    trice_plan_print "Disk: $(trice_plan_size "$(df -Pk . | awk 'END {printf "%d",$4/1024}')") free."
  fi
  [ "$failed" -eq 0 ]
}

# Display every selection before asking once. Continue authorizes checkout-local
# preparation; update additionally offers confirmed, version-bounded upgrades.
# An internal recheck after updates retains consent for remaining local tools.
trice_test_tool_plan() {
  local scope="${1:-native}" os architecture selected found tool version action target installed peak answer='' blocked=0 checkout_space_ok=1 system_status
  local after_updates="${2:-0}"
  # Exit 2 also describes real setup failures. Keep explicit refusal distinct
  # so the suite can avoid replaying an inventory already mirrored to the user.
  export TRICE_TEST_SETUP_CANCELLED=0
  # Capture interactive output once, before table and disk loops redirect stdin.
  local TRICE_PLAN_MIRROR_TTY=0
  if [ ! -t 1 ] && [ -t 0 ] && { true >/dev/tty; } 2>/dev/null; then TRICE_PLAN_MIRROR_TTY=1; fi
  # Later installers inherit this terminal selection after planning returns.
  export TRICE_TEST_PROGRESS_TTY="$TRICE_PLAN_MIRROR_TTY"
  case " ${TRICE_TEST_PLAN_SCOPES:-} " in *" $scope "*) return 0 ;; esac
  TRICE_PLAN_SCOPE="$scope"
  os="$(uname -s)"
  architecture="$(uname -m)"
  TRICE_PLAN_FORMAT_HOST="$os-$architecture"
  case "$os" in Linux*) TRICE_PLAN_OS=linux ;; Darwin*) TRICE_PLAN_OS=darwin ;; MINGW* | MSYS* | CYGWIN*) TRICE_PLAN_OS=windows ;; *)
    trice_plan_print "INCOMPLETE: unsupported native OS $os"
    return 2
    ;;
  esac
  case "$architecture" in x86_64 | amd64) architecture=amd64 ;; arm64 | aarch64) architecture=arm64 ;; *)
    trice_plan_print "INCOMPLETE: unsupported native architecture $architecture"
    return 2
    ;;
  esac
  TRICE_PLAN_HOST="$TRICE_PLAN_OS/$architecture"
  [ "$TRICE_PLAN_OS" != windows ] || TRICE_PLAN_FORMAT_HOST="windows-$(uname -m)"
  TRICE_PLAN_PLATFORM="$(printf '%s_%s' "$TRICE_PLAN_OS" "$architecture" | tr '[:lower:]' '[:upper:]')"
  # Existing cache paths use a hyphen, while Go's version output uses a slash.
  TRICE_PLAN_HOST_DIR="$TRICE_PLAN_OS-$architecture"
  mkdir -p "${LOG_DIR:-temp/log}" || return 2
  TRICE_PLAN_FILE="$(mktemp "${LOG_DIR:-temp/log}/tool-plan.XXXXXX")" || return 2
  TRICE_PLAN_HASH="$(trice_tool_hash scripts/test-tools.env)" || return 2
  TRICE_PLAN_HASH="${TRICE_PLAN_HASH:0:16}"
  local prerequisites='git bash tar unzip curl'
  [ "$scope" != native ] || prerequisites="$prerequisites make"
  for tool in $prerequisites; do
    if command -v "$tool" >/dev/null 2>&1; then
      trice_plan_row "$tool" prerequisite available reuse "$(command -v "$tool")" temp/tools 0 0
    else
      trice_plan_row "$tool" prerequisite missing manual 'install in the host environment' temp/tools 0 0
      blocked=1
    fi
  done
  case "$scope" in native | lint | pdf | cli* | analyzer)
    selected="$(trice_find_go || true)"
    TRICE_PLAN_GO_BIN="$selected"
    found="$(GOTOOLCHAIN=local go version 2>/dev/null || printf missing)"
    if [ -n "$selected" ]; then
      trice_plan_row Go "$TRICE_GO_VERSION" "$found" reuse "$selected" temp/tools 0 0
    else trice_plan_row Go "$TRICE_GO_VERSION" "$found" install 'official Go binary archive' temp/tools 300 500; fi
    ;;
  esac
  case "$scope" in native | lint | pdf | node)
    TRICE_PLAN_NODE="$(trice_find_node || true)"
    if [ -n "$TRICE_PLAN_NODE" ]; then
      trice_plan_row Node.js "$TRICE_NODE_VERSION" "$(trice_found_version node)" reuse "$TRICE_PLAN_NODE" temp/tools 0 0
    else trice_plan_row Node.js "$TRICE_NODE_VERSION" "$(trice_found_version node)" install 'official Node/npm archive' temp/tools 180 300; fi
    ;;
  esac
  if [ "$scope" = native ]; then
    trice_plan_bundle cmake cmake "$TRICE_CMAKE_VERSION" 220 500
    case "$TRICE_PLAN_OS" in
      linux)
        trice_plan_bundle gcc gcc "$TRICE_GCC_LINUX_VERSION" 500 1200
        trice_plan_bundle clang clang "$TRICE_CLANG_LINUX_VERSION" 12500 16000
        ;;
      darwin)
        if ! xcrun --show-sdk-path >/dev/null 2>&1; then
          trice_plan_row macOS-SDK required missing manual 'install Xcode Command Line Tools' temp/tools 0 0
          blocked=1
        fi
        if trice_tool_matches gcc "$TRICE_GCC_DARWIN_VERSION" && trice_bundle_complete gcc "$(command -v gcc)" "$TRICE_GCC_DARWIN_VERSION"; then
          trice_plan_row gcc "$TRICE_GCC_DARWIN_VERSION" "$(trice_found_version gcc)" reuse "$(command -v gcc)" temp/tools 0 0
        elif selected="$(trice_find_versioned_gcc "$TRICE_GCC_DARWIN_VERSION")"; then
          trice_plan_row gcc "$TRICE_GCC_DARWIN_VERSION" "$TRICE_GCC_DARWIN_VERSION" reuse "$selected" temp/tools 0 0
        else
          trice_plan_row gcc "$TRICE_GCC_DARWIN_VERSION" "$(trice_found_version gcc)" manual 'install matching native GCC' temp/tools 0 0
          blocked=1
        fi
        if [ "$architecture" = amd64 ]; then
          trice_plan_bundle clang clang "$TRICE_CLANG_DARWIN_VERSION" 2500 32000
        else trice_plan_bundle clang clang "$TRICE_CLANG_DARWIN_VERSION" 4500 6500; fi
        ;;
      windows)
        trice_plan_row GCC-G++ "$TRICE_GCC_WINDOWS_VERSION" "$(trice_found_version gcc)" dependency 'native GCC selection in the WinLibs row' temp/tools 0 0
        trice_plan_row Clang-clangd "$TRICE_CLANG_WINDOWS_VERSION" "$(trice_found_version clang)" dependency 'native Clang selection in the WinLibs row' temp/tools 0 0
        TRICE_PLAN_WINDOWS_GCC="$(trice_find_bundle gcc gcc "$TRICE_GCC_WINDOWS_VERSION" || true)"
        TRICE_PLAN_WINDOWS_CLANG="$(trice_find_bundle clang clang "$TRICE_CLANG_WINDOWS_VERSION" || true)"
        if [ -n "$TRICE_PLAN_WINDOWS_GCC" ] && [ -n "$TRICE_PLAN_WINDOWS_CLANG" ]; then
          trice_plan_row WinLibs "$TRICE_GCC_WINDOWS_VERSION/$TRICE_CLANG_WINDOWS_VERSION" 'matching GCC/Clang' reuse 'complete native compiler installations' temp/tools 0 0
        elif select_windows_host_test_package && trice_tool_matches "$TRICE_HOST_PACKAGE_BIN/gcc" "$TRICE_GCC_WINDOWS_VERSION" && trice_bundle_complete gcc "$TRICE_HOST_PACKAGE_BIN/gcc" "$TRICE_GCC_WINDOWS_VERSION" && trice_tool_matches "$TRICE_HOST_PACKAGE_BIN/clang" "$TRICE_CLANG_WINDOWS_VERSION" && trice_bundle_complete clang "$TRICE_HOST_PACKAGE_BIN/clang" "$TRICE_CLANG_WINDOWS_VERSION"; then
          trice_plan_row WinLibs "$TRICE_GCC_WINDOWS_VERSION/$TRICE_CLANG_WINDOWS_VERSION" 'mismatch in PATH' reuse "$TRICE_HOST_PACKAGE_BIN" temp/tools 0 0
        else trice_plan_row WinLibs "$TRICE_GCC_WINDOWS_VERSION/$TRICE_CLANG_WINDOWS_VERSION" 'missing or incomplete' install 'shared GCC/Clang runtime release' temp/tools 4000 5000; fi
        ;;
    esac
    trice_plan_bundle arm arm-none-eabi-gcc "$TRICE_ARM_GCC_VERSION" 1600 2300
    for tool in goreleaser actionlint shellcheck jq shfmt mdtoc stitchmd; do trice_plan_cli "$tool"; done
  fi
  if [ "$scope" = lint ]; then
    for tool in goreleaser actionlint shellcheck jq shfmt mdtoc stitchmd; do trice_plan_cli "$tool"; done
  fi
  local packages=''
  case "$scope" in native) packages='cppcheck clang-format' ;; format) packages=clang-format ;; analyzer)
    packages=cppcheck
    trice_plan_bundle cmake cmake "$TRICE_CMAKE_VERSION" 220 500
    ;;
  esac
  if [ -n "$packages" ]; then
    for tool in $packages; do
      version="TRICE_TOOL_$(printf '%s' "$tool" | tr '[:lower:]-' '[:upper:]_')_VERSION"
      version="${!version:?}"
      selected="$(command -v "$tool" || true)"
      if [ "$tool" = clang-format ] && [ -n "${TRICE_HOST_PACKAGE_BIN:-}" ] && [ -z "${CLANG_FORMAT_BIN:-}" ] && trice_tool_matches "$TRICE_HOST_PACKAGE_BIN/clang-format" "$version"; then selected="$TRICE_HOST_PACKAGE_BIN/clang-format"; fi
      if [ "$tool" = clang-format ] && ! trice_tool_matches "${selected:-clang-format}" "$version" && [ -z "${CLANG_FORMAT_BIN:-}" ]; then
        target="$(command -v "clang-format-${version%%.*}" || true)"
        if [ -n "$target" ] && trice_tool_matches "$target" "$version"; then selected="$target"; fi
      fi
      if [ "$tool" = cppcheck ] && ! trice_tool_matches "${selected:-cppcheck}" "$version"; then
        target="$(cppcheck_system_test_tool || true)"
        if trice_tool_matches "$target" "$version"; then selected="$target"; fi
        if ! trice_tool_matches "${selected:-cppcheck}" "$version"; then
          target="$(cppcheck_host_cache_test_tool || true)"
          [ -z "$target" ] || selected="$target"
        fi
      fi
      if [ "$tool" = clang-format ] && [ -n "${CLANG_FORMAT_BIN:-}" ]; then selected="$CLANG_FORMAT_BIN"; fi
      if [ -n "$selected" ] && trice_tool_matches "$selected" "$version"; then
        trice_plan_row "$tool" "$version" "$(trice_found_version "$selected")" reuse "$selected" temp/tools 0 0
      else
        selected=''
        for target in "temp/tools/$tool/$version/bin/$tool" "temp/tools/$tool/$version/PFiles/Cppcheck/$tool.exe" \
          temp/tools/clang-format-"$version"/*/clang_format/data/bin/clang-format \
          temp/tools/clang-format-"$version"/*/clang_format/data/bin/clang-format.exe \
          temp/tools/clang-format-"$version"/*/lib/python*/site-packages/clang_format/data/bin/clang-format \
          temp/tools/clang-format-"$version"/*/Lib/site-packages/clang_format/data/bin/clang-format.exe \
          temp/tools/clang-format-"$version"/*/bin/clang-format \
          temp/tools/clang-format-"$version"/*/Scripts/clang-format.exe; do
          if trice_tool_matches "$target" "$version"; then
            selected="$target"
            break
          fi
        done
        if [ -n "$selected" ]; then
          trice_plan_row "$tool" "$version" "$(trice_found_version "$tool")" reuse "$selected" temp/tools 0 0
        elif [ "$tool" = clang-format ] && [ -n "${CLANG_FORMAT_BIN:-}" ]; then
          trice_plan_row "$tool" "$version" "$(trice_found_version "$CLANG_FORMAT_BIN")" manual 'fix or unset CLANG_FORMAT_BIN' temp/tools 0 0
        else trice_plan_row "$tool" "$version" "$(trice_found_version "$tool")" install 'pinned package/build' temp/tools 150 600; fi
      fi
    done
  fi
  if [ "$scope" = native ] || { [ "$scope" = format ] && awk -F '\t' '$1 == "clang-format" && $4 == "install" { found=1 } END {exit !found}' "$TRICE_PLAN_FILE"; }; then
    local need_venv=0
    if [ "$TRICE_PLAN_OS" != windows ] && awk -F '\t' '$1 == "clang-format" && $4 == "install" {found=1} END {exit !found}' "$TRICE_PLAN_FILE"; then need_venv=1; fi
    selected=''
    for tool in "${TRICE_TEST_PYTHON:-python3}" python3 python; do
      if python_test_tool_works "$tool" && { [ "$need_venv" -eq 0 ] || "$tool" -c 'import venv, ensurepip' >/dev/null 2>&1; }; then
        selected="$tool"
        break
      fi
    done
    if [ -n "$selected" ]; then
      trice_plan_row Python '>=3.9' "$("$selected" --version 2>&1)" reuse "$selected" temp/tools 0 0
    elif [ "$TRICE_PLAN_OS" = windows ]; then
      select_windows_python_test_package || return 2
      if python_test_tool_works "$TRICE_PYTHON_DIR/python.exe"; then
        trice_plan_row Python '>=3.9' 'not in PATH' reuse "$TRICE_PYTHON_DIR/python.exe" temp/tools 0 0
      else trice_plan_row Python "$TRICE_PYTHON_VERSION" missing install 'official standard-library runtime' temp/tools 40 80; fi
    else trice_plan_row Python '>=3.9 with venv/ensurepip' missing manual 'install Python with venv/ensurepip' temp/tools 0 0; fi
    if [ "$need_venv" -eq 1 ] && [ -n "$selected" ]; then
      TRICE_FORMAT_PYTHON="$(command -v "$selected")"
      export TRICE_FORMAT_PYTHON
    fi
  fi
  if [ "$scope" = pdf ]; then
    trice_plan_cli stitchmd
  fi
  case "$scope" in cli:*) for tool in ${scope#cli:}; do trice_plan_cli "$tool"; done ;; esac
  case "$scope" in native | lint | pdf) trice_plan_npm || return 2 ;; esac
  # Go source fallbacks also write module/build caches. Account for their own
  # filesystems instead of assuming every byte lands beside the checkout.
  if awk -F '\t' '$4 == "install" && $5 == "source build" { found=1 } END {exit !found}' "$TRICE_PLAN_FILE"; then
    local build_cache module_cache
    build_cache="${GOCACHE:-$(GOTOOLCHAIN=local go env GOCACHE 2>/dev/null || true)}"
    module_cache="${GOMODCACHE:-$(GOTOOLCHAIN=local go env GOMODCACHE 2>/dev/null || true)}"
    [ -n "$build_cache" ] || build_cache="$(pwd)/.gocache"
    [ -n "$module_cache" ] || module_cache="${GOPATH:-$HOME/go}/pkg/mod"
    trice_plan_row Go-build-cache shared 'source fallback' install 'build-cache allowance' "$(trice_tool_path "$build_cache")" 0 768
    trice_plan_row Go-module-cache shared 'source fallback' install 'module-cache allowance' "$(trice_tool_path "$module_cache")" 0 512
  fi
  if [ "$after_updates" != 1 ]; then trice_plan_table; fi
  while IFS=$'\t' read -r tool version found action selected target installed peak; do
    [ "$action" != manual ] || blocked=1
  done <"$TRICE_PLAN_FILE"
  trice_plan_space || checkout_space_ok=0
  if [ "$blocked" -ne 0 ]; then trice_plan_can_continue || true; fi
  if [ "$after_updates" != 1 ] && [ "${TRICE_TEST_SETUP_APPROVED:-0}" != 1 ] && { [ -z "${CI:-}" ] || [ "${CI:-}" = false ]; }; then
    while true; do
      trice_plan_print ''
      trice_plan_print '[c] Continue'
      trice_plan_print '    Install missing pinned tool versions into temp/tools.'
      trice_plan_print '[u] Update system tools (up to pinned versions)'
      trice_plan_print '    Leave newer system versions unchanged.'
      trice_plan_print '    Install remaining missing pinned tool versions into temp/tools.'
      trice_plan_print '[a] Abort'
      trice_plan_print 'Choose [c/u/a] (default: a):'
      IFS= read -r answer || answer=''
      answer="${answer%$'\r'}"
      case "$answer" in
        u | U)
          if ! declare -F trice_system_tool_setup >/dev/null; then
            trice_plan_print 'INCOMPLETE: system upgrade helper is missing from this checkout.'
            return 2
          fi
          system_status=0
          trice_system_tool_setup || system_status=$?
          if [ "$system_status" -eq 4 ]; then continue; fi
          case "$system_status" in 0 | 3) ;; *) return 2 ;; esac
          # Rebuild the local plan, including blockers and disk checks. This
          # avoids installing a duplicate pin after a successful system update.
          hash -r
          trice_test_tool_plan "$scope" 1
          return $?
          ;;
        c | C)
          if ! trice_plan_can_continue; then continue; fi
          if [ "$checkout_space_ok" -ne 1 ]; then
            trice_plan_print 'Cannot continue: insufficient disk space.'
            continue
          fi
          break
          ;;
        a | A | '')
          TRICE_TEST_SETUP_CANCELLED=1
          trice_plan_print 'Cancelled.'
          return 2
          ;;
        *) trice_plan_print 'Please enter c, u, or a.' ;;
      esac
    done
  fi
  if [ "$blocked" -ne 0 ] || [ "$checkout_space_ok" -ne 1 ]; then
    trice_plan_print 'INCOMPLETE: checkout setup cannot proceed with the displayed host prerequisites or free space.'
    return 2
  fi
  if [ "$(trice_tool_hash scripts/test-tools.env | cut -c1-16)" != "$TRICE_PLAN_HASH" ]; then
    trice_plan_print 'INCOMPLETE: tool pins changed while the plan was displayed; rerun startup.'
    return 2
  fi
  export TRICE_TEST_SETUP_APPROVED=1
  export TRICE_TEST_PLAN_ACTIVE=1
  export TRICE_PLAN_FILE TRICE_PLAN_OS TRICE_PLAN_HOST TRICE_PLAN_PLATFORM
  # Select a discovered versioned/cache formatter explicitly; prep must not
  # download a second wheel merely because the host's default name is older.
  if [ -z "${CLANG_FORMAT_BIN:-}" ]; then
    while IFS=$'\t' read -r tool version found action selected target installed peak; do
      if [ "$tool" = clang-format ] && [ "$action" = reuse ]; then
        if [ -f "$selected" ]; then
          CLANG_FORMAT_BIN="$(trice_resolve_tool "$selected")"
          export CLANG_FORMAT_BIN
        fi
      fi
    done <"$TRICE_PLAN_FILE"
  fi
  export TRICE_TEST_PLAN_SCOPES="${TRICE_TEST_PLAN_SCOPES:-} $scope"
}
