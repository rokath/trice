#!/usr/bin/env bash

# Optional, explicitly selected system setup. Package catalog reads never run
# updates, add repositories/taps, or substitute a newer version for a missing pin.
# No function in this file is called merely by sourcing the test environment.

# Keep package-manager identities explicit. Unmapped tools (notably npm graphs
# and the complete WinLibs runtime) remain available through the checkout path.
trice_system_package_names() {
  local tool="$1" version="$2" major="${2%%.*}"
  case "$TRICE_SYSTEM_MANAGER:$tool" in
    apt:Go) printf 'golang-%s-go\n' "${version%.*}" ;;
    apt:Node.js)
      # Debian's nodejs package does not include npm. Do not advertise it as
      # a complete Node/npm fallback when those prerequisites are absent.
      command -v npm >/dev/null 2>&1 && command -v npx >/dev/null 2>&1 || return 1
      printf 'nodejs\n'
      ;;
    apt:cmake) printf 'cmake\n' ;;
    apt:gcc) printf 'gcc-%s g++-%s\n' "$major" "$major" ;;
    apt:clang) printf 'clang-%s clangd-%s\n' "$major" "$major" ;;
    apt:clang-format) printf 'clang-format-%s\n' "$major" ;;
    apt:cppcheck) printf 'cppcheck\n' ;;
    apt:shfmt | apt:jq | apt:shellcheck | apt:goreleaser) printf '%s\n' "$tool" ;;
    brew:Go) printf 'go\n' ;;
    brew:Node.js) printf 'node\n' ;;
    brew:cmake | brew:gcc | brew:cppcheck | brew:shfmt | brew:jq | brew:shellcheck | brew:goreleaser) printf '%s\n' "$tool" ;;
    brew:clang) printf 'llvm\n' ;;
    winget:Go) printf 'GoLang.Go\n' ;;
    winget:Node.js) printf 'OpenJS.NodeJS\n' ;;
    winget:cmake) printf 'Kitware.CMake\n' ;;
    winget:cppcheck) printf 'Cppcheck.Cppcheck\n' ;;
    *) return 1 ;;
  esac
}

# Debian/Ubuntu package versions include epochs and distribution revisions.
# Compare the complete upstream release, then retain the full catalog version
# in the actual apt command. For example 1:23.1.4-2 matches only pin 23.1.4.
trice_apt_package_version() {
  local package="$1" wanted="$2"
  apt-cache madison "$package" 2>/dev/null | awk -F '|' -v wanted="$wanted" '
    { version=$2; gsub(/[[:space:]]/,"",version); upstream=version;
      sub(/^[0-9]+:/,"",upstream); sub(/[-+].*$/, "", upstream);
      if(upstream==wanted) {print version; exit} }'
}

# Build a concrete, exact-version command list before requesting installation.
# Unsupported/missing pins stay unchanged. Available complete package groups can
# be installed after review, but never trigger a silent checkout fallback.
trice_system_tool_setup() {
  local tool version found action installed peak packages package catalog info escaped
  local missing=0 unsupported=0 total=0 prefix cache answer='' group_available
  local plan="$TRICE_PLAN_FILE.system" commands="$TRICE_PLAN_FILE.system-commands"
  TRICE_SYSTEM_MANAGER=''
  case "$TRICE_PLAN_OS" in
    linux) if command -v apt-cache >/dev/null 2>&1 && command -v apt-get >/dev/null 2>&1; then TRICE_SYSTEM_MANAGER=apt; fi ;;
    darwin) if command -v brew >/dev/null 2>&1; then TRICE_SYSTEM_MANAGER=brew; fi ;;
    windows) if command -v winget >/dev/null 2>&1; then TRICE_SYSTEM_MANAGER=winget; fi ;;
  esac
  if [ -z "$TRICE_SYSTEM_MANAGER" ]; then
    trice_plan_print 'System setup unavailable: no supported apt/Homebrew/winget installation found.'
    return 3
  fi
  case "$TRICE_SYSTEM_MANAGER" in
    apt)
      prefix="$(dirname "$(dirname "$(trice_resolve_tool "$(command -v apt-get)")")")"
      # apt-config returns the configured cache; no filesystem location is
      # assumed to be on the checkout's device.
      cache="$(apt-config dump 2>/dev/null | awk '$1=="Dir::Cache" {gsub(/[";]/,"",$2); print $2; exit}')"
      case "$cache" in /*) ;; *) cache="$(dirname "$prefix")/${cache:-var/cache/apt}" ;; esac
      ;;
    brew)
      prefix="$(HOMEBREW_NO_AUTO_UPDATE=1 brew --prefix)" || return 3
      cache="$(HOMEBREW_NO_AUTO_UPDATE=1 brew --cache)" || return 3
      ;;
    winget)
      prefix="$(trice_tool_path "${PROGRAMFILES:-${ProgramFiles:-${LOCALAPPDATA:-$PWD}}}")" || return 3
      cache="$(trice_tool_path "${TEMP:-${TMPDIR:-$PWD}}")" || return 3
      ;;
  esac
  : >"$plan"
  : >"$commands"
  trice_plan_print "Checking exact system packages with $TRICE_SYSTEM_MANAGER (no repository update or installation)."
  while IFS=$'\t' read -r tool version found action _ _ installed peak; do
    case "$action" in install | manual) ;; *) continue ;; esac
    case "$tool" in Go-build-cache | Go-module-cache) continue ;; esac
    missing=$((missing + 1))
    if ! packages="$(trice_system_package_names "$tool" "$version")"; then
      trice_plan_print "  $tool $version: no verified complete system-package mapping; use checkout setup or install it manually."
      unsupported=1
      continue
    fi
    group_available=1
    : >"$commands.group"
    for package in $packages; do
      catalog=''
      case "$TRICE_SYSTEM_MANAGER" in
        apt) catalog="$(trice_apt_package_version "$package" "$version")" ;;
        brew)
          info="$(HOMEBREW_NO_AUTO_UPDATE=1 brew info --json=v2 --formula "$package" 2>/dev/null)" || info=''
          escaped="${version//./\\.}"
          if printf '%s' "$info" | tr -d '\r\n' | grep -Eq '"versions"[[:space:]]*:[[:space:]]*\{[^}]*"stable"[[:space:]]*:[[:space:]]*"'"$escaped"'"'; then catalog="$version"; fi
          ;;
        winget)
          if winget show --id "$package" --exact --version "$version" --source winget --accept-source-agreements --disable-interactivity >/dev/null 2>&1; then catalog="$version"; fi
          ;;
      esac
      if [ -z "$catalog" ]; then
        trice_plan_print "  $package: exact upstream version $version is not available in the configured package catalog."
        unsupported=1
        group_available=0
        continue
      fi
      printf '%s\t%s\n' "$package" "$catalog" >>"$commands.group"
      trice_plan_print "  $package = $catalog"
    done
    [ "$group_available" -eq 1 ] || continue
    cat "$commands.group" >>"$commands" || return 2
    # System packages have their own footprint; manual prerequisite rows carry
    # zero checkout bytes and must never authorize a zero-size system install.
    case "$tool" in
      Go)
        installed=500
        peak=1000
        ;;
      Node.js)
        installed=300
        peak=600
        ;;
      gcc)
        installed=1500
        peak=3000
        ;;
      clang)
        installed=16000
        peak=32000
        ;;
      cmake)
        installed=300
        peak=600
        ;;
      *)
        installed=200
        peak=600
        ;;
    esac
    # Prefer distribution sizes where apt supplies them. Dependencies have a
    # separate allowance below; do not charge a source-build budget for binaries.
    if [ "$TRICE_SYSTEM_MANAGER" = apt ]; then
      local sizes size_install=0 size_download=0
      while IFS=$'\t' read -r package catalog; do
        sizes="$(apt-cache show "$package=$catalog" 2>/dev/null | awk '/^Installed-Size:/ {i=$2} /^Size:/ {d=$2} END {printf "%d %d",i,d}')"
        local package_install package_download
        read -r package_install package_download <<<"$sizes"
        size_install=$((size_install + (package_install + 1023) / 1024))
        size_download=$((size_download + (package_download + 1048575) / 1048576))
      done <"$commands.group"
      if [ "$size_install" -gt 0 ]; then
        installed="$size_install"
        peak=$((installed * 2 + size_download))
      fi
    fi
    trice_plan_print "  $tool: approximately $installed MiB additional installed data, $peak MiB setup peak at $prefix."
    printf '%s\t%s\t%s\tinstall\t%s package\t%s\t%s\t%s\n' "$tool" "$version" "$found" "$TRICE_SYSTEM_MANAGER" "$prefix" "$installed" "$peak" >>"$plan"
    total=$((total + peak))
  done <"$TRICE_PLAN_FILE"
  if [ "$missing" -eq 0 ]; then
    trice_plan_print 'No system packages need installation.'
    return 0
  fi
  if [ ! -s "$commands" ]; then
    trice_plan_print 'No complete exact system-package group is available. No package will be installed.'
    return 3
  fi
  if [ "$unsupported" -ne 0 ]; then trice_plan_print 'Only the available exact package groups listed below can be installed. Unavailable tools stay unchanged; no checkout fallback is started automatically.'; fi
  # Package-manager dependencies and cache/staging consumption vary by host.
  # Keep a conservative allowance on their actual destination filesystem too.
  printf 'Package-cache\tallowance\tmanager-dependent\tinstall\tpackage cache/dependencies\t%s\t0\t%s\n' "$cache" "$((total / 2 + 512))" >>"$plan"
  printf 'System-dependencies\tallowance\tmanager-dependent\tinstall\tdependency estimate\t%s\t0\t%s\n' "$prefix" "$((total + 512))" >>"$plan"
  trice_plan_print 'System sizes are conservative estimates, including separate dependency and package-cache allowances; package managers may require additional space.'
  trice_plan_print "System destination: $prefix; package cache/staging: $cache. Estimates include a conservative dependency allowance."
  (
    TRICE_PLAN_FILE="$plan"
    trice_plan_space
  ) || return 2
  trice_plan_print 'Exact system-install commands (the package manager may require administrator credentials):'
  while IFS=$'\t' read -r package catalog; do
    case "$TRICE_SYSTEM_MANAGER" in
      apt) trice_plan_print "  apt-get install --yes --no-remove --no-install-recommends $package=$catalog" ;;
      brew) trice_plan_print "  HOMEBREW_NO_AUTO_UPDATE=1 brew install --formula $package (catalog version $catalog)" ;;
      winget) trice_plan_print "  winget install --id $package --exact --version $catalog --source winget" ;;
    esac
  done <"$commands"
  trice_plan_print 'Install these exact system packages? [y/N]'
  IFS= read -r answer || answer=''
  answer="${answer%$'\r'}"
  case "$answer" in y | Y | yes | YES) ;; *)
    trice_plan_print 'System installation cancelled before any package change.'
    return 2
    ;;
  esac
  if [ "$TRICE_SYSTEM_MANAGER" = apt ]; then
    local apt_arguments=()
    while IFS=$'\t' read -r package catalog; do apt_arguments+=("$package=$catalog"); done <"$commands"
    if [ "$(id -u)" = 0 ]; then
      apt-get install --yes --no-remove --no-install-recommends "${apt_arguments[@]}" || return 2
    elif command -v sudo >/dev/null 2>&1; then
      sudo apt-get install --yes --no-remove --no-install-recommends "${apt_arguments[@]}" || return 2
    else
      trice_plan_print 'INCOMPLETE: sudo is required for the displayed apt installation; no checkout fallback was started.'
      return 2
    fi
  else
    while IFS=$'\t' read -r package catalog; do
      case "$TRICE_SYSTEM_MANAGER" in
        brew)
          # Recheck immediately: a catalog update must not install a new release
          # after the user reviewed a different pin. No forced downgrade occurs.
          info="$(HOMEBREW_NO_AUTO_UPDATE=1 brew info --json=v2 --formula "$package")" || return 2
          escaped="${catalog//./\\.}"
          printf '%s' "$info" | tr -d '\r\n' | grep -Eq '"versions"[[:space:]]*:[[:space:]]*\{[^}]*"stable"[[:space:]]*:[[:space:]]*"'"$escaped"'"' || {
            trice_plan_print "INCOMPLETE: $package catalog changed; rerun the plan."
            return 2
          }
          HOMEBREW_NO_AUTO_UPDATE=1 brew install --formula "$package" || return 2
          ;;
        winget) winget install --id "$package" --exact --version "$catalog" --source winget --accept-source-agreements --accept-package-agreements || return 2 ;;
      esac
    done <"$commands"
  fi
  trice_plan_print 'System package setup finished. Open a fresh shell if PATH changed, then rerun the startup check. No tests were started.'
  export TRICE_TEST_SETUP_ONLY=1
}
