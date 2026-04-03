#!/bin/bash

# Show the git log with graph information and the nearest branch name for each
# commit. The script is intentionally optimized for changelog maintenance:
# it keeps the ASCII graph intact, adds a formatted date column, and supports
# both date-based and Git-range-based filtering.

set -euo pipefail

show_usage() {
  cat <<'EOF'
Usage:
  ./gitLogWithBranches.sh [options]

Options:
  --since YYYY-MM-DD   Include commits on or after the given date.
  --until YYYY-MM-DD   Include commits on or before the given date.
  --range A..B         Use an explicit Git revision range, for example
                       v1.1.1..v1.2.0.
  --color              Force ANSI colors even when stdout is not a terminal.
  --no-color           Disable ANSI colors completely.
  -h, --help           Show this help text.

Notes:
  - If neither --color nor --no-color is given, colors are enabled only when
    stdout is connected to a terminal.
  - --range is combined with --since/--until if you provide both. This allows
    additional date trimming of a Git revision range if desired.
EOF
}

resolve_revision_name() {
  local rev="$1"

  if git rev-parse --verify --quiet "$rev^{commit}" >/dev/null; then
    printf '%s\n' "$rev"
    return 0
  fi

  # This repository contains early tags like "v0.1.00" and "v0.2.00".
  # Accept the more common shorthand form "v0.1.0" by trying a padded patch
  # component when the exact name does not exist.
  if [[ "$rev" =~ ^v([0-9]+)\.([0-9]+)\.([0-9])$ ]]; then
    local padded="v${BASH_REMATCH[1]}.${BASH_REMATCH[2]}.0${BASH_REMATCH[3]}"
    if git rev-parse --verify --quiet "$padded^{commit}" >/dev/null; then
      printf '%s\n' "$padded"
      return 0
    fi
  fi

  return 1
}

resolve_range_argument() {
  local raw_range="$1"
  local separator=""
  local left=""
  local right=""
  local resolved_left=""
  local resolved_right=""

  if [[ "$raw_range" == *"..."* ]]; then
    separator="..."
    left="${raw_range%%...*}"
    right="${raw_range#*...}"
  elif [[ "$raw_range" == *".."* ]]; then
    separator=".."
    left="${raw_range%%..*}"
    right="${raw_range#*..}"
  else
    if resolved_left=$(resolve_revision_name "$raw_range"); then
      printf '%s\n' "$resolved_left"
      return 0
    fi
    return 1
  fi

  if [[ -n "$left" ]]; then
    resolved_left=$(resolve_revision_name "$left") || return 1
  fi

  if [[ -n "$right" ]]; then
    resolved_right=$(resolve_revision_name "$right") || return 1
  fi

  # For this helper, a leading-empty double-dot range means "show history up to
  # the right-side revision". Raw Git interprets "..X" as "HEAD..X", which is
  # often empty for old tags and not useful for changelog backfills.
  if [[ "$separator" == ".." && -z "$left" && -n "$resolved_right" ]]; then
    printf '%s\n' "$resolved_right"
    return 0
  fi

  printf '%s%s%s\n' "$resolved_left" "$separator" "$resolved_right"
}

# These variables collect the filtering options that will later be passed to
# "git log". Arrays are used intentionally to preserve exact shell argument
# boundaries without relying on word splitting.
since_arg=""
until_arg=""
range_arg=""
color_mode="auto"

while [[ $# -gt 0 ]]; do
  case "$1" in
  --since)
    if [[ $# -lt 2 ]]; then
      echo "Error: --since requires a date argument." >&2
      exit 1
    fi
    since_arg="--since=$2"
    shift 2
    ;;
  --until)
    if [[ $# -lt 2 ]]; then
      echo "Error: --until requires a date argument." >&2
      exit 1
    fi
    until_arg="--until=$2"
    shift 2
    ;;
  --range)
    if [[ $# -lt 2 ]]; then
      echo "Error: --range requires a Git revision range argument." >&2
      exit 1
    fi
    range_arg="$2"
    shift 2
    ;;
  --color)
    color_mode="always"
    shift
    ;;
  --no-color)
    color_mode="never"
    shift
    ;;
  -h | --help)
    show_usage
    exit 0
    ;;
  *)
    echo "Error: unknown argument '$1'." >&2
    echo >&2
    show_usage >&2
    exit 1
    ;;
  esac
done

# Decide whether ANSI colors should be used.
# - "always": force colors
# - "never":  disable colors completely
# - "auto":   enable colors only when stdout is an interactive terminal
use_color=0
case "$color_mode" in
always)
  use_color=1
  ;;
never)
  use_color=0
  ;;
auto)
  if [[ -t 1 ]]; then
    use_color=1
  fi
  ;;
esac

if [[ "$use_color" -eq 1 ]]; then
  yellow=$'\033[1;33m'
  green=$'\033[1;32m'
  cyan=$'\033[1;36m'
  white=$'\033[1;37m'
  reset=$'\033[0m'
else
  yellow=''
  green=''
  cyan=''
  white=''
  reset=''
fi

# Build the common "git log" argument list once and reuse it for all log
# queries below. The order is:
#   1. date filters
#   2. optional Git revision range
git_log_args=()

if [[ -n "$since_arg" ]]; then
  git_log_args+=("$since_arg")
fi

if [[ -n "$until_arg" ]]; then
  git_log_args+=("$until_arg")
fi

if [[ -n "$range_arg" ]]; then
  raw_range_arg="$range_arg"
  if ! range_arg=$(resolve_range_argument "$range_arg"); then
    echo "Error: could not resolve Git revision range '$raw_range_arg'." >&2
    echo "Hint: early release tags in this repo use forms like 'v0.1.00' and 'v0.2.00'." >&2
    exit 1
  fi
  git_log_args+=("$range_arg")
fi

# The graph returned by "git log --graph" contains the abbreviated hashes in
# the same physical positions where the graph is drawn. To keep the graph
# alignment intact, we first capture those lines and replace hexadecimal digits
# with spaces. The commit hashes themselves are collected separately in a second
# pass. Both result arrays have identical ordering.
GRAPH_LINES=()
HASHES=()

while IFS= read -r line; do
  GRAPH_LINES+=("${line//[0-9a-f]/ }")
done < <(git log --graph --pretty=format:'%h' "${git_log_args[@]}")

while IFS= read -r hash; do
  HASHES+=("$hash")
done < <(git log --pretty=format:'%H' "${git_log_args[@]}")

# Print each line in a stable, easy-to-scan format:
#   graph | short hash | commit date | nearest branch | subject
for i in "${!GRAPH_LINES[@]}"; do
  graph="${GRAPH_LINES[$i]}"
  fullhash="${HASHES[$i]}"

  # Defensive guard: skip empty entries if Git ever returns an unexpected
  # mismatch between graph lines and collected hashes.
  [[ -z "$fullhash" ]] && continue

  short="${fullhash:0:8}"

  # Extract the formatted commit date and subject in one Git call.
  info=$(git log -1 --date=format:'%Y-%m-%d %H:%M' \
    --pretty=format:'%ad|%s' "$fullhash")
  date="${info%%|*}"
  subject="${info#*|}"

  # Determine the nearest symbolic name for the commit. Remote prefixes are
  # removed because they make the branch column noisy and are usually not
  # helpful for changelog work.
  branch=$(git name-rev --name-only "$fullhash" 2>/dev/null | sed -E 's#^remotes/origin/##')
  [[ -z "$branch" ]] && branch="(no branch)"
  branch=$(printf "%.32s" "$branch")

  printf "${white}%-16s${yellow}%-8s${reset} ${green}%-16s${reset} [${cyan}%-32s${reset}] %s\n" \
    "$graph" "$short" "$date" "$branch" "$subject"
done
