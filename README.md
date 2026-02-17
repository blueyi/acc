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

# Optional: add tools to PATH so you can run acc, ac-opt, etc. from anywhere in this repo
source scripts/acc_env.sh

# Run end-to-end verification (pure Python, no build required)
python3 examples/e2e/pytorch_triton_acc_e2e.py
python3 examples/e2e/triton_matmul_to_acc.py
```

## Toolchain

Binaries are under `build/bin/`. Either add them to PATH or run by path:

```bash
# After: source scripts/acc_env.sh
acc model.onnx -o output
ac-opt input.mlir --ac-op-fusion
ac-translate --import-onnx model.onnx
ac-runner input.mlir
```

Without sourcing, run by path (from project root):

```bash
./build/bin/acc model.onnx -o output
./build/bin/ac-opt input.mlir --ac-op-fusion
./build/bin/ac-translate --import-onnx model.onnx
./build/bin/ac-runner input.mlir
```

## Testing

One-click run of the full test suite (builds if needed, then runs lit tests):

```bash
./scripts/run_tests.sh
```

Options:

- `./scripts/run_tests.sh --build` — build then run tests (default)
- `./scripts/run_tests.sh --no-build` — run tests only (requires prior build)

From the build directory:

```bash
cd build && ninja check-acc
```

**Note:** Tests use LLVM’s lit. If you see `llvm-lit: No such file or directory`, configure with a path to `llvm-lit` (e.g. from an LLVM build that includes MLIR):  
`cmake -DLLVM_EXTERNAL_LIT=/path/to/llvm-install/bin/llvm-lit ...` then rebuild. Alternatively, install lit and run: `pip install lit` then `./scripts/run_tests.sh --no-build` (if your environment provides the lit.llvm config).

### Test coverage

| Area | Location | Description |
|------|----------|-------------|
| **ACHigh dialect** | `test/Dialect/ACHigh/ops.mlir` | matmul, conv2d, relu, add |
| **ac-opt passes** | `test/Transforms/*.mlir` | ac-op-fusion, ac-constant-folding, ac-shape-inference, ac-layout-transform, ac-dead-code-elimination |
| **Conversion** | `test/Conversion/ACHighToACMid/basic.mlir` | ACHigh → ACMid lowering |
| **ac-translate** | `test/Tools/ac-translate-import-onnx.mlir` | `--import-onnx` option |
| **acc driver** | `test/Tools/acc-help.mlir` | acc binary runs and prints help |
| **ONNX add example** | `test/onnx-import-add/` | Sample `model.onnx` (add op) + expected MLIR |

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
