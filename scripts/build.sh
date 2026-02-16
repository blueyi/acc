#!/bin/bash
# Build script for ACompiler

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building ACompiler...${NC}"

# Default values
BUILD_TYPE=${BUILD_TYPE:-Release}
BUILD_DIR=${BUILD_DIR:-build}
LLVM_DIR=${LLVM_DIR:-}
MLIR_DIR=${MLIR_DIR:-}
JOBS=${JOBS:-$(nproc)}

# Parse arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --debug)
      BUILD_TYPE=Debug
      shift
      ;;
    --llvm-dir)
      LLVM_DIR="$2"
      shift 2
      ;;
    --mlir-dir)
      MLIR_DIR="$2"
      shift 2
      ;;
    --clean)
      echo -e "${YELLOW}Cleaning build directory...${NC}"
      rm -rf "${BUILD_DIR}"
      shift
      ;;
    --help)
      echo "Usage: $0 [OPTIONS]"
      echo "Options:"
      echo "  --debug         Build in debug mode"
      echo "  --llvm-dir DIR  Path to LLVM installation"
      echo "  --mlir-dir DIR  Path to MLIR installation"
      echo "  --clean         Clean build directory before building"
      echo "  --help          Show this help message"
      exit 0
      ;;
    *)
      echo -e "${RED}Unknown option: $1${NC}"
      exit 1
      ;;
  esac
done

# Create build directory
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Configure CMake
echo -e "${GREEN}Configuring CMake...${NC}"
CMAKE_ARGS=(
  -G Ninja
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
)

if [ -n "${LLVM_DIR}" ]; then
  CMAKE_ARGS+=(-DLLVM_DIR="${LLVM_DIR}")
fi

if [ -n "${MLIR_DIR}" ]; then
  CMAKE_ARGS+=(-DMLIR_DIR="${MLIR_DIR}")
fi

cmake .. "${CMAKE_ARGS[@]}"

# Build
echo -e "${GREEN}Building (${JOBS} parallel jobs)...${NC}"
ninja -j "${JOBS}"

echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "Build artifacts are in: ${BUILD_DIR}"
