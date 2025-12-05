#!/usr/bin/env bash

set -euo pipefail

clang-format -i \
  ./src/*.c \
  ./src/*.h \
  ./_test/*/*.c \
  ./_test/*/*.h \
  ./examples/*_inst/Core/Inc/trice*.h
