# ACompiler — AI Compiler based on LLVM/MLIR

ACompiler 是一个基于 LLVM/MLIR 基础设施实现的 AI 编译器教育项目，通过实例驱动的方式系统学习现代编译器技术。项目采用**自上而下、由浅入深**的渐进式学习路线，从 MLIR 高层抽象开始，逐步深入到 LLVM IR 优化和机器码生成。

## 项目特性

- **渐进式学习**: 从简单到复杂，逐步掌握编译技术
- **实例驱动**: 每个概念都有完整的代码示例和教程
- **多后端架构**: CPU (实现中) + CUDA (规划) + Ascend (规划)
- **完整文档**: 详细的教程、API 文档和实践指南
- **测试完备**: FileCheck 集成测试 + GoogleTest 单元测试 + 性能基准测试

## 编译流水线

```
AI Model (DSL / ONNX)
    │
    ▼
Frontend ──→ MLIR (ACHigh Dialect)
    │         20+ AI 算子: Conv2D, MatMul, BatchMatMul, ReLU,
    │         GELU, Sigmoid, BatchNorm, Softmax, Dense, ...
    ▼
Graph Optimization: 算子融合, 常量折叠, 形状推导, 布局变换
    │
    ▼
MLIR (ACMid Dialect) ──→ Linalg ──→ Affine/SCF ──→ LLVM Dialect
    │
    ▼
LLVM IR ──→ LLVM Optimization ──→ CodeGen (x86-64 / AArch64)
    │
    ▼
Executable + Runtime (im2col+GEMM, SIMD, 多线程)
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

# 导入 ONNX 模型
./build/bin/ac-translate --import-onnx resnet18.onnx -o resnet18.mlir

# 主编译器
./build/bin/acompiler model.onnx -o output

# JIT 执行
./build/bin/ac-runner input.mlir --entry-point=main
```

## 项目结构

| 目录 | 说明 |
|------|------|
| `include/ACompiler/Dialect/` | MLIR 方言定义: ACHigh (图层, 20+ 算子), ACMid (Tile/Loop 层) |
| `include/ACompiler/Transforms/` | MLIR 优化 Pass 声明 (融合, 折叠, 推导, 布局, DCE) |
| `include/ACompiler/Conversion/` | Lowering Pass: ACHigh→ACMid→Linalg→LLVM |
| `include/ACompiler/Frontend/` | 前端: Lexer, Parser, AST, Sema, MLIRGen, ONNX 导入 |
| `include/ACompiler/IROptimizer/` | LLVM IR 自定义优化 Pass |
| `include/ACompiler/CodeGen/` | 代码生成: CPU 后端, JIT 引擎 |
| `lib/` | 所有模块的 C++ 实现 |
| `tools/` | 命令行工具 (acompiler, ac-opt, ac-translate, ac-runner) |
| `runtime/` | Runtime API (句柄式 C API) + CPU Kernel (im2col+GEMM) |
| `test/` | FileCheck 测试 + 性能基准 |
| `unittests/` | GoogleTest 单元测试 |
| `examples/` | DSL 示例 + 8 个教程 |
| `docs/` | 项目规划, 目录结构说明 |

详细目录结构请参阅 [docs/DIRECTORY_STRUCTURE.md](docs/DIRECTORY_STRUCTURE.md)。

## 学习路线（4 阶段）

| 阶段 | 主题 | 周期 | 教程 |
|------|------|------|------|
| **Phase 1** | MLIR 基础 + Dialect + Pass + 前端 | Week 1-4 | [编译LLVM](examples/tutorials/01-build-llvm/) / [方言](examples/tutorials/02-first-dialect/) / [算子](examples/tutorials/03-define-ops/) / [Pass](examples/tutorials/04-write-pass/) |
| **Phase 2** | Lowering + LLVM IR 优化 + SIMD | Week 5-7 | [Lowering](examples/tutorials/05-lowering/) / [LLVM IR](examples/tutorials/06-llvm-ir-opt/) |
| **Phase 3** | CodeGen + Runtime + 性能优化 | Week 8-11 | [CodeGen](examples/tutorials/07-codegen/) / [E2E](examples/tutorials/08-e2e-pipeline/) |
| **Phase 4** | 多后端: CUDA + Ascend (TODO) | Week 12+ | — |

## 多后端支持

| 后端 | 状态 | 说明 |
|------|------|------|
| CPU (x86-64 / AArch64) | Phase 1 (进行中) | SSE/AVX/NEON 向量化, im2col+GEMM |
| NVIDIA GPU (CUDA) | TODO (Phase 4) | NVPTX + Tensor Core |
| Huawei Ascend NPU | TODO (Phase 4) | Ascend IR + CANN |

## 性能目标

| 模型 | 输入规模 | ACompiler 目标 | vs ONNX Runtime |
|------|---------|---------------|-----------------|
| ResNet-18 | 1x3x224x224 | 30ms | 65%+ |
| MobileNetV2 | 1x3x224x224 | 20ms | 75%+ |
| BERT-Base | seq=128 | 150ms | 65%+ |

## 技术栈

- **LLVM 17+** / **MLIR** — 编译基础设施
- **CMake + Ninja** — 构建系统
- **TableGen** — 方言/算子/Pass 定义
- **lit + FileCheck** — MLIR 测试, **GoogleTest** — 单元测试
- **C++17** — 主要开发语言

## 文档

- [项目详细规划](docs/PROJECT_PLAN.md)（4 阶段, 含代码示例和验收标准）
- [目录结构说明](docs/DIRECTORY_STRUCTURE.md)

## License

This project is licensed under the Apache License 2.0.
