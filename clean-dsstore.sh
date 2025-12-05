#!/bin/bash

# ================================================
# clean-dsstore-advanced.sh
# Deletes .DS_Store files recursively
#
# 📌 Usage:
#   ./clean-dsstore-advanced.sh [TARGET_DIR] [--dry-run]
#
# ✅ Examples:
#   ./clean-dsstore-advanced.sh                  # cleans current directory
#   ./clean-dsstore-advanced.sh ~/Documents      # cleans specific directory
#   ./clean-dsstore-advanced.sh --dry-run        # shows what would be deleted
#
# ⚠️ Notes:
# - Excludes system-protected macOS folders
# - Quits and restarts Finder only if it was running
# ================================================

# === Terminal colors ===
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
NC='\033[0m' # No color

# === Default settings ===
TARGET_DIR="."
DRY_RUN=false

# === Parse arguments ===
for arg in "$@"; do
	case "$arg" in
	--dry-run) DRY_RUN=true ;;
	/*) TARGET_DIR="$arg" ;;
	esac
done

COUNT=0
OS_TYPE="$(uname)"

if [[ $OS_TYPE == "Darwin" ]]; then
	# === Check if Finder is running ===
	FINDER_WAS_RUNNING=false
	if pgrep -xq "Finder"; then
		FINDER_WAS_RUNNING=true
		echo -e "${YELLOW}🛑 Quitting Finder to prevent interference...${NC}"
		osascript -e 'tell application "Finder" to quit'
	fi
fi

echo -e "${BLUE}🔍 Scanning for .DS_Store files...${NC}"

# === Main find/delete loop ===
find "$TARGET_DIR" \
	-path "*/.Spotlight-V100" -prune -o \
	-path "*/.DocumentRevisions-V100" -prune -o \
	-path "*/.TemporaryItems" -prune -o \
	-name '.DS_Store' -type f -print | while read -r file; do
	echo -e "${YELLOW}🗑️  Found: $file${NC}"
	if [ "$DRY_RUN" = false ]; then
		rm -f "$file" && ((COUNT++)) &&
			echo -e "${GREEN}✔️  Deleted${NC}"
	else
		((COUNT++))
	fi
done

# === Summary ===
echo -e "${BLUE}✅ Done. Processed $COUNT file(s).${NC}"

if [[ $OS_TYPE == "Darwin" ]]; then
	# === Restart Finder if it was previously running ===
	if [ "$FINDER_WAS_RUNNING" = true ]; then
		echo -e "${YELLOW}🔄 Restarting Finder...${NC}"
		open -a Finder
	else
		echo -e "${BLUE}📎 Finder was not running before – not restarted.${NC}"
	fi
fi
