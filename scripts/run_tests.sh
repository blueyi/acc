#!/bin/bash
# ==============================================================================
# run_tests.sh - Run ACC tests
# ==============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"

echo "=== Running ACC Tests ==="

if [ ! -d "$BUILD_DIR" ]; then
  echo "Error: Build directory not found at $BUILD_DIR"
  echo "Please build ACC first: ./scripts/build.sh"
  exit 1
fi

cd "$BUILD_DIR"

# Run lit tests
echo "Running FileCheck/lit tests..."
ninja check-acc

# Run unit tests (if built)
if [ -d "$BUILD_DIR/unittests" ]; then
  echo "Running unit tests..."
  ctest --test-dir "$BUILD_DIR" --output-on-failure
fi

echo ""
echo "=== All tests passed ==="
