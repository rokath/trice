#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
CC=${CC:-clang}

rm -rf build
mkdir -p build

echo "1. Build PoC trice executable"
go build -o build/trice ./cmd/trice

echo "2. Run trice bind"
./build/trice bind \
  -til til.json \
  -li li.json \
  -src app/module.c \
  -out build/module.c.triceIDs.h

echo "3. Compile the original user source directly"
"$CC" \
  -std=c11 -Wall -Wextra -Werror \
  -Iapp -Ibuild -Itrice/src \
  app/module.c trice/src/trice.c \
  -o build/demo

echo "4. Run target demo"
./build/demo

echo "5. Raw build/log.bin"
if command -v xxd >/dev/null 2>&1; then
  xxd -g 1 build/log.bin
else
  od -An -tx1 -v build/log.bin
fi

echo
echo "6. Decode with tlog"
tlog -p FILEBUFFER -args ./build/log.bin -pf none -prefix off -hs off -i ./til.json -li ./li.json
