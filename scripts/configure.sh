#!/usr/bin/env bash
set -euo pipefail

GENERATOR="Unix Makefiles"
if command -v ninja >/dev/null 2>&1; then
  GENERATOR="Ninja"
fi

cmake -S . -B build -G "${GENERATOR}" "$@"
