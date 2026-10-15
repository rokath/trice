cmake_minimum_required(VERSION 3.21)

# =====================================================================================
# CLion / CMake "Review Host" for the Trice C library
#
# Purpose:
#   This CMake project is NOT meant to replace Trice's existing demos/tests.
#   It is a lightweight "review host" that gives IDEs (CLion) a deterministic build model
#   so that indexing, navigation, inspections, and duplicate-definition checks work reliably.
#
# Layout assumptions:
#   trice/                      -> repository root (this is NOT where this CMakeLists.txt lives)
#   trice/_test/clion-review/   -> review folder (contains: main.c, triceConfig.h, this CMakeLists.txt)
#   trice/_test/testdata/       -> contains triceCheck.c (implements TriceCheck())
#   trice/src/                  -> contains the library .c and .h files
#
# Key concept:
#   User projects include only "#include trice.h".
#   trice.h will include "triceConfig.h" (user project specific) before falling back to defaults.
#   We mimic that by putting _test/clion-review/ FIRST in include directories.
#
# Cross-platform:
#   Works on Windows, Linux, macOS. Toolchain (compiler/linker) selection is handled by CMake.
#   On Windows (MinGW/clang), this is expected to work with clang target x86_64-w64-windows-gnu.
# =====================================================================================

project(trice_clion_review LANGUAGES C)

# -------------------------------------------------------------------------------------
# 1) Language and basic build defaults
# -------------------------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON) # ON -> -std=gnu11, OFF -> -std=c11 (demands "trice0" when no params are passed)

# Make compile_commands.json available for tooling (also helpful for CLion/clang-tidy).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Prefer lld when building with clang (especially helpful on Windows/MinGW).
if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_link_options(-fuse-ld=lld)
endif()

# Default to a reasonable build type if the generator supports it and none is provided.
# (Multi-config generators like Visual Studio ignore this.)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# -------------------------------------------------------------------------------------
# 2) Resolve repository paths relative to this CMakeLists.txt location.
#    This file lives in: <repo>/_test/clion-review/CMakeLists.txt
# -------------------------------------------------------------------------------------
get_filename_component(TRICE_REVIEW_DIR   "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)      # .../_test/clion-review
get_filename_component(TRICE_TEST_DIR     "${TRICE_REVIEW_DIR}/.." ABSOLUTE)         # .../_test
get_filename_component(TRICE_ROOT_DIR     "${TRICE_TEST_DIR}/.." ABSOLUTE)           # .../ (repo root)
set(TRICE_SRC_DIR        "${TRICE_ROOT_DIR}/src")
set(TRICE_TESTDATA_DIR   "${TRICE_TEST_DIR}/testdata")

# Sanity checks to fail early with a clear error message if paths are wrong.
if(NOT EXISTS "${TRICE_SRC_DIR}")
  message(FATAL_ERROR "Expected Trice source directory not found: ${TRICE_SRC_DIR}")
endif()

if(NOT EXISTS "${TRICE_TESTDATA_DIR}/triceCheck.c")
  message(FATAL_ERROR "Expected file not found: ${TRICE_TESTDATA_DIR}/triceCheck.c")
endif()

if(NOT EXISTS "${TRICE_REVIEW_DIR}/main.c")
  message(FATAL_ERROR "Expected file not found: ${TRICE_REVIEW_DIR}/main.c")
endif()

if(NOT EXISTS "${TRICE_REVIEW_DIR}/triceConfig.h")
  message(FATAL_ERROR "Expected file not found: ${TRICE_REVIEW_DIR}/triceConfig.h")
endif()

# Keep review preparation private: indexing and builds must not rewrite the
# canonical test source or the repository's shared TIL/LI files.
find_program(TRICE_EXECUTABLE NAMES trice HINTS "${TRICE_ROOT_DIR}/temp/bin")
if(NOT TRICE_EXECUTABLE)
  message(FATAL_ERROR "Trice is required to prepare review headers. Run scripts/buildTriceTool.sh, or configure with -DTRICE_EXECUTABLE=<path-to-trice>.")
endif()
set(TRICE_REVIEW_INPUT "${CMAKE_CURRENT_BINARY_DIR}/source/triceCheck.c")
set(TRICE_REVIEW_SIDECARS "${CMAKE_CURRENT_BINARY_DIR}/sidecars")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/source")
configure_file("${TRICE_TESTDATA_DIR}/triceCheck.c" "${TRICE_REVIEW_INPUT}" COPYONLY)
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/til.json")
  configure_file("${TRICE_ROOT_DIR}/demoTIL.json" "${CMAKE_CURRENT_BINARY_DIR}/til.json" COPYONLY)
endif()
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/li.json")
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/li.json" "")
endif()
set(TRICE_REVIEW_BIND_COMMAND
  "${TRICE_EXECUTABLE}" bind -src "${TRICE_REVIEW_INPUT}"
  -til "${CMAKE_CURRENT_BINARY_DIR}/til.json"
  -li "${CMAKE_CURRENT_BINARY_DIR}/li.json" -liRoot "${CMAKE_CURRENT_BINARY_DIR}"
  -genDir "${TRICE_REVIEW_SIDECARS}"
  -alias CUSTOM_PRINT -salias CUSTOM_ASSERT -alias printi -salias prints
)
# Configure supplies headers for the IDE immediately. The build target also
# refreshes missing or stale sidecars before any C compilation starts.
execute_process(COMMAND ${TRICE_REVIEW_BIND_COMMAND} RESULT_VARIABLE TRICE_REVIEW_BIND_STATUS)
if(NOT TRICE_REVIEW_BIND_STATUS EQUAL 0)
  message(FATAL_ERROR "Trice Bind failed for the private review source (status ${TRICE_REVIEW_BIND_STATUS}). See the diagnostics above; generated files are under ${CMAKE_CURRENT_BINARY_DIR}.")
endif()
add_custom_target(trice_review_bind COMMAND ${TRICE_REVIEW_BIND_COMMAND} VERBATIM)

# -------------------------------------------------------------------------------------
# 3) Collect Trice library C sources.
#    We deliberately glob here because you described ~40 files that should all be included.
#
#    Note:
#      If some .c files are mutually exclusive (alternative backends, platform layers, etc.),
#      you may see multiple-definition linker errors. That's a meaningful review signal,
#      but you can also exclude specific files below if you want a clean "single variant" build.
# -------------------------------------------------------------------------------------
file(GLOB TRICE_C_SOURCES CONFIGURE_DEPENDS
  "${TRICE_SRC_DIR}/*.c"
)

# Optional: Exclude known mutually exclusive or unwanted sources here.
# Example:
# list(REMOVE_ITEM TRICE_C_SOURCES
#   "${TRICE_SRC_DIR}/someAlternativeBackend.c"
#   "${TRICE_SRC_DIR}/someOtherImplementation.c"
# )

# -------------------------------------------------------------------------------------
# 4) Define the review executable target.
#    This links:
#      - Your tiny main.c (review entry point)
#      - triceCheck.c (provides TriceCheck())
#      - all Trice library .c sources from ./src
# -------------------------------------------------------------------------------------
add_executable(trice_review
  "${TRICE_REVIEW_DIR}/main.c"
  "${TRICE_REVIEW_INPUT}"
  ${TRICE_C_SOURCES}
)
add_dependencies(trice_review trice_review_bind)

# -------------------------------------------------------------------------------------
# 5) Include directories: ensure user config is found BEFORE library defaults.
#
#    Why ordering matters:
#      trice.h includes "triceConfig.h" first.
#      We want the compiler/IDE to resolve this to:
#        <repo>/_test/clion-review/triceConfig.h
#      not to any other triceConfig.h that might exist in demos/tests.
# -------------------------------------------------------------------------------------
target_include_directories(trice_review PRIVATE
  "${TRICE_REVIEW_DIR}"  # <-- MUST be first to prefer user project config
  "${TRICE_REVIEW_SIDECARS}" # private generated headers before library defaults
  "${TRICE_SRC_DIR}"     # library headers (trice.h and others), as per your repo layout
  "${TRICE_SRC_DIR}/default_conf" # fallback configuration must be searched last
)

# -------------------------------------------------------------------------------------
# 6) Warnings / diagnostics:
#    Keep them helpful but not overly aggressive by default.
#    You can tune this for your review style.
# -------------------------------------------------------------------------------------
option(TRICE_REVIEW_PEDANTIC_WARNINGS "Enable stricter warnings for review (may be noisy)" OFF)

if(TRICE_REVIEW_PEDANTIC_WARNINGS)
  if(MSVC)
    target_compile_options(trice_review PRIVATE /W4)
  else()
    target_compile_options(trice_review PRIVATE -Wall -Wextra -Wpedantic)
  endif()
else()
  if(MSVC)
    target_compile_options(trice_review PRIVATE /W3)
  else()
    target_compile_options(trice_review PRIVATE -Wall -Wextra)
  endif()
endif()

# -------------------------------------------------------------------------------------
# 7) Optional sanitizers (useful if you ever run the executable locally).
#    This is OFF by default, since your main goal is static review in CLion.
#
#    Notes:
#      - AddressSanitizer/UBSan are typically available on Linux/macOS with clang/gcc.
#      - On Windows/MinGW clang, ASan sometimes works but depends on distribution/runtime.
# -------------------------------------------------------------------------------------
option(TRICE_REVIEW_USE_SANITIZERS "Enable AddressSanitizer + UndefinedBehaviorSanitizer (where supported)" OFF)

if(TRICE_REVIEW_USE_SANITIZERS)
  if(NOT MSVC)
    target_compile_options(trice_review PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(trice_review PRIVATE -fsanitize=address,undefined)
  else()
    message(WARNING "Sanitizers are not enabled for MSVC in this template. Use clang-cl or adjust flags if needed.")
  endif()
endif()

# -------------------------------------------------------------------------------------
# 8) Platform notes / portability:
#    No platform-specific code is forced here. Toolchain selection is external:
#      - Linux/ma
