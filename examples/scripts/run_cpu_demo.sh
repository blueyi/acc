#!/usr/bin/env bash
set -euo pipefail

if [[ ! -x "./build/tools/acompiler-run" ]]; then
  echo "acompiler-run not found, please build first"
  exit 1
fi

./build/tools/acompiler-run
