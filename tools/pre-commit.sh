#!/usr/bin/env bash
# tools/pre-commit.sh
# pre-commit hook: форматирование и минимальный clang-tidy. Ставится в .git/hooks/pre-commit
set -euo pipefail

# Stash unstaged changes
git update-index -q --refresh

STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|c|hpp|h)$' || true)
if [ -z "$STAGED_FILES" ]; then
  exit 0
fi

# format staged files
echo "Formatting staged files..."
for file in $STAGED_FILES; do
  clang-format -i "$file"
  git add "$file"
done

# run clang-tidy on staged files (best-effort, do not block commit on failures)
echo "Running clang-tidy (non-blocking)..."
CLANG_TIDY_CHECKS="-checks=modernize-*,readability-*,bugprone-*,performance-*,-clang-analyzer-*"
for file in $STAGED_FILES; do
  clang-tidy "$file" --quiet --checks="${CLANG_TIDY_CHECKS}" -- -std=c++20 -Iinc || true
done

exit 0


#
##!/usr/bin/env bash
## tools/pre-commit.sh
## Проверяет staged C/C++ файлы: запускает clang-format и, опционально, clang-tidy (медленно).
#set -e
#
#REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
#BUILD_DIR="$REPO_ROOT/build"
#
## Список расширений
#EXT_REGEX='\.(c|cc|cpp|cxx|h|hpp|hh)$'
#
## Получаем staged файлы по пути в рабочем каталоге
#STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E "$EXT_REGEX" || true)
#[ -z "$STAGED_FILES" ] && exit 0
#
#CLANG_FORMAT_BIN="${CLANG_FORMAT_BIN:-clang-format}"
#CLANG_TIDY_BIN="${CLANG_TIDY_BIN:-clang-tidy}"
#
## Форматирование и повторный add
#for f in $STAGED_FILES; do
#  if [ -f "$f" ]; then
#    echo "Formatting $f"
#    "$CLANG_FORMAT_BIN" -i "$f"
#    git add "$f"
#  fi
#done
#
## Быстрая проверка clang-tidy только для изменённых файлов (не обязательна)
#if command -v "$CLANG_TIDY_BIN" >/dev/null 2>&1; then
#  echo "Running clang-tidy on staged files (this can be slow)"
#  # Требуется compile_commands.json в build/
#  if [ -f "$BUILD_DIR/compile_commands.json" ]; then
#    TIDY_FAIL=0
#    for f in $STAGED_FILES; do
#      if [ -f "$f" ]; then
#        echo "clang-tidy $f"
#        "$CLANG_TIDY_BIN" "$f" -- -p="$BUILD_DIR" || TIDY_FAIL=1
#      fi
#    done
#    if [ "$TIDY_FAIL" -ne 0 ]; then
#      echo "clang-tidy reported issues. Fix or run 'git commit --no-verify' to bypass."
#      exit 1
#    fi
#  else
#    echo "Warning: $BUILD_DIR/compile_commands.json not found. Skipping clang-tidy."
#  fi
#fi
#
#exit 0
