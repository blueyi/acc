#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
  echo "usage: $0 <path-to-llvm-project>"
  exit 1
fi

LLVM_SRC="$1"
BUILD_DIR="${2:-third_party/llvm-project/build}"
INSTALL_DIR="${3:-third_party/llvm-project/install}"

cmake -S "${LLVM_SRC}/llvm" -B "${BUILD_DIR}" -G Ninja \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}"

cmake --build "${BUILD_DIR}" -j
cmake --install "${BUILD_DIR}"
