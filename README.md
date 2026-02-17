# ACC — AI Compiler Core

[中文版](README_zh.md)

ACC (AI Compiler Core) is an AI compiler core framework built on LLVM/MLIR. It supports end-to-end compilation from mainstream AI frameworks (PyTorch, Triton) to multiple hardware backends (CPU / GPU / NPU), serving both as a hands-on platform for learning compiler technologies and as an extensible AI Compiler Core infrastructure.

## End-to-End Compilation Flow

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
│  [WIP]   │   [TODO]         │   [TODO]                  │
└──────────┴──────────────────┴───────────────────────────┘
```

## Key Features

- **Multi-Framework Support**: PyTorch (ONNX / Torch-MLIR) + Triton DSL + Custom DSL
- **Multi-Level IR**: ACHigh (Graph) → ACMid (Tile/Loop) → Linalg → LLVM
- **Multi-Backend**: CPU (WIP) + CUDA (Planned) + Ascend NPU (Planned)
- **20+ AI Operators**: Conv2D, MatMul, BatchMatMul, GELU, BatchNorm, Dense...
- **5+ Optimization Passes**: Op Fusion, Constant Folding, Shape Inference, Layout Transform, DCE
- **High-Performance Runtime**: im2col+GEMM Conv, Tiled MatMul, OpenMP Parallelism

## Quick Start

```bash
# Build LLVM/MLIR
./scripts/build_llvm.sh

# Build ACC
./scripts/build.sh

# Run end-to-end verification (pure Python, no build required)
python3 examples/e2e/pytorch_triton_acc_e2e.py
python3 examples/e2e/triton_matmul_to_acc.py
```

## Toolchain

```bash
acc-compiler model.onnx -o output     # Main compiler driver
ac-opt input.mlir --ac-op-fusion      # MLIR optimization tool
ac-translate --import-onnx model.onnx # IR translation tool
ac-runner input.mlir                  # JIT execution tool
```

## Project Phases

| Phase | Topic | Timeline | Key Deliverables |
|-------|-------|----------|-----------------|
| **Phase 1** | MLIR Basics + Dialect + Pass + Frontend | Week 1-4 | ACHigh 20+ ops, 5 Passes, ONNX/DSL frontend |
| **Phase 2** | Lowering + LLVM IR Optimization + SIMD | Week 5-8 | Full lowering pipeline, LLVM passes, SIMD mapping |
| **Phase 3** | CodeGen + Runtime + E2E | Week 9-12 | CPU backend, im2col+GEMM, ResNet-18 inference |
| **Phase 4** | Advanced Features + Multi-Backend | Week 13-14 | Quantization/AutoTuning, CUDA/Ascend framework |
| **Phase 5** | Triton/PyTorch Integration | Week 15-20 | TritonImporter, TorchImporter, E2E verification |
| **Phase 6** | Advanced Features | Week 20+ | Auto-Scheduling, Dynamic Shapes, Mixed Precision |

## Project Structure

| Directory | Description |
|-----------|-------------|
| `include/ACC/Dialect/` | MLIR Dialects: ACHigh (Graph), ACMid (Tile/Loop), ACGPU (TODO) |
| `include/ACC/Frontend/` | Frontends: Lexer, Parser, ONNXImporter, **TritonImporter**, **TorchImporter** |
| `include/ACC/Transforms/` | MLIR Optimization Passes |
| `include/ACC/Conversion/` | Lowering: ACHigh→ACMid→Linalg→LLVM |
| `include/ACC/IROptimizer/` | Custom LLVM IR Passes |
| `include/ACC/CodeGen/` | CPU Backend, JIT Engine |
| `lib/` | C++ implementations for all modules |
| `tools/` | CLI tools (acc-compiler, ac-opt, ac-translate, ac-runner) |
| `runtime/` | Runtime C API + CPU Kernels (im2col+GEMM) |
| `examples/e2e/` | **E2E Verification**: Triton→ACC, PyTorch+Triton+ACC |
| `test/` | FileCheck tests + Performance benchmarks |
| `docs/` | [Project Plan](docs/PROJECT_PLAN.md) · [Directory Structure](docs/DIRECTORY_STRUCTURE.md) · [TODO Tracker](docs/TODO.md) |

## End-to-End Verification Examples

### Triton MatMul → ACC → CPU

```python
# examples/e2e/triton_matmul_to_acc.py
python3 examples/e2e/triton_matmul_to_acc.py
# [PASS] MatMul(64x64 @ 64x64)
# [PASS] MatMul(128x128 @ 128x256)
# [PASS] MatMul(333x555 @ 555x444)
```

### PyTorch MLP → Triton Kernels → ACC → Inference

```python
# examples/e2e/pytorch_triton_acc_e2e.py
python3 examples/e2e/pytorch_triton_acc_e2e.py
# [PASS] batch_size=  1, 8, 32, 64, 100
# Result: ALL TESTS PASSED
```

## Tech Stack

- **LLVM 17+** / **MLIR** — Compiler infrastructure
- **OpenAI Triton** — Operator DSL frontend
- **PyTorch** — AI framework integration (ONNX / Torch-MLIR)
- **CMake + Ninja** — Build system
- **C++17** / **Python** — Primary languages

## Documentation

| Document | Language |
|----------|----------|
| [Project Plan (CN)](docs/PROJECT_PLAN.md) | Chinese |
| [Project Plan (EN)](docs/PROJECT_PLAN_en.md) | English |
| [TODO Tracker (CN)](docs/TODO.md) | Chinese |
| [TODO Tracker (EN)](docs/TODO_en.md) | English |
| [Directory Structure (CN)](docs/DIRECTORY_STRUCTURE.md) | Chinese |
| [Directory Structure (EN)](docs/DIRECTORY_STRUCTURE_en.md) | English |

## License

This project is licensed under the Apache License 2.0.
