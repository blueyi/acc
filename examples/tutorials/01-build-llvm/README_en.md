# Tutorial 1: Building the LLVM/MLIR Development Environment

## Goal

Learn how to build LLVM and MLIR from source and set up the ACC project development environment.

## Prerequisites

- CMake >= 3.20
- Ninja build tool
- C++17 compatible compiler (GCC >= 8 / Clang >= 10)
- At least 32GB RAM (required for building LLVM)
- ~50GB disk space

## Steps

### 1. Clone LLVM Source

```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout llvmorg-18.1.0  # Use a stable release
```

### 2. Build LLVM + MLIR

```bash
mkdir build && cd build
cmake -G Ninja ../llvm \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DCMAKE_INSTALL_PREFIX=$HOME/llvm-install \
  -DLLVM_INSTALL_UTILS=ON \
  -DMLIR_ENABLE_BINDINGS_PYTHON=OFF

ninja -j$(nproc)
ninja install
```

### 3. Build ACC

```bash
cd /path/to/ACC
mkdir build && cd build
cmake -G Ninja .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLLVM_DIR=$HOME/llvm-install/lib/cmake/llvm \
  -DMLIR_DIR=$HOME/llvm-install/lib/cmake/mlir

ninja -j$(nproc)
```

### 4. Verify

```bash
./bin/ac-opt --help
```

## Key Takeaways

1. How LLVM's modular build system works
2. The role of `LLVM_ENABLE_PROJECTS`
3. How out-of-tree projects link against LLVM/MLIR
4. The purpose of `LLVM_DIR` and `MLIR_DIR`
