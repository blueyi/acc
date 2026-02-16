# 构建与开发环境

## 依赖

- LLVM + MLIR（建议与本项目兼容的同一套安装/构建产物）
- CMake（>= 3.20）
- Ninja（推荐）

## 构建方式（standalone）

通过 `LLVM_DIR` / `MLIR_DIR` 指向 LLVM/MLIR 的 CMake 配置目录：

```bash
cmake -S . -B build -GNinja \
  -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm \
  -DMLIR_DIR=/path/to/llvm/lib/cmake/mlir
cmake --build build
```

## 运行 lit 测试（可选）

如果你的 LLVM 构建产物提供 `llvm-lit`，可在配置时确保 `LLVM_EXTERNAL_LIT` 被正确设置（或在 CMake 中手动指定）。

```bash
cmake -S . -B build -GNinja \
  -DLLVM_DIR=... -DMLIR_DIR=... \
  -DLLVM_EXTERNAL_LIT=/path/to/llvm-lit
cmake --build build --target check-acompiler
```

