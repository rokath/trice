#!/bin/bash
# Usage: ./AddWorktreeFromGitLogLine.sh <commit-hash> <YYYY-MM-DD> <HH:MM>
# Example: ./my.sh cb9b06026 2025-08-18 12:21

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

# Format date and time: YYYY-MM-DD → MM-DD, HH:MM → HH-MM
DATE_PART=$(echo "$DATE" | cut -d'-' -f2-3)
TIME_PART=$(echo "$TIME" | tr ':' '-')

# Construct branch and directory name
NAME="trice_${DATE_PART}-${TIME_PART}_${HASH}"
DIR="../${NAME}"

# Create worktree and branch
echo "➡️  Creating worktree and branch '$NAME' from commit $HASH..."
git worktree add -b "$NAME" "$DIR" "$HASH"

# Report result
if [ $? -eq 0 ]; then
  echo "✅ Worktree created at $DIR"
else
  echo "❌ Error creating worktree"
fi
