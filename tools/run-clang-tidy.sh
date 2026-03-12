#!/usr/bin/env bash
set -euo pipefail
# простой вызов clang-tidy для изменённых файлов or all
FILES=$(git ls-files '*.cpp' '*.hpp' '*.c' | tr '\n' ' ')
if [ -z "$FILES" ]; then
  echo "No files for clang-tidy"
  exit 0
fi
# минимальный набор проверок
CLANG_TIDY_CHECKS="-checks=modernize-*,readability-*,bugprone-*,performance-*,-clang-analyzer-*"
for f in $FILES; do
  echo "Running clang-tidy on $f"
  clang-tidy "$f" --quiet --checks="${CLANG_TIDY_CHECKS}" -- -std=c++20 -Iinc || true
done
