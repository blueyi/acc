# ACC — AI Compiler Core

ACC (AI Compiler Core) 是一个基于 LLVM/MLIR 构建的 AI 编译器核心框架。项目支持从主流 AI 框架（PyTorch、Triton）到多硬件后端（CPU / GPU / NPU）的端到端编译流程，既作为学习编译器技术的实战平台，也作为可扩展的 AI Compiler Core 基础设施。

## 端到端编译流程

```
┌─────────────────────────────────────────────────────────┐
│                  AI Framework Layer                      │
│   PyTorch (nn.Module)    Triton (@jit)    ACC DSL (.ac) │
│         │                     │                │         │
│         ▼                     ▼                ▼         │
│   ┌───────────┐    ┌────────────────┐   ┌──────────┐   │
│   │ONNX/Torch │    │TritonImporter  │   │DSL       │   │
│   │Importer   │    │(TTIR → MLIR)   │   │Frontend  │   │
│   └─────┬─────┘    └───────┬────────┘   └────┬─────┘   │
├─────────┴──────────────────┴──────────────────┴─────────┤
│                  ACC MLIR Layer                          │
│   ACHigh Dialect  →  ACMid Dialect  →  Linalg/SCF       │
│   (Graph Opt)        (Tile/Loop)       (Bufferize)      │
├─────────────────────────────────────────────────────────┤
│                  LLVM Layer                              │
│   LLVM Dialect  →  LLVM IR  →  LLVM Optimization       │
├──────────┬──────────────────┬───────────────────────────┤
│   CPU    │   NVIDIA GPU     │   Ascend NPU              │
│  x86/ARM │   CUDA/PTX      │   Ascend IR               │
│  [实现中] │   [TODO]         │   [TODO]                  │
└──────────┴──────────────────┴───────────────────────────┘
```

## 核心特性

- **多框架对接**: PyTorch (ONNX / Torch-MLIR) + Triton DSL + 自定义 DSL
- **多层级 IR**: ACHigh (图层) → ACMid (Tile/Loop) → Linalg → LLVM
- **多后端支持**: CPU（实现中）+ CUDA（规划）+ Ascend NPU（规划）
- **20+ AI 算子**: Conv2D, MatMul, BatchMatMul, GELU, BatchNorm, Dense...
- **5+ 优化 Pass**: 算子融合, 常量折叠, 形状推导, 布局变换, 死代码消除
- **高性能 Runtime**: im2col+GEMM 卷积, Tiled MatMul, OpenMP 并行

## 快速开始

```bash
# 编译 LLVM/MLIR
./scripts/build_llvm.sh

# 编译 ACC
./scripts/build.sh

# 运行端到端验证（纯 Python，无需编译）
python3 examples/e2e/pytorch_triton_acc_e2e.py
python3 examples/e2e/triton_matmul_to_acc.py
```

## 工具链

```bash
acc-compiler model.onnx -o output     # 主编译器
ac-opt input.mlir --ac-op-fusion      # MLIR 优化工具
ac-translate --import-onnx model.onnx # IR 翻译工具
ac-runner input.mlir                  # JIT 执行工具
```

## 项目阶段

| 阶段 | 主题 | 周期 | 核心产出 |
|------|------|------|----------|
| **Phase 1** | MLIR 基础 + Dialect + Pass + 前端 | Week 1-4 | ACHigh 20+ 算子, 5 个 Pass, ONNX/DSL 前端 |
| **Phase 2** | Lowering + LLVM IR 优化 + SIMD | Week 5-8 | 完整降级链路, LLVM Pipeline, SIMD 映射 |
| **Phase 3** | CodeGen + Runtime + E2E | Week 9-12 | CPU 后端, im2col+GEMM, ResNet-18 推理 |
| **Phase 4** | 高级特性 + 多后端 | Week 13-14 | 量化/AutoTuning, CUDA/Ascend 框架 |
| **Phase 5** | Triton/PyTorch 对接 | Week 15-20 | TritonImporter, TorchImporter, E2E 验证 |
| **Phase 6** | 高阶特性 | Week 20+ | Auto-Scheduling, 动态形状, 混合精度 |

## 项目结构

| 目录 | 说明 |
|------|------|
| `include/ACC/Dialect/` | MLIR 方言: ACHigh (图层), ACMid (Tile/Loop), ACGPU (TODO) |
| `include/ACC/Frontend/` | 前端: Lexer, Parser, ONNXImporter, **TritonImporter**, **TorchImporter** |
| `include/ACC/Transforms/` | MLIR 优化 Pass |
| `include/ACC/Conversion/` | Lowering: ACHigh→ACMid→Linalg→LLVM |
| `include/ACC/IROptimizer/` | LLVM IR 自定义 Pass |
| `include/ACC/CodeGen/` | CPU 后端, JIT 引擎 |
| `lib/` | 所有模块的 C++ 实现 |
| `tools/` | 命令行工具 (acc-compiler, ac-opt, ac-translate, ac-runner) |
| `runtime/` | Runtime C API + CPU Kernel (im2col+GEMM) |
| `examples/e2e/` | **端到端验证**: Triton→ACC, PyTorch+Triton+ACC |
| `test/` | FileCheck 测试 + 性能基准 |
| `docs/` | [项目规划](docs/PROJECT_PLAN.md) · [目录结构](docs/DIRECTORY_STRUCTURE.md) |

## 端到端验证示例

### Triton MatMul → ACC → CPU

```python
# examples/e2e/triton_matmul_to_acc.py
# Triton matmul kernel → ACC MLIR → CPU tiled execution → NumPy 验证
python3 examples/e2e/triton_matmul_to_acc.py
# [PASS] MatMul(64x64 @ 64x64)
# [PASS] MatMul(128x128 @ 128x256)
# [PASS] MatMul(333x555 @ 555x444)
```

### PyTorch MLP → Triton Kernels → ACC → Inference

```python
# examples/e2e/pytorch_triton_acc_e2e.py
# PyTorch SimpleMLPModel → Triton fused_linear_relu → ACC compile → verify
python3 examples/e2e/pytorch_triton_acc_e2e.py
# [PASS] batch_size=  1, 8, 32, 64, 100
# Result: ALL TESTS PASSED
```

## 技术栈

- **LLVM 17+** / **MLIR** — 编译基础设施
- **OpenAI Triton** — 算子 DSL 前端
- **PyTorch** — AI 框架对接（ONNX / Torch-MLIR）
- **CMake + Ninja** — 构建系统
- **C++17** / **Python** — 主要开发语言

## License

This project is licensed under the Apache License 2.0.
