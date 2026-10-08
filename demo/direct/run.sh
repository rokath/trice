#!/bin/sh
# SPDX-License-Identifier: MIT
# Write each Trice record immediately to a binary file.

# Stop on errors, so an unsuccessful build cannot run an old executable.
set -e
cd "$(dirname "$0")"

# When copying this project folder, adjust only this path to the Trice library.
trice_src=../../src
compiler=cc # Use gcc here if that is your compiler's name.
# Optional: uncomment to display the compiler path before building.
# command -v "$compiler"

# 1. Assign IDs; keep generated headers and the field registry inside build/.
# build/ is ignored by git, so generated files do not pollute the repository.
trice bind -src main.c -genDir build/generated_sidecars

# 2. Compile the program and library. -I adds header directories; -o names output.
# Project headers come first; default_conf provides the fallback RTT header.
# Optional size tuning: ../../docs/TriceReferenceManual.md#trice-project-image-size-optimization
mkdir -p build
"$compiler" -I. -Ibuild/generated_sidecars -I"$trice_src" -I"$trice_src/default_conf" \
  main.c "$trice_src"/*.c -o build/demo_direct.exe

# 3. Run where the program writes log.bin; return to the ID tables afterwards.
cd build
./demo_direct.exe
cd ..

# 4. Decode the captured records using this project's til.json.
trice log -p FILEBUFFER -args build/log.bin
