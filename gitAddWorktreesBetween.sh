#!/bin/bash
# Usage:
#   ./gitAddWorktreesBetween.sh <older-hash> <newer-hash>
#   or
#   ./gitAddWorktreesBetween.sh "<since-date>" "<until-date>"
#
# Requires: AddWorktreeFromGitLogLineData.sh in the same directory

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKTREE_SCRIPT="$SCRIPT_DIR/AddWorktreeFromGitLogLineData.sh"

if [ $# -ne 2 ]; then
	echo "Usage: $0 <older-hash|since-date> <newer-hash|until-date>"
	exit 1
fi

START=$1
END=$2

# Detect whether inputs are commit hashes or dates
if [[ "$START" =~ ^[0-9a-fA-F]{6,40}$ && "$END" =~ ^[0-9a-fA-F]{6,40}$ ]]; then
	echo "🔍 Using commit range: $START..$END"
	COMMITS=$(git log --reverse --pretty=format:'%H %ad' --date=format:'%Y-%m-%d %H:%M' "$START".."$END")
else
	echo "🕒 Using date range: $START → $END"
	COMMITS=$(git log --reverse --pretty=format:'%H %ad' --date=format:'%Y-%m-%d %H:%M' --since="$START" --until="$END")
fi

if [ -z "$COMMITS" ]; then
	echo "⚠️  No commits found in range."
	exit 0
fi

echo "➡️  Creating worktrees for $(echo "$COMMITS" | wc -l) commits..."

# Loop over commits
echo "$COMMITS" | while read -r HASH DATE TIME; do
	# Construct expected worktree folder name (based on AddWorktreeFromGitLogLineData.sh logic)
	DATE_PART=$(echo "$DATE" | cut -d'-' -f2-3)
	TIME_PART=$(echo "$TIME" | tr ':' '-')
	BRANCHES=$(git branch --contains "$HASH" 2>/dev/null | sed 's/^[* ]*//')
	[ -z "$BRANCHES" ] && BRANCHES="noBranch"
	BRANCHES_CLEAN=$(echo "$BRANCHES" | tr '\n' '+' | tr -d ' ' | tr '/' '_' | sed 's/+$//')

	SHORT_HASH=$(git rev-parse --short "$HASH")
	NAME="trice_${DATE_PART}-${TIME_PART}_${SHORT_HASH}_${BRANCHES_CLEAN}"

	DIR="../${NAME}"

	if [ -d "$DIR" ]; then
		echo "⏭️  Skipping existing worktree: $DIR"
		continue
	fi

	echo ""
	echo "🧱 Processing commit $HASH ($DATE $TIME)"
	bash "$WORKTREE_SCRIPT" "$HASH" "$DATE" "$TIME"
done

echo ""
echo "✅ All worktrees created (existing ones skipped)."
