#!/usr/bin/env bash
#
# Step 07: Runs Go coverage and a temp-local Trice tool build.
#
# Direct invocation:
# - ./_testAll_07_GoCoverage.sh
# If Go is not installed locally, the step is marked as SKIP.
#
# Notes:
# - The coverage run instruments normal repository packages globally.
# - The dedicated CGO target matrix under ./_test runs only in step 10, where
#   Bulk-first ordering and line-by-line diagnostics are controlled explicitly.
# - The follow-up tool build now writes explicitly into the disposable
#   TRICE_BIN_DIR instead of relying on go install destination resolution.
# - This keeps any user-installed trice binary outside the test sandbox
#   untouched, even when the step fails or is interrupted.
#
# Log file:
# - ./temp/log/_testAll_07_GoCoverage.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

print_global_coverage_hotspots() {
  # Summarize the raw coverprofile by package and file using uncovered
  # statement counts. This is the useful view for improving the overall
  # repository coverage because the global run uses -coverpkg=./....
  local coverage_file="$1"

  log "Top packages by uncovered statements (global coverpkg view):"
  awk -F'[: ,]+' '
    NR == 1 { next }
    {
      file = $1
      gsub(/^github.com\/rokath\/trice\//, "", file)
      block = file ":" $2 "," $3
      n = split(file, parts, "/")
      pkg = (n > 1) ? parts[1] : "."
      for (i = 2; i < n; i++) {
        pkg = pkg "/" parts[i]
      }
      block_pkg[block] = pkg
      block_stmts[block] = $(NF-1) + 0
      if ($NF + 0 > 0) {
        block_hit[block] = 1
      }
    }
    END {
      for (block in block_stmts) {
        pkg = block_pkg[block]
        stmts = block_stmts[block]
        pkg_total[pkg] += stmts
        if (!block_hit[block]) {
          pkg_uncovered[pkg] += stmts
        }
      }
      for (pkg in pkg_total) {
        uncovered = pkg_uncovered[pkg] + 0
        covered_pct = (pkg_total[pkg] - uncovered) * 100 / pkg_total[pkg]
        printf "%8d\t%7.1f%%\t%8d\t%s\n", uncovered, covered_pct, pkg_total[pkg], pkg
      }
    }
  ' "$coverage_file" | sort -rn | awk 'BEGIN {
      printf "  %8s  %7s  %8s  %s\n", "uncovered", "covered", "statements", "package"
    }
    NR <= 15 {
      printf "  %8s  %7s  %8s  %s\n", $1, $2, $3, $4
    }' | log_pipe

  log "Top files by uncovered statements (global coverpkg view):"
  awk -F'[: ,]+' '
    NR == 1 { next }
    {
      file = $1
      gsub(/^github.com\/rokath\/trice\//, "", file)
      block = file ":" $2 "," $3
      block_file[block] = file
      block_stmts[block] = $(NF-1) + 0
      if ($NF + 0 > 0) {
        block_hit[block] = 1
      }
    }
    END {
      for (block in block_stmts) {
        file = block_file[block]
        stmts = block_stmts[block]
        file_total[file] += stmts
        if (!block_hit[block]) {
          file_uncovered[file] += stmts
        }
      }
      for (file in file_total) {
        uncovered = file_uncovered[file] + 0
        covered_pct = (file_total[file] - uncovered) * 100 / file_total[file]
        printf "%8d\t%7.1f%%\t%8d\t%s\n", uncovered, covered_pct, file_total[file], file
      }
    }
  ' "$coverage_file" | sort -rn | awk 'BEGIN {
      printf "  %8s  %7s  %8s  %s\n", "uncovered", "covered", "statements", "file"
    }
    NR <= 20 {
      printf "  %8s  %7s  %8s  %s\n", $1, $2, $3, $4
    }' | log_pipe
}

main() {
  local coverage_file="./temp/log/coverage.out"
  local coverage_packages=""
  local goexe
  local go_toolchain
  local package
  local package_list
  local packages=()

  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  # Keep nested Go commands on the toolchain selected for this module. Go's
  # coverage implementation invokes "go tool covdata" for packages without
  # tests; an older PATH launcher must not combine itself with the selected
  # toolchain's GOROOT after an automatic toolchain switch.
  go_toolchain="$(go env GOVERSION)" || {
    log "FAIL: unable to determine the selected Go toolchain"
    exit 1
  }
  log "Go coverage toolchain: $go_toolchain"

  # Step 10 owns the PC/CGO packages transactionally. Testing them during
  # coverage would run their historical per-folder modes before the ordered PC
  # pass and would dominate quick-test duration. Keep all normal Go packages as
  # both test targets and coverage targets instead.
  log "+ go list ./..."
  package_list="$(go list ./... 2>>"$LOGFILE")" || {
    log "FAIL: go list ./... failed"
    exit 1
  }
  for package in $package_list; do
    case "$package" in
      */_test/*) continue ;;
    esac
    packages+=("$package")
    if [ -n "$coverage_packages" ]; then
      coverage_packages="$coverage_packages,$package"
    else
      coverage_packages="$package"
    fi
  done
  if [ "${#packages[@]}" -eq 0 ] || [ -z "$coverage_packages" ]; then
    log "FAIL: go list returned no non-PC-test packages for coverage"
    exit 1
  fi
  run_cmd env "GOTOOLCHAIN=$go_toolchain" go test "${packages[@]}" -covermode=atomic -coverprofile="$coverage_file" "-coverpkg=$coverage_packages" || {
    log "FAIL: go coverage test failed"
    exit 1
  }
  run_cmd go tool cover -func="$coverage_file" || {
    log "FAIL: go tool cover -func failed"
    exit 1
  }
  print_global_coverage_hotspots "$coverage_file"
  goexe="$(go env GOEXE)"
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  run_cmd "$SCRIPTS_DIR/buildTriceTool.sh" --target-file "$TRICE_BIN_DIR/trice${goexe}" --force --no-backup --silent || {
    log "FAIL: temp-local trice build failed"
    exit 1
  }
}

main "$@"
