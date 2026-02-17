#!/usr/bin/env bash
# ==============================================================================
# run_tests.sh - One-click run of ACC test suite
# ==============================================================================
# Usage:
#   ./scripts/run_tests.sh           # run tests (build if needed)
#   ./scripts/run_tests.sh --build   # build then run tests
#   ./scripts/run_tests.sh --no-build # run tests only (fail if not built)
#
# Requires: lit (pip install lit) or LLVM with llvm-lit in PATH for ninja check-acc.
# ==============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
DO_BUILD=1

for arg in "$@"; do
  case "$arg" in
    --build)    DO_BUILD=1 ;;
    --no-build) DO_BUILD=0 ;;
    --help|-h)
      echo "Usage: $0 [--build|--no-build]"
      echo "  --build     Build then run tests (default)."
      echo "  --no-build  Run tests only; fail if build is missing."
      echo "Requires: pip install lit  (or use LLVM with llvm-lit for ninja check-acc)."
      exit 0
      ;;
  esac
done

if [ "$DO_BUILD" -eq 1 ]; then
  echo "=== Building ACC ==="
  if [ -f "$PROJECT_ROOT/scripts/build.sh" ]; then
    (cd "$PROJECT_ROOT" && ./scripts/build.sh)
  else
    (cd "$BUILD_DIR" && ninja -j"${NUM_JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu)}")
  fi
  echo ""
fi

if [ ! -d "$BUILD_DIR" ]; then
  echo "Error: Build directory not found: $BUILD_DIR"
  echo "Run with --build or run ./scripts/build.sh first."
  exit 1
fi

echo "=== Running ACC tests (lit) ==="
export PATH="$BUILD_DIR/bin:$PATH"
# FileCheck is required; use LLVM install bin if not set (match scripts/build.sh default)
if [ -z "${LLVM_TOOLS_DIR}" ] && [ -d "${LLVM_INSTALL_DIR:-$HOME/llvm-install}/bin" ]; then
  export LLVM_TOOLS_DIR="${LLVM_INSTALL_DIR:-$HOME/llvm-install}/bin"
fi

# Prefer lit / llvm-lit executable (pip install lit adds 'lit', LLVM install may add 'llvm-lit')
if command -v lit >/dev/null 2>&1; then
  (cd "$PROJECT_ROOT" && lit -sv "$BUILD_DIR/test")
elif command -v llvm-lit >/dev/null 2>&1; then
  (cd "$PROJECT_ROOT" && llvm-lit -sv "$BUILD_DIR/test")
else
  (cd "$BUILD_DIR" && ninja check-acc) || {
    echo ""
    echo "Tests need lit. Install with: pip install lit  (or use LLVM with llvm-lit in PATH)"
    echo "Then run: $0 --no-build"
    exit 1
  }
fi
echo ""
echo "=== All tests finished ==="
