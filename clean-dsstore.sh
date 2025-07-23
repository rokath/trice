#!/bin/bash

# Directory to scan (default: current directory)
TARGET_DIR="${1:-.}"

echo "🔍 Searching for .DS_Store files in: $TARGET_DIR"

# Find and delete all .DS_Store files
find "$TARGET_DIR" -name '.DS_Store' -type f -print -delete

echo "✅ Done! All .DS_Store files have been removed."
