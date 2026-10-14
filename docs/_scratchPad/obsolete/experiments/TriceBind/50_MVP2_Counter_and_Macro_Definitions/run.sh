#!/bin/sh
# SPDX-License-Identifier: MIT

# Compile and execute both independent bind MVP2 experiments with available C compilers.
set -eu

ROOT=$(
  CDPATH='' cd -- "$(dirname -- "$0")" || exit 1
  pwd
)
TMP=$(mktemp -d "${TMPDIR:-/tmp}/trice-bind-mvp2.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

if [ -n "${CC:-}" ]; then
  set -- "$CC"
else
  set -- gcc clang
fi

compilerFound=0
counterCompilerFound=0

for compiler; do
  if ! command -v "$compiler" >/dev/null 2>&1; then
    continue
  fi
  compilerFound=1
  name=${compiler##*/}

  printf '\n== C compiler: %s ==\n' "$compiler"

  wrapperExe="$TMP/macro_definition_$name"
  "$compiler" -std=c11 -Wall -Wextra -Werror -pedantic \
    "$ROOT/macro_definition.c" -I"$ROOT" -o "$wrapperExe"
  "$wrapperExe"

  if printf '%s\n' '#ifndef __COUNTER__' '#error __COUNTER__ unavailable' '#endif' |
    "$compiler" -x c -E - >/dev/null 2>&1; then
    counterCompilerFound=1
    counterExe="$TMP/counter_sites_$name"
    "$compiler" -std=c11 -Wall -Wextra -Werror -pedantic \
      "$ROOT/counter_sites.c" -I"$ROOT" -o "$counterExe"
    "$counterExe"
  else
    printf 'counter-sites: skipped (%s has no __COUNTER__)\n' "$compiler"
  fi
done

if [ "$compilerFound" -eq 0 ]; then
  printf '%s\n' 'No GCC- or Clang-compatible C compiler found.' >&2
  exit 1
fi
if [ "$counterCompilerFound" -eq 0 ]; then
  printf '%s\n' 'No available compiler supports __COUNTER__; counter PoC not verified.' >&2
  exit 1
fi

printf '\n%s\n' 'All available bind MVP2 PoCs passed.'
