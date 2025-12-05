#!/bin/bash
# Show git log with graph and the branch (from name-rev) of each commit.
# Usage:
#   ./GitLogWithBranches.sh [--since YYYY-MM-DD]

SINCE=""
if [ "$1" == "--since" ] && [ -n "$2" ]; then
	SINCE="--since=$2"
fi

# ANSI colors
yellow='\033[1;33m'
green='\033[1;32m'
cyan='\033[1;36m'
white='\033[1;37m'
reset='\033[0m'

# Collect graph and hashes separately (keep graph layout intact)
GRAPH_LINES=()
HASHES=()

# Fill arrays line by line
while IFS= read -r line; do
	GRAPH_LINES+=("${line//[0-9a-f]/ }") # replace Hash with spaces
done < <(git log --graph --pretty=format:'%h' --date=short $SINCE)

while IFS= read -r hash; do
	HASHES+=("$hash")
done < <(git log --pretty=format:'%H' $SINCE)

# Print each line with full info
for i in "${!GRAPH_LINES[@]}"; do
	graph="${GRAPH_LINES[$i]}"
	fullhash="${HASHES[$i]}"

	# Skip if fullhash is empty
	[ -z "$fullhash" ] && continue

	short=${fullhash:0:8} # for display

	# Extract info
	info=$(git log -1 --date=format:'%Y-%m-%d %H:%M' \
		--pretty=format:'%ad|%s' "$fullhash")
	date="${info%%|*}"
	subject="${info#*|}"

	# Determine nearest branch name (ignore remotes/)
	branch=$(git name-rev --name-only "$fullhash" 2>/dev/null | sed -E 's#^remotes/origin/##')
	[ -z "$branch" ] && branch="(no branch)"
	branch=$(printf "%.32s" "$branch")

	# Output formatted, keeping graph intact
	printf "${white}%-16s${yellow}%-8s${reset} ${green}%-16s${reset} [${cyan}%-32s${reset}] %s\n" \
		"$graph" "$short" "$date" "$branch" "$subject"
done

###############################################################################
# OK, no graph display
#

# Show git log with branch info determined by 'git name-rev'.
# Usage:
#   ./GitLogWithBranches.sh [--since YYYY-MM-DD]
#
# SINCE=""
# if [ "$1" == "--since" ] && [ -n "$2" ]; then
#   SINCE="--since=$2"
# fi
#
# # Define colors (compatible with Git's pretty format)
# yellow='\033[1;33m'
# green='\033[1;32m'
# cyan='\033[1;36m'
# white='\033[1;37m'
# reset='\033[0m'
#
# # Collect commits
# git log $SINCE --pretty=format:'%h|%ad|%s' --date=format:'%Y-%m-%d %H:%M' |
# while IFS='|' read -r hash date subject; do
#   # Determine branch(es) name(s)
#   branch=$(git name-rev --name-only "$hash" 2>/dev/null \
#            | sed -E 's#^remotes/origin/##')
#
#   # Optional: fall back if no branch found
#   if [ -z "$branch" ]; then
#     branch="(no branch)"
#   fi
#
#   # Print formatted output with colors
#   printf "${yellow}%-10s${reset}  ${green}%-16s${reset} [${cyan}%-15s${reset}] ${white}%s${reset}\n" \
#          "$hash" "$date" "$branch" "$subject"
# done
#
#
###############################################################################

# ###############################################################################
# # OK, with graph display
# #
# # Show git log with graph and the branch (from name-rev) of each commit.
# # Usage:
# #   ./GitLogWithBranches.sh [--since YYYY-MM-DD]
#
# SINCE=""
# if [ "$1" == "--since" ] && [ -n "$2" ]; then
#   SINCE="--since=$2"
# fi
#
# # ANSI colors
# yellow='\033[1;33m'
# green='\033[1;32m'
# cyan='\033[1;36m'
# white='\033[1;37m'
# reset='\033[0m'
#
# # Collect graph and hashes separately (keep graph layout intact)
# GRAPH_LINES=()
# HASHES=()
#
# # Fill arrays line by line
# while IFS= read -r line; do
#   GRAPH_LINES+=("${line//[0-9a-f]/ }")  # replace Hash with spaces
# done < <(git log --graph --pretty=format:'%h' --date=short $SINCE)
#
# while IFS= read -r hash; do
#   HASHES+=("$hash")
# done < <(git log --pretty=format:'%H' $SINCE)
#
# # Print each line with full info
# for i in "${!GRAPH_LINES[@]}"; do
#   graph="${GRAPH_LINES[$i]}"
#   fullhash="${HASHES[$i]}"
#   short=${fullhash:0:8}   # for display
#
#   # Extract info
#   info=$(git log -1 --date=format:'%Y-%m-%d %H:%M' \
#                 --pretty=format:'%ad|%s' "$fullhash")
#   date="${info%%|*}"
#   subject="${info#*|}"
#
#   # Determine nearest branch name (ignore remotes/)
#   branch=$(git name-rev --name-only "$fullhash" 2>/dev/null | sed -E 's#^remotes/origin/##')
#   [ -z "$branch" ] && branch="(no branch)"
#   branch=$(printf "%.32s" "$branch")
#
#   # Output formatted, keeping graph intact
#   printf "${white}%-16s${yellow}%-8s${reset} ${green}%-16s${reset} [${cyan}%-32s${reset}] %s\n" \
#          "$graph" "$short" "$date" "$branch" "$subject"
# done
#
#
###############################################################################
