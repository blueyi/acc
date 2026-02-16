#!/bin/bash
# ==============================================================================
# build_llvm.sh - Build LLVM/MLIR from source for ACompiler development
# ==============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Configuration
LLVM_VERSION="${LLVM_VERSION:-18.1.0}"
LLVM_SRC_DIR="${LLVM_SRC_DIR:-$PROJECT_ROOT/third_party/llvm-project}"
LLVM_BUILD_DIR="${LLVM_BUILD_DIR:-$PROJECT_ROOT/third_party/llvm-project/build}"
LLVM_INSTALL_DIR="${LLVM_INSTALL_DIR:-$HOME/llvm-install}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
NUM_JOBS="${NUM_JOBS:-$(nproc)}"

echo "=== ACompiler: Building LLVM/MLIR ==="
echo "LLVM Version:  $LLVM_VERSION"
echo "Source Dir:    $LLVM_SRC_DIR"
echo "Build Dir:     $LLVM_BUILD_DIR"
echo "Install Dir:   $LLVM_INSTALL_DIR"
echo "Build Type:    $BUILD_TYPE"
echo "Parallel Jobs: $NUM_JOBS"
echo "======================================"

# Clone LLVM if not exists
if [ ! -d "$LLVM_SRC_DIR" ]; then
  echo "Cloning LLVM source..."
  git clone --depth 1 --branch "llvmorg-$LLVM_VERSION" \
    https://github.com/llvm/llvm-project.git "$LLVM_SRC_DIR"
fi

# Configure
mkdir -p "$LLVM_BUILD_DIR"
cd "$LLVM_BUILD_DIR"

cmake -G Ninja "$LLVM_SRC_DIR/llvm" \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  -DCMAKE_INSTALL_PREFIX="$LLVM_INSTALL_DIR" \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_INSTALL_UTILS=ON \
  -DMLIR_ENABLE_BINDINGS_PYTHON=OFF \
  -DLLVM_ENABLE_ZLIB=OFF \
  -DLLVM_ENABLE_ZSTD=OFF

# Build
echo "Building LLVM/MLIR (this may take a while)..."
ninja -j"$NUM_JOBS"

# Install
echo "Installing LLVM/MLIR..."
ninja install

echo ""
echo "=== LLVM/MLIR build complete ==="
echo "Installed to: $LLVM_INSTALL_DIR"
echo ""
echo "To build ACompiler, run:"
echo "  cd $PROJECT_ROOT && ./scripts/build.sh"
