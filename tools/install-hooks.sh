#!/usr/bin/env bash
# tools/install-hooks.sh
# Устанавливает pre-commit hook в .git/hooks/
set -e

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
HOOKS_DIR="$REPO_ROOT/.git/hooks"
SRC="$REPO_ROOT/tools/pre-commit.sh"
DEST="$HOOKS_DIR/pre-commit"

if [ ! -d "$HOOKS_DIR" ]; then
  echo "Error: .git/hooks not found. Выполните из корня git-репозитория."
  exit 1
fi

if [ ! -f "$SRC" ]; then
  echo "Error: $SRC not found."
  exit 1
fi

cp
"$SRC" "$DEST"
chmod +x "$DEST"
echo "Installed pre-commit hook to $DEST"
echo "Note: to skip hook use 'git commit --no-verify'"
