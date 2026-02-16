# ACompiler — AI Compiler based on LLVM/MLIR

ACompiler 是一个基于 LLVM/MLIR 基础设施实现的 AI 编译器学习与实践项目。通过逐步构建一个完整的 AI 编译器，帮助开发者快速掌握现代编译器技术栈。

## 项目特点

- **由浅入深**: 从 MLIR 方言定义开始，逐步深入到 LLVM IR 优化和代码生成
- **自上而下**: 先从高层图优化入手，再到底层指令映射
- **实践驱动**: 每个阶段都有可运行的代码和测试用例
- **多后端架构**: 支持 CPU，预留 GPU (CUDA) 和 NPU (Ascend) 扩展接口

## 编译流水线

```
AI Model (DSL/ONNX)
    │
    ▼
Frontend ──→ MLIR (ACHigh Dialect) ──→ Graph Optimization
    │
    ▼
MLIR (ACMid Dialect) ──→ Linalg ──→ Affine/SCF ──→ LLVM Dialect
    │
    ▼
LLVM IR ──→ LLVM Optimization ──→ CodeGen ──→ Executable
```

## 快速开始

### 1. 编译 LLVM/MLIR

```bash
./scripts/build_llvm.sh
```

### 2. 编译 ACompiler

```bash
./scripts/build.sh
```

### 3. 使用工具

```bash
# MLIR 优化工具
./build/bin/ac-opt input.mlir --ac-op-fusion --ac-constant-folding

# 主编译器
./build/bin/acompiler model.onnx -o output

# JIT 执行
./build/bin/ac-runner input.mlir --entry-point=main
```

## 项目结构

| 目录 | 说明 |
|------|------|
| `include/ACompiler/` | 公开头文件（方言定义、Pass声明、API） |
| `lib/` | 核心库实现（方言、Lowering、优化、前端、代码生成） |
| `tools/` | 命令行工具（acompiler, ac-opt, ac-translate, ac-runner） |
| `runtime/` | 运行时支持库（CPU / [TODO] CUDA / [TODO] Ascend） |
| `test/` | FileCheck 回归测试 |
| `examples/` | 示例代码和教程 |
| `docs/` | 项目文档（规划、目录结构说明） |

详细目录结构请参阅 [docs/DIRECTORY_STRUCTURE.md](docs/DIRECTORY_STRUCTURE.md)。

## 学习路线

| 阶段 | 主题 | 教程 |
|------|------|------|
| 1 | 环境搭建 | [编译LLVM/MLIR](examples/tutorials/01-build-llvm/) |
| 2 | MLIR 基础 | [创建方言](examples/tutorials/02-first-dialect/) / [定义算子](examples/tutorials/03-define-ops/) |
| 3 | MLIR 优化 | [编写Pass](examples/tutorials/04-write-pass/) |
| 4 | Lowering | [实现Lowering](examples/tutorials/05-lowering/) |
| 5 | LLVM IR | [LLVM IR优化](examples/tutorials/06-llvm-ir-opt/) |
| 6 | CodeGen | [代码生成](examples/tutorials/07-codegen/) |
| 7 | 端到端 | [完整流水线](examples/tutorials/08-e2e-pipeline/) |

## 多后端支持

| 后端 | 状态 | 说明 |
|------|------|------|
| CPU (x86-64/AArch64) | Phase 1 (进行中) | SSE/AVX/NEON 向量化 |
| NVIDIA GPU (CUDA) | TODO (Phase 2) | NVPTX + Tensor Core |
| Huawei Ascend NPU | TODO (Phase 3) | Ascend IR + CANN |

## 技术栈

- **LLVM 18+** — IR 优化和代码生成
- **MLIR** — 多层级中间表示和方言
- **CMake + Ninja** — 构建系统
- **TableGen** — 方言/操作/Pass 定义
- **lit + FileCheck** — 测试框架
- **C++17** — 主要开发语言

## 文档

- [项目详细规划](docs/PROJECT_PLAN.md)
- [目录结构说明](docs/DIRECTORY_STRUCTURE.md)

## License

This project is licensed under the Apache License 2.0.
