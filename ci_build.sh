#!/usr/bin/env bash
#
# ci_build.sh
#
# Purpose:
#   CI orchestrator script that builds all example targets using the existing
#   project scripts without modifying them.
#
# Design principles:
#   - Keep existing scripts unchanged (build.sh, build_with_clang.sh, buildAllTargets_*.sh).
#   - Make CI deterministic by explicitly selecting toolchain tools via environment variables
#     (e.g. OBJCOPY) and by validating tool availability early.
#   - Provide CLI switches to control what is built, including TRICE ON/OFF and whether to run
#     the long configuration sweep in L432_inst.
#
# Typical usages:
#   ./ci_build.sh --toolchain=gcc  --trice=on
#   ./ci_build.sh --toolchain=gcc  --trice=off
#   ./ci_build.sh --toolchain=clang --trice=on          # only builds the clang target(s) that exist
#   ./ci_build.sh --toolchain=gcc  --trice=on --configs=quick
#   ./ci_build.sh --toolchain=gcc  --trice=on --configs=full
#
# Notes:
#   - Your repository already has:
#       examples/buildAllTargets_TRICE_ON.sh
#       examples/buildAllTargets_TRICE_OFF.sh
#     which cover all targets using GCC (build.sh).
#   - The only explicit clang build script you showed is:
#       examples/G0B1_inst/build_with_clang.sh
#     Therefore, --toolchain=clang currently runs only that target unless you add more.
#
set -euo pipefail

###############################################################################
# Defaults
###############################################################################
TOOLCHAIN="gcc"        # gcc | clang
TRICE="on"             # on | off
CONFIGS_MODE="none"    # none | quick | full
CONFIGS_START=0        # used only for quick/full mode
CONFIGS_END=100        # used only for quick/full mode (inclusive)
EXAMPLES_DIR="examples"

###############################################################################
# Helper functions
###############################################################################
usage() {
  cat <<'EOF'
Usage: ./ci_build.sh [options]

Options:
  --toolchain=gcc|clang
      Select what to run.
      - gcc  : runs examples/buildAllTargets_TRICE_{ON|OFF}.sh
      - clang: runs clang build(s) that exist (currently only examples/G0B1_inst/build_with_clang.sh)

  --trice=on|off
      Controls whether the GCC "all targets" build runs with TRICE_OFF=1 or not.
      For clang builds, the existing build_with_clang.sh always inserts IDs; we do not alter it.

  --configs=none|quick|full
      Controls running the long configuration sweep in examples/L432_inst/all_configs_build.sh.
      - none : do not run configuration sweep
      - quick: run a shortened sweep (CONFIGURATION range controlled by --configs-start/--configs-end)
      - full : run the full sweep (default 0..100)

  --configs-start=N
  --configs-end=N
      Configuration range for quick mode (inclusive). Ignored unless --configs=quick.

  --examples-dir=PATH
      Path to the examples folder (default: ./examples)

  -h, --help
      Show help.

Environment:
  OBJCOPY
      If set, it will be used by Makefiles that respect OBJCOPY.
      If not set, this script will select a sensible default (preferring arm-none-eabi-objcopy).

EOF
}

log() { echo "[ci_build] $*"; }

die() {
  echo "[ci_build] ERROR: $*" >&2
  exit 2
}

require_cmd() {
  local c="$1"
  command -v "$c" >/dev/null 2>&1 || die "Required command not found in PATH: $c"
}

select_objcopy() {
  # Prefer the ARM cross objcopy for bare-metal ARM outputs.
  if command -v arm-none-eabi-objcopy >/dev/null 2>&1; then
    echo "arm-none-eabi-objcopy"
    return 0
  fi

  # Fallback to LLVM objcopy if present.
  if command -v llvm-objcopy >/dev/null 2>&1; then
    echo "llvm-objcopy"
    return 0
  fi

  # Fallback to GNU objcopy variants.
  if command -v gobjcopy >/dev/null 2>&1; then
    echo "gobjcopy"
    return 0
  fi
  if command -v objcopy >/dev/null 2>&1; then
    echo "objcopy"
    return 0
  fi

  return 1
}

run_with_status() {
  # Run a command and return its exit code without stopping the script.
  # This allows us to accumulate failures across multiple targets.
  set +e
  "$@"
  local rc=$?
  set -e
  return $rc
}

###############################################################################
# Argument parsing (simple, explicit)
###############################################################################
for arg in "$@"; do
  case "$arg" in
    --toolchain=*)
      TOOLCHAIN="${arg#*=}"
      ;;
    --trice=*)
      TRICE="${arg#*=}"
      ;;
    --configs=*)
      CONFIGS_MODE="${arg#*=}"
      ;;
    --configs-start=*)
      CONFIGS_START="${arg#*=}"
      ;;
    --configs-end=*)
      CONFIGS_END="${arg#*=}"
      ;;
    --examples-dir=*)
      EXAMPLES_DIR="${arg#*=}"
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      die "Unknown argument: $arg (use --help)"
      ;;
  esac
done

###############################################################################
# Validate options
###############################################################################
case "$TOOLCHAIN" in
  gcc|clang) ;;
  *) die "--toolchain must be gcc or clang (got: $TOOLCHAIN)" ;;
esac

case "$TRICE" in
  on|off) ;;
  *) die "--trice must be on or off (got: $TRICE)" ;;
esac

case "$CONFIGS_MODE" in
  none|quick|full) ;;
  *) die "--configs must be none, quick, or full (got: $CONFIGS_MODE)" ;;
esac

if [[ ! -d "$EXAMPLES_DIR" ]]; then
  die "Examples directory not found: $EXAMPLES_DIR"
fi

###############################################################################
# Toolchain sanity checks and deterministic tool selection
###############################################################################
log "Toolchain=$TOOLCHAIN, TRICE=$TRICE, CONFIGS=$CONFIGS_MODE, EXAMPLES_DIR=$EXAMPLES_DIR"

# We expect at least make to be present for any build.
require_cmd make

# Select a deterministic OBJCOPY unless the caller already set one.
if [[ -z "${OBJCOPY:-}" ]]; then
  if objcopy_bin="$(select_objcopy)"; then
    export OBJCOPY="$objcopy_bin"
    log "Selected OBJCOPY=$OBJCOPY"
  else
    log "WARNING: No objcopy found; .hex/.bin generation may fail."
  fi
else
  log "Using pre-set OBJCOPY=$OBJCOPY"
fi

# Ensure compiler presence depending on mode.
if [[ "$TOOLCHAIN" == "gcc" ]]; then
  require_cmd arm-none-eabi-gcc
elif [[ "$TOOLCHAIN" == "clang" ]]; then
  require_cmd clang
fi

###############################################################################
# Run builds
###############################################################################
fail_count=0

if [[ "$TOOLCHAIN" == "gcc" ]]; then
  # For GCC we use your existing all-target scripts unchanged.
  if [[ "$TRICE" == "on" ]]; then
    script="$EXAMPLES_DIR/buildAllTargets_TRICE_ON.sh"
  else
    script="$EXAMPLES_DIR/buildAllTargets_TRICE_OFF.sh"
  fi

  [[ -x "$script" ]] || die "Not executable or not found: $script"

  log "Running: $script"
  if ! run_with_status bash "$script"; then
    fail_count=$((fail_count + 1))
    log "FAIL: $script"
  fi
fi

if [[ "$TOOLCHAIN" == "clang" ]]; then
  # Currently only one clang build entry point is known.
  # If you add more clang build scripts later, extend this section.
  clang_script="$EXAMPLES_DIR/G0B1_inst/build_with_clang.sh"
  [[ -x "$clang_script" ]] || die "Not executable or not found: $clang_script"

  log "Running: $clang_script"
  if ! run_with_status bash "$clang_script"; then
    fail_count=$((fail_count + 1))
    log "FAIL: $clang_script"
  fi
fi

###############################################################################
# Optional: configuration sweep in L432_inst
###############################################################################
# The repository has:
#   examples/L432_inst/all_configs_build.sh  (runs CONFIGURATION=0..100)
#
# In CI, a full sweep can be time-consuming. Therefore:
# - --configs=none : skip
# - --configs=quick: run a limited range (defaults from --configs-start/--configs-end)
# - --configs=full : run 0..100 exactly as the script does (without changing it)
#
if [[ "$CONFIGS_MODE" != "none" ]]; then
  cfg_dir="$EXAMPLES_DIR/L432_inst"
  cfg_script="$cfg_dir/all_configs_build.sh"
  [[ -d "$cfg_dir" ]] || die "Not found: $cfg_dir"
  [[ -x "$cfg_script" ]] || die "Not executable or not found: $cfg_script"

  if [[ "$CONFIGS_MODE" == "full" ]]; then
    log "Running full configuration sweep: $cfg_script (0..100 as implemented)"
    if ! run_with_status bash -lc "cd '$cfg_dir' && ./all_configs_build.sh"; then
      fail_count=$((fail_count + 1))
      log "FAIL: full configuration sweep"
    fi
  fi

  if [[ "$CONFIGS_MODE" == "quick" ]]; then
    # We keep your all_configs_build.sh unchanged and implement a CI-specific
    # shortened sweep here.
    #
    # IMPORTANT: The existing build.sh in L432_inst accepts CONFIGURATION=$i.
    # We replicate the loop with a configurable range.
    log "Running QUICK configuration sweep in $cfg_dir: CONFIGURATION=$CONFIGS_START..$CONFIGS_END"

    for ((i=CONFIGS_START; i<=CONFIGS_END; i++)); do
      log "CONFIGURATION=$i"
      if ! run_with_status bash -lc "cd '$cfg_dir' && make clean && ./build.sh CONFIGURATION=$i"; then
        fail_count=$((fail_count + 1))
        log "FAIL: L432_inst CONFIGURATION=$i"
        # In quick mode, we stop early on first failure to reduce noise.
        break
      fi
    done
  fi
fi

###############################################################################
# Final status
###############################################################################
if [[ "$fail_count" -ne 0 ]]; then
  die "$fail_count build step(s) failed"
fi

log "All requested build steps succeeded."
