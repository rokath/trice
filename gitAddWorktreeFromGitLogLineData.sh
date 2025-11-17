#!/bin/bash
# Usage: ./gitAddWorktreeFromGitLogLineData.sh <commit-hash> <YYYY-MM-DD> <HH:MM>
# Example: ./gitAddWorktreeFromGitLogLineData.sh cb9b06026 2025-08-18 12:21

# Use git log --graph --decorate master --pretty=format:'%C(bold yellow)%h%Creset %C(bold green)%ad%Creset %C(bold blue)%an%Creset %C(bold cyan)%d%Creset %C(white)%s%Creset' --date=format:'%Y-%m-%d %H:%M'
# to get one-line git logs from branch master, wer you can snip <commit-hash> <YYYY-MM-DD> <HH:MM>

# Check argument count
if [ $# -ne 3 ]; then
  echo "Usage: $0 <commit-hash> <YYYY-MM-DD> <HH:MM>"
  exit 1
fi

HASH=$1
DATE=$2
TIME=$3

SHORT_HASH=$(git rev-parse --short "$HASH")

# Format date and time: YYYY-MM-DD → MM-DD, HH:MM → HH-MM
DATE_PART=$(echo "$DATE" | cut -d'-' -f2-3)
TIME_PART=$(echo "$TIME" | tr ':' '-')

# Try to find the original branch name for this commit
RAW_NAME=$(git name-rev --name-only "$HASH" 2>/dev/null)

if [ -n "$RAW_NAME" ]; then
  # Clean up: remove 'remotes/origin/', 'remotes/', 'heads/' prefixes and trailing '~<n>'
  BRANCH=$(echo "$RAW_NAME" | sed -E 's#^remotes/origin/##' | sed -E 's#^remotes/##' | sed -E 's#^heads/##' | sed -E 's#~[0-9]+$##')
else
  BRANCH="noBranch"
fi

# Construct branch and directory name
NAME="trice_${DATE_PART}-${TIME_PART}_${SHORT_HASH}_${BRANCH}"
DIR="../${NAME}"

# Create worktree and new branch
echo "➡️  Creating worktree and branch '$NAME' from commit $HASH (branches: $BRANCHES_CLEAN)..."
git worktree add -b "$NAME" "$DIR" "$HASH"

# Report result
if [ $? -eq 0 ]; then
  echo "✅ Worktree created at $DIR"
else
  echo "❌ Error creating worktree"
fi
