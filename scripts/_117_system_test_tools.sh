#!/usr/bin/env bash

# Optional, explicitly selected system upgrades. Package catalog reads never run
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

# Read package-manager ownership rather than a possibly unrelated PATH command
# (for example Apple's gcc alias or a checkout-local executable).
trice_system_installed_versions() {
  local package="$1"
  case "$TRICE_SYSTEM_MANAGER" in
    apt)
      dpkg-query -W -f='${Status}\t${Version}\n' "$package" 2>/dev/null |
        awk -F '\t' '$1=="install ok installed" {print $2}'
      ;;
    brew)
      HOMEBREW_NO_AUTO_UPDATE=1 brew list --versions --formula "$package" 2>/dev/null |
        awk -v package="$package" '$1==package {for(i=2;i<=NF;i++) print $i}'
      ;;
    winget)
      winget list --id "$package" --exact --source winget --accept-source-agreements --disable-interactivity 2>/dev/null |
        awk -v package="$package" '{for(i=1;i<NF;i++) if($i==package) print $(i+1)}'
      ;;
  esac
}

# Compare numeric releases without GNU sort -V or a prepared runtime. Unknown
# identities stay untouched. Distribution revisions do not change upstream pins.
trice_system_version_is_older() {
  awk -v current="$1" -v pin="$2" 'BEGIN {
    sub(/^[0-9]+:/,"",current); sub(/[-+_].*$/,"",current);
    if(current !~ /^[0-9]+([.][0-9]+)+$/ || pin !~ /^[0-9]+([.][0-9]+)+$/) exit 1;
    n=split(current,a,"."); m=split(pin,b,".");
    for(i=1;i<=n || i<=m;i++) {
      if(a[i]+0 < b[i]+0) exit 0;
      if(a[i]+0 > b[i]+0) exit 1;
    }
    exit 1;
  }'
}

# Every installed copy must be older: an inactive newer Homebrew keg must never
# be replaced just because an older executable happens to appear first in PATH.
trice_system_package_is_older() {
  local versions current
  versions="$(trice_system_installed_versions "$1")" || return 1
  [ -n "$versions" ] || return 1
  while IFS= read -r current; do
    trice_system_version_is_older "$current" "$2" || return 1
  done <<<"$versions"
}

# Keep skipped packages and full commands available without filling the menu.
# This file is beside the saved plan and does not share tools.log's file offset.
trice_system_detail() {
  printf '%s\n' "$*" >>"$TRICE_PLAN_FILE.system.details"
}

# Prepare upgrades only to exact available pins, never to a newer catalog
# release. Unsupported, absent and newer system tools use local preparation.
# Return 3 when no upgrade is available, 4 on refusal, and 2 on an actual failure.
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
  : >"$plan.details"
  trice_system_detail "Checking exact system packages with $TRICE_SYSTEM_MANAGER (no repository update or installation)."
  while IFS=$'\t' read -r tool version found action _ _ installed peak; do
    case "$action" in install | manual | reuse) ;; *) continue ;; esac
    [[ "$version" =~ ^[0-9]+\.[0-9]+(\.[0-9]+)?$ ]] || continue
    case "$tool" in Go-build-cache | Go-module-cache) continue ;; esac
    missing=$((missing + 1))
    if ! packages="$(trice_system_package_names "$tool" "$version")"; then
      trice_system_detail "  $tool: no supported system upgrade; use temp/tools if needed."
      unsupported=1
      continue
    fi
    group_available=1
    : >"$commands.group"
    for package in $packages; do
      if ! trice_system_package_is_older "$package" "$version"; then
        trice_system_detail "  $package: not installed, not older, or version unknown; leave unchanged."
        group_available=0
        continue
      fi
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
        trice_system_detail "  $package: exact upstream version $version is not available in the configured package catalog."
        unsupported=1
        group_available=0
        continue
      fi
      printf '%s\t%s\n' "$package" "$catalog" >>"$commands.group"
      trice_plan_print "  System upgrade: $package -> $catalog"
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
    trice_system_detail "  $tool: approximately $installed MiB additional installed data, $peak MiB setup peak at $prefix."
    printf '%s\t%s\t%s\tinstall\t%s package\t%s\t%s\t%s\n' "$tool" "$version" "$found" "$TRICE_SYSTEM_MANAGER" "$prefix" "$installed" "$peak" >>"$plan"
    total=$((total + peak))
  done <"$TRICE_PLAN_FILE"
  if [ "$missing" -eq 0 ]; then
    trice_plan_print 'No system tools need an upgrade.'
    return 3
  fi
  if [ ! -s "$commands" ]; then
    trice_plan_print 'No system upgrades available. Missing pinned tool versions will use temp/tools.'
    return 3
  fi
  if [ "$unsupported" -ne 0 ]; then trice_plan_print 'Only the listed system tools will be upgraded. Remaining missing pinned tool versions will use temp/tools.'; fi
  # Package-manager dependencies and cache/staging consumption vary by host.
  # Keep a conservative allowance on their actual destination filesystem too.
  printf 'Package-cache\tallowance\tmanager-dependent\tinstall\tpackage cache/dependencies\t%s\t0\t%s\n' "$cache" "$((total / 2 + 512))" >>"$plan"
  printf 'System-dependencies\tallowance\tmanager-dependent\tinstall\tdependency estimate\t%s\t0\t%s\n' "$prefix" "$((total + 512))" >>"$plan"
  trice_system_detail 'System sizes are conservative estimates, including separate dependency and package-cache allowances; package managers may require additional space.'
  trice_system_detail "System destination: $prefix; package cache/staging: $cache. Estimates include a conservative dependency allowance."
  (
    TRICE_PLAN_FILE="$plan"
    trice_plan_space
  ) || return 2
  trice_system_detail 'System-upgrade commands (the package manager may require administrator credentials):'
  while IFS=$'\t' read -r package catalog; do
    case "$TRICE_SYSTEM_MANAGER" in
      apt) trice_system_detail "  apt-get install --only-upgrade --yes --no-remove --no-install-recommends $package=$catalog" ;;
      brew) trice_system_detail "  HOMEBREW_NO_AUTO_UPDATE=1 HOMEBREW_NO_INSTALLED_DEPENDENTS_CHECK=1 HOMEBREW_NO_INSTALL_CLEANUP=1 brew upgrade --formula $package (catalog version $catalog)" ;;
      winget) trice_system_detail "  winget upgrade --id $package --exact --version $catalog --source winget" ;;
    esac
  done <"$commands"
  trice_plan_print "Upgrade details: $plan.details"
  trice_plan_print 'Upgrade these system tools to the pinned versions? [y/N]'
  IFS= read -r answer || answer=''
  answer="${answer%$'\r'}"
  case "$answer" in y | Y | yes | YES) ;; *)
    trice_plan_print 'System upgrades cancelled before any package change. Returning to the menu.'
    return 4
    ;;
  esac
  # Recheck after confirmation before the first package change. Another setup
  # process may have upgraded a tool while the user was reviewing the plan.
  while IFS=$'\t' read -r package catalog; do
    local upstream="$catalog"
    upstream="${upstream#*:}"
    upstream="${upstream%%[-+_]*}"
    if ! trice_system_package_is_older "$package" "$upstream"; then
      trice_plan_print "INCOMPLETE: $package changed since the upgrade plan; rerun startup."
      return 2
    fi
  done <"$commands"
  if [ "$TRICE_SYSTEM_MANAGER" = apt ]; then
    local apt_arguments=()
    while IFS=$'\t' read -r package catalog; do apt_arguments+=("$package=$catalog"); done <"$commands"
    if [ "$(id -u)" = 0 ]; then
      trice_setup_with_progress 'Updating system tools' apt-get install --only-upgrade --yes --no-remove --no-install-recommends "${apt_arguments[@]}" || return 2
    elif command -v sudo >/dev/null 2>&1; then
      trice_setup_with_progress 'Updating system tools' sudo apt-get install --only-upgrade --yes --no-remove --no-install-recommends "${apt_arguments[@]}" || return 2
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
          HOMEBREW_NO_AUTO_UPDATE=1 HOMEBREW_NO_INSTALLED_DEPENDENTS_CHECK=1 HOMEBREW_NO_INSTALL_CLEANUP=1 trice_setup_with_progress "Updating system $package to $catalog" brew upgrade --formula "$package" || return 2
          ;;
        winget) trice_setup_with_progress "Updating system $package to $catalog" winget upgrade --id "$package" --exact --version "$catalog" --source winget --accept-source-agreements --accept-package-agreements || return 2 ;;
      esac
    done <"$commands"
  fi
  hash -r
  trice_plan_print 'System upgrades finished. Rechecking tools before installing remaining missing pinned tool versions into temp/tools.'
}
