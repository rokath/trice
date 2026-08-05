#!/bin/sh
set -eu

ROOT=$(
  CDPATH='' cd -- "$(dirname -- "$0")" || exit 1
  pwd
)
TMP=${TMPDIR:-/tmp}/trice_bind_verification_$$
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

found=0

run_c_suite() {
  compiler=$1
  found=1
  printf '\n== C compiler: %s ==\n' "$compiler"
  (cd "$ROOT/poc1_local_dispatch" && CC="$compiler" ./run.sh)
  (cd "$ROOT/poc2_site_descriptor" && CC="$compiler" ./run.sh)
}

run_cpp_suite() {
  compiler=$1
  found=1
  printf '\n== C++ compiler: %s ==\n' "$compiler"
  for poc in poc1_local_dispatch poc2_site_descriptor; do
    exe="$TMP/${poc}_${compiler##*/}"
    out="$exe.out"
    "$compiler" -x c++ -std=c++17 -Wall -Wextra -Werror -pedantic \
      "$ROOT/$poc/main.c" -I"$ROOT/$poc" -o "$exe"
    "$exe" >"$out"
    diff -u "$ROOT/$poc/expected.txt" "$out"
    printf '%s: passed\n' "$poc"
  done
}

for compiler in gcc clang; do
  if command -v "$compiler" >/dev/null 2>&1; then
    run_c_suite "$compiler"
  fi
done

for compiler in g++ clang++; do
  if command -v "$compiler" >/dev/null 2>&1; then
    run_cpp_suite "$compiler"
  fi
done

if [ "$found" -eq 0 ]; then
  printf '%s\n' 'No supported C or C++ compiler found.' >&2
  exit 1
fi

printf '\n%s\n' 'All available verification suites passed.'
