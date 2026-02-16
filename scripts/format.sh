#!/usr/bin/env bash
set -euo pipefail

if ! command -v clang-format >/dev/null 2>&1; then
  echo "clang-format not found"
  exit 1
fi

mapfile -t files < <(rg --files -g "*.{h,hpp,c,cc,cpp}")
if [[ ${#files[@]} -eq 0 ]]; then
  echo "no source files found"
  exit 0
fi

clang-format -i "${files[@]}"
echo "formatted ${#files[@]} files"
