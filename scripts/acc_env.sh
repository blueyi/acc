#!/usr/bin/env bash
# Add ACC build tools to PATH so you can run: acc, ac-opt, ac-translate, ac-runner
# Usage: source scripts/acc_env.sh   (from project root)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD="${PROJECT_ROOT}/build"

if [[ -d "$BUILD/bin" ]]; then
  export PATH="$BUILD/bin:$PATH"
elif [[ -d "$BUILD/tools" ]]; then
  for dir in "$BUILD/tools/acc-compiler" "$BUILD/tools/ac-opt" "$BUILD/tools/ac-translate" "$BUILD/tools/ac-runner"; do
    if [[ -d "$dir" ]]; then export PATH="$dir:$PATH"; fi
  done
else
  echo "ACC build not found. Run: ./scripts/build.sh" >&2
  return 1 2>/dev/null || exit 1
fi

echo "ACC tools added to PATH (acc, ac-opt, ac-translate, ac-runner). Use: acc-compiler or acc."
