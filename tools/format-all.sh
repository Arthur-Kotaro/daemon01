#!/usr/bin/env bash
set -euo pipefail
FILES=$(git ls-files '*.cpp' '*.hpp' '*.h' '*.c' | tr '\n' ' ')
if [ -z "$FILES" ]; then
    echo "No files to format"
    exit 0
fi
clang-format -i "$FILES"