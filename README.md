# ACompiler

基于 LLVM/MLIR 的 AI 编译器实践项目（ACompiler）。项目目标是通过“端到端可运行样例 + 分阶段实现 Dialect/Pass/Lowering/CodeGen”的方式，快速学习并掌握编译器核心知识与工程能力。

## 当前状态

- 本仓库提供 **工程骨架 + 规划文档**。
- 第一阶段目标：**仅支持 CPU 后端**。
- GPU（CUDA）与 Ascend NPU 后端：已在目录与 TODO 中预留，**暂不实现**。

## 文档

- 项目规划：`docs/ACompiler_Plan.md`

## 构建（需要本机已安装 LLVM/MLIR）

本项目采用“standalone”方式查找 LLVM/MLIR（通过 `LLVM_DIR` / `MLIR_DIR` 指向对应的 CMake 配置目录）。

示例：

```bash
cmake -S . -B build \
  -GNinja \
  -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm \
  -DMLIR_DIR=/path/to/llvm/lib/cmake/mlir
cmake --build build
```

## 工具

- `acompiler-opt`：类似 `mlir-opt`，用于跑 pass 与调试 IR（后续会逐步扩展自定义 Dialect/Pass）。

