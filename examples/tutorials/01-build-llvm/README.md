# 教程 1: 编译 LLVM/MLIR 开发环境

## 目标

学习如何从源码编译 LLVM 和 MLIR，搭建 ACC 项目的开发环境。

## 前置条件

- CMake >= 3.20
- Ninja 构建工具
- C++17 兼容的编译器 (GCC >= 8 / Clang >= 10)
- 至少 32GB 内存（编译 LLVM 需要）
- 约 50GB 磁盘空间

## 步骤

### 1. 克隆 LLVM 源码

```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout llvmorg-18.1.0  # 使用稳定版本
```

### 2. 编译 LLVM + MLIR

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

### 3. 编译 ACC

```bash
cd /path/to/ACC
mkdir build && cd build
cmake -G Ninja .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLLVM_DIR=$HOME/llvm-install/lib/cmake/llvm \
  -DMLIR_DIR=$HOME/llvm-install/lib/cmake/mlir

ninja -j$(nproc)
```

### 4. 验证

```bash
./bin/ac-opt --help
```

## 学习要点

1. LLVM 的模块化构建系统如何工作
2. `LLVM_ENABLE_PROJECTS` 的作用
3. Out-of-tree 项目如何链接 LLVM/MLIR
4. `LLVM_DIR` 和 `MLIR_DIR` 的作用
