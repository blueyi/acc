#!/bin/bash
# ==============================================================================
# build.sh - Build ACC
# ==============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Configuration
BUILD_DIR="${BUILD_DIR:-$PROJECT_ROOT/build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
LLVM_INSTALL_DIR="${LLVM_INSTALL_DIR:-$HOME/llvm-install}"
if command -v nproc >/dev/null 2>&1; then
  NUM_JOBS="${NUM_JOBS:-$(nproc)}"
else
  NUM_JOBS=$(sysctl -n hw.ncpu)
fi

echo "=== ACC Build ==="
echo "Project Root:  $PROJECT_ROOT"
echo "Build Dir:     $BUILD_DIR"
echo "Build Type:    $BUILD_TYPE"
echo "LLVM Dir:      $LLVM_INSTALL_DIR"
echo "Parallel Jobs: $NUM_JOBS"
echo "========================"

# Check LLVM installation; build if missing
if [ ! -d "$LLVM_INSTALL_DIR/lib/cmake/llvm" ]; then
  echo "LLVM not found at $LLVM_INSTALL_DIR"
  echo "Building LLVM/MLIR via build_llvm.sh..."
  "$SCRIPT_DIR/build_llvm.sh"
  echo "LLVM build finished. Continuing ACC build..."
fi

# Configure
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -G Ninja "$PROJECT_ROOT" \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  -DLLVM_DIR="$LLVM_INSTALL_DIR/lib/cmake/llvm" \
  -DMLIR_DIR="$LLVM_INSTALL_DIR/lib/cmake/mlir" \
  -DAC_ENABLE_TESTS=ON

# Build
echo "Building ACC..."
ninja -j"$NUM_JOBS"

echo ""
echo "=== ACC build complete ==="
echo "Binaries are in: $BUILD_DIR/bin/"
echo ""
echo "Available tools:"
echo "  $BUILD_DIR/bin/acc      - Main compiler driver"
echo "  $BUILD_DIR/bin/ac-opt         - MLIR optimization tool"
echo "  $BUILD_DIR/bin/ac-translate   - IR translation tool"
echo "  $BUILD_DIR/bin/ac-runner      - JIT execution tool"
