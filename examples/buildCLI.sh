#!/bin/sh
# Shared argument handling for the paired GCC and Clang example entry points.
# Loading this file has no side effects; parse before source preparation.

# example_build_usage describes the common interface and project-specific range.
example_build_usage() {
  printf 'Usage: bash ./%s [configuration|CONFIGURATION=configuration] [NAME[=VALUE] ...]\n' "${example_build_entry##*/}"
  printf '       bash ./%s --help | --check | --x0-matrix\n' "${example_build_entry##*/}"
  printf 'Without a configuration, keep the project default. Builds do not flash.\n'
  printf 'Firmware uses -O3; images remain in out.gcc or out.clang.\n'
  printf 'Supported configurations: %s\n' "$example_build_profiles"
  printf '%s\n' '--check checks prerequisites without preparing sources or building.'
  printf '%s\n' '--x0-matrix builds all four TRICE_OFF/typeX0 combinations with this compiler.'
  printf 'See docs/rm/examples.md#%s (from the repository root).\n' "$example_build_manual"
}

# example_build_error rejects an argument before tools or generated files change.
example_build_error() {
  printf 'error: %s\n' "$1" >&2
  example_build_usage >&2
  return 2
}

# parse_example_build_cli publishes flags, configuration, triceOFF and check_only
# for existing build bodies. Each entry supplies its actual supported profiles;
# "none" means the project has no selectable CONFIGURATION profiles.
parse_example_build_cli() {
  example_build_entry=$1
  example_build_profiles=$2
  example_build_manual=$3
  shift 3
  flags=""
  configuration=""
  triceOFF=0
  check_only=0

  # Control options stand alone, so a misspelled build cannot prepare sources.
  if [ "$#" -eq 1 ]; then
    case "$1" in
      --help | -h) example_build_usage; exit 0 ;;
      --check) check_only=1; return 0 ;;
      --x0-matrix)
        # Re-enter the same compiler script with ordinary define arguments.
        for example_build_off in 0 1; do
          for example_build_x0 in 0 1; do
            printf 'x0-matrix: TRICE_OFF=%s TRICE_TX_X0_COUNTED_BUFFER_SUPPORT=%s\n' "$example_build_off" "$example_build_x0"
            bash "$example_build_entry" "TRICE_OFF=$example_build_off" "TRICE_TX_X0_COUNTED_BUFFER_SUPPORT=$example_build_x0" || return $?
          done
        done
        exit 0
        ;;
    esac
  fi

  for example_build_argument in "$@"; do
    case "$example_build_argument" in
      CONFIGURATION=* | [0-9]* | -[0-9]*)
        if [ -n "$configuration" ]; then
          example_build_error 'select only one configuration.'; return 2
        fi
        configuration=${example_build_argument#CONFIGURATION=}
        case "$configuration" in
          0 | [1-9] | [1-9][0-9] | 100) ;;
          *) example_build_error "configuration must be a decimal number from 0 through 100; received: $configuration"; return 2 ;;
        esac
        case "$example_build_profiles" in
          '0 through 100') ;;
          '0 or 98')
            case "$configuration" in
              0 | 98) ;;
              *) example_build_error "this project supports configuration 0 or 98; received: $configuration"; return 2 ;;
            esac
            ;;
          *) example_build_error 'this project has no selectable configurations.'; return 2 ;;
        esac
        example_build_argument="CONFIGURATION=$configuration"
        ;;
      -*) example_build_error "unknown or combined control option: $example_build_argument"; return 2 ;;
      *)
        # Validate the macro name, leaving the existing define value untouched.
        example_build_name=${example_build_argument%%=*}
        case "$example_build_name" in
          '' | [!a-zA-Z_]* | *[!a-zA-Z0-9_]*)
            example_build_error "expected NAME or NAME=VALUE; received: $example_build_argument"; return 2 ;;
        esac
        ;;
    esac
    [ "$example_build_argument" != TRICE_OFF=1 ] || triceOFF=1
    flags="${flags}-D${example_build_argument} "
  done
}

# check_example_build_tools performs read-only PATH/backend checks. L432 Clang
# retains its more detailed hard-float header/runtime checks in its entry point.
check_example_build_tools() {
  for example_build_tool in make arm-none-eabi-gcc arm-none-eabi-objcopy arm-none-eabi-size; do
    command -v "$example_build_tool" >/dev/null 2>&1 || {
      example_build_error "required tool not found on PATH: $example_build_tool"; return 1
    }
  done
  if [ "${TRICE_ID_WORKFLOW_OWNER:-0}" != 1 ]; then
    command -v trice >/dev/null 2>&1 || {
      example_build_error 'required tool not found on PATH: trice'; return 1
    }
  fi
  example_build_make_version=$(make --version) || return $?
  case "$example_build_make_version" in
    *'GNU Make '*) ;;
    *) example_build_error 'the make command is not GNU make.'; return 1 ;;
  esac
  if [ "$1" = clang ]; then
    command -v clang >/dev/null 2>&1 || {
      example_build_error 'required tool not found on PATH: clang'; return 1
    }
    example_build_targets=$(clang --print-targets) || return $?
    printf '%s\n' "$example_build_targets" | awk '$1 == "thumb" { found=1 } END { exit !found }' || {
      example_build_error 'Clang does not provide the required Thumb backend.'; return 1
    }
    # Both G0B1 Makefiles use the size utility selected by the Clang driver.
    example_build_size=$(clang --print-prog-name=llvm-size) || return $?
    command -v "$example_build_size" >/dev/null 2>&1 || {
      example_build_error "required Clang size utility not found: $example_build_size"; return 1
    }
  fi
  printf 'Prerequisites OK.\n'
}
