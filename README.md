# ACompiler

基于 LLVM/MLIR 的 AI 编译器项目，通过实例实践快速学习和掌握编译器知识。

## 项目简介

ACompiler 采用自上而下、由浅入深的学习路径：

- **MLIR 环节**：从 Dialect 定义到 Pass 优化，掌握多级 IR 与图优化
- **LLVM IR 环节**：Lowering、Pass 优化、指令映射与 Codegen

## 当前支持的后端

- **CPU**：x86_64 / AArch64（第一阶段目标）

## TODO：扩展后端（暂不实现）

- NVIDIA GPU - CUDA 支持
- 华为 Ascend NPU - Ascend IR 支持

详见 [TODO.md](TODO.md)。

## 文档

- [项目规划](docs/PROJECT_PLAN.md)
- [目录结构说明](docs/PROJECT_PLAN.md#六目录说明)

## 依赖

- LLVM (>= 16.0)
- MLIR（与 LLVM 同源）

## 构建

```bash
mkdir build && cd build
cmake -G Ninja ../ -DLLVM_DIR=<path_to_llvm> -DMLIR_DIR=<path_to_mlir>
ninja
```

## 许可证

待定
