#!/bin/sh
set -eu

CC=${CC:-cc}
OUT=${TMPDIR:-/tmp}/trice_bind_poc2_$$
PP=${TMPDIR:-/tmp}/trice_bind_poc2_$$.i
ACTUAL=${TMPDIR:-/tmp}/trice_bind_poc2_$$.out
trap 'rm -f "$OUT" "$PP" "$ACTUAL"' EXIT HUP INT TERM

"$CC" -std=c11 -Wall -Wextra -Werror -pedantic main.c -o "$OUT"
"$CC" -std=c11 -E -P main.c > "$PP"
"$OUT" > "$ACTUAL"
diff -u expected.txt "$ACTUAL"

printf '%s\n' 'PoC 2 passed.'
printf '%s\n' 'Relevant preprocessor result:'
grep -E 'poc2_emit\("(AUTO|REPLACE)"' "$PP" || true
