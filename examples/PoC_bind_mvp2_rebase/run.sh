#!/bin/sh
# SPDX-License-Identifier: MIT

# Compile the positive and negative rebase cases with available C/C++ compilers.
set -eu

# SCRIPT_DIR stabilizes all repository-relative paths used below.
SCRIPT_DIR=$(
  CDPATH='' cd -- "$(dirname -- "$0")" || exit 1
  pwd
)
cd "$SCRIPT_DIR"

# TMP honors the host's temporary-directory variables and falls back locally.
TMP=$(mktemp -d "${TMPDIR:-${TMP:-.}}/trice-bind-rebase.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

# expect_failure accepts a compiler, language, standard, define, and diagnostic.
# A rejected build counts as success only when it contains the intended guard.
expect_failure()
{
  compiler=$1
  language=$2
  standard=$3
  define=$4
  diagnostic=$5
  label=$6
  log="$TMP/${label}.log"

  if "$compiler" -x "$language" "-std=$standard" -Wall -Wextra -Werror -pedantic \
    "-D$define" main.c -I. -o "$TMP/${label}" >"$log" 2>&1; then
    printf '%s\n' "$label unexpectedly compiled successfully" >&2
    exit 1
  fi
  if ! grep -F "$diagnostic" "$log" >/dev/null 2>&1; then
    printf '%s\n' "$label failed without the expected diagnostic:" >&2
    sed -n '1,120p' "$log" >&2
    exit 1
  fi
  printf '%s\n' "$label: expected diagnostic confirmed"
}

# counter_available probes the selected language mode without relying on a
# compiler brand or version list.
counter_available()
{
  compiler=$1
  language=$2
  printf '%s\n' '#ifndef __COUNTER__' '#error __COUNTER__ unavailable' '#endif' | \
    "$compiler" -x "$language" -E - >/dev/null 2>&1
}

cCompilerFound=0
if [ -n "${CC:-}" ]; then
  set -- "$CC"
else
  set -- gcc clang
fi

for compiler; do
  if ! command -v "$compiler" >/dev/null 2>&1; then
    continue
  fi
  if ! counter_available "$compiler" c; then
    printf 'C compiler %s has no __COUNTER__; positive test skipped.\n' "$compiler"
    continue
  fi
  cCompilerFound=1
  name=${compiler##*/}
  printf '\n== C compiler: %s ==\n' "$compiler"

  "$compiler" -x c -std=c11 -O0 -Wall -Wextra -Werror -pedantic \
    main.c -I. -o "$TMP/c_${name}_o0"
  "$TMP/c_${name}_o0"

  "$compiler" -x c -std=c11 -O2 -Wall -Wextra -Werror -pedantic \
    main.c -I. -o "$TMP/c_${name}_o2"
  "$TMP/c_${name}_o2"

  expect_failure "$compiler" c c11 POC_INJECT_COUNTER_DRIFT=1 \
    'unexpected __COUNTER__ expansion inside an advanced scope' "c_${name}_drift"
  expect_failure "$compiler" c c11 POC_FORCE_NO_COUNTER=1 \
    'target preprocessor has no __COUNTER__' "c_${name}_no_counter"
done

if [ "$cCompilerFound" -eq 0 ]; then
  printf '%s\n' 'No available GCC- or Clang-compatible C compiler provides __COUNTER__.' >&2
  exit 1
fi

cxxCompilerFound=0
if [ -n "${CXX:-}" ]; then
  set -- "$CXX"
else
  set -- g++ clang++
fi

for compiler; do
  if ! command -v "$compiler" >/dev/null 2>&1; then
    continue
  fi
  if ! counter_available "$compiler" c++; then
    printf 'C++ compiler %s has no __COUNTER__; positive test skipped.\n' "$compiler"
    continue
  fi
  cxxCompilerFound=1
  name=${compiler##*/}
  printf '\n== C++ compiler: %s ==\n' "$compiler"

  "$compiler" -x c++ -std=c++17 -O0 -Wall -Wextra -Werror -pedantic \
    main.c -I. -o "$TMP/cxx_${name}_o0"
  "$TMP/cxx_${name}_o0"

  "$compiler" -x c++ -std=c++17 -O2 -Wall -Wextra -Werror -pedantic \
    main.c -I. -o "$TMP/cxx_${name}_o2"
  "$TMP/cxx_${name}_o2"

  expect_failure "$compiler" c++ c++17 POC_INJECT_COUNTER_DRIFT=1 \
    'unexpected __COUNTER__ expansion inside an advanced scope' "cxx_${name}_drift"
  expect_failure "$compiler" c++ c++17 POC_FORCE_NO_COUNTER=1 \
    'target preprocessor has no __COUNTER__' "cxx_${name}_no_counter"
done

if [ "$cxxCompilerFound" -eq 0 ]; then
  printf '%s\n' 'No available GCC- or Clang-compatible C++ compiler provides __COUNTER__.' >&2
  exit 1
fi

printf '\n%s\n' 'All available variant-1 rebase checks passed.'
