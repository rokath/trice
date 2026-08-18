#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Shared pre-build preparation for example projects whose sources are kept in
# Trice Bind state. Source this file and call prepare_trice_bind_build with the
# repository root; the exported variables then remain visible to the compiler.

# prepare_trice_bind_build creates missing or stale generated headers for a
# direct build. A managed test workflow already owns source preparation and is
# only validated here, so nested builds cannot change its Insert/Clean state.
prepare_trice_bind_build() {
  trice_bind_repo_root=$1

  if [ "${TRICE_ID_WORKFLOW_OWNER:-0}" = "1" ]; then
    if [ "${TRICE_ID_WORKFLOW:-}" = "bind" ]; then
      if [ -z "${TRICE_BIND_INCLUDE_DIR:-}" ] || [ ! -d "$TRICE_BIND_INCLUDE_DIR" ]; then
        echo "Generated Trice bind headers are missing for the managed build." >&2
        echo "The workflow owner must set TRICE_BIND_INCLUDE_DIR to its bind output directory." >&2
        return 1
      fi
    fi
    return 0
  fi

  if ! command -v trice >/dev/null 2>&1; then
    echo "The trice executable is required to generate bind headers." >&2
    echo "Install trice, put it on PATH, and run this build again." >&2
    return 1
  fi

  # The repository helper owns the canonical source list, TIL/LI options, and
  # bind output location. It is idempotent for already-bound source files.
  (
    cd "$trice_bind_repo_root" || exit 1
    bash ./trice_bindIDs_in_examples_and_test_folder.sh
  ) || return $?

  trice_bind_include_dir=$trice_bind_repo_root/build/triceIDs
  if [ ! -d "$trice_bind_include_dir" ]; then
    echo "trice bind did not create the expected header directory:" >&2
    echo "  $trice_bind_include_dir" >&2
    return 1
  fi

  # CMake consumes TRICE_BIND_INCLUDE_DIR explicitly. GCC and Clang-based
  # Makefiles consume C_INCLUDE_PATH, whose list separator differs on Windows.
  trice_bind_native_dir=$trice_bind_include_dir
  trice_bind_path_separator=:
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      trice_bind_path_separator=';'
      if command -v cygpath >/dev/null 2>&1; then
        trice_bind_native_dir=$(cygpath -m "$trice_bind_include_dir")
      fi
      ;;
  esac
  TRICE_BIND_INCLUDE_DIR=$trice_bind_native_dir
  if [ -n "${C_INCLUDE_PATH:-}" ]; then
    C_INCLUDE_PATH=$trice_bind_include_dir$trice_bind_path_separator$C_INCLUDE_PATH
  else
    C_INCLUDE_PATH=$trice_bind_include_dir
  fi

  # Inner example scripts must not start the legacy Insert/Clean lifecycle after
  # this preparation because that would invalidate the generated line mapping.
  TRICE_ID_WORKFLOW_OWNER=1
  TRICE_ID_WORKFLOW=bind
  export TRICE_BIND_INCLUDE_DIR C_INCLUDE_PATH TRICE_ID_WORKFLOW_OWNER TRICE_ID_WORKFLOW
}
