# ACC Getting Started Guide

> Step-by-step guide to start the ACC project and understand its structure. Follow the "Progress & TODO" section below.

---

## 1. Current progress and where to start

From the **Progress & TODO** (Section 8):

- **Phase 1** in progress (~0% bar)
- **1.1 Project setup**: 1.1.1–1.1.3, 1.1.5–1.1.6 done; **TODO**: 1.1.4 unit test framework (GoogleTest), acceptance (clean build, `ac-opt --version`, tests and CI)
- **1.2 MLIR basics**: dialects and most ops defined; **TODO**: 1.2.7 op verify(), 1.2.8 shape inference interface, 1.2.10 dialect tests
- **1.3 Pass development**, **1.4 Frontend** not started

**Suggested start**: Get the environment and tests running, then proceed in order 1.1.4 → 1.2.7/1.2.8 → 1.3.

---

## 2. Environment and build (TODO 1.1)

### 2.1 Dependencies

- **LLVM 17+** and **MLIR** (the project can build from source via scripts)
- **CMake 3.20+**, **Ninja**, **C++17** compiler
- (Optional) Python 3, `lit` (for lit tests)

### 2.2 One-shot build

```bash
# From project root
./scripts/build.sh
```

The script will:

1. If `$LLVM_INSTALL_DIR` is not set (default `$HOME/llvm-install`), run `scripts/build_llvm.sh` to build LLVM/MLIR first
2. Configure and build ACC under `build/` with Ninja

To use a custom LLVM install:

```bash
LLVM_INSTALL_DIR=/path/to/llvm-install ./scripts/build.sh
```

### 2.3 Acceptance (TODO 1.1)

```bash
# 1. Clean build (no warnings)
./scripts/build.sh

# 2. ac-opt works
./build/bin/ac-opt --version

# 3. Tools on PATH (optional)
source scripts/acc_env.sh
ac-opt --version
acc --help
```

---

## 3. Project structure overview

```
acc/
├── CMakeLists.txt              # Top-level CMake, LLVM/MLIR, subdirs, tests
├── include/ACC/                # Public headers and TableGen definitions
│   ├── Dialect/
│   │   ├── ACHigh/             # High-level AI dialect (Conv2D, MatMul, ReLU, GELU...)
│   │   │   ├── ACHighDialect.td, ACHighOps.td, ACHighTypes.td
│   │   │   └── *.h
│   │   ├── ACMid/              # Mid-level Tile/Loop dialect
│   │   └── ACGPU/              # [TODO] GPU dialect
│   ├── Conversion/             # ACHigh→ACMid→Linalg→LLVM Lowering
│   ├── Transforms/             # MLIR Passes: fusion, constant folding, shape inference, etc.
│   ├── Frontend/               # Lexer/Parser/AST, ONNX/Triton/Torch importers
│   ├── IROptimizer/            # LLVM IR passes
│   ├── CodeGen/                 # Code generation, JIT
│   └── InitAllDialects.h
├── lib/                        # Implementation (mirrors include/ACC)
│   ├── Dialect/ACHigh, ACMid
│   ├── Conversion/, Transforms/, Frontend/, IROptimizer/, CodeGen/, Support/
├── runtime/                    # Runtime C API + CPU kernels (im2col+GEMM, etc.)
├── tools/                      # Executables
│   ├── acc-compiler/           # Main compiler driver (binary: acc)
│   ├── ac-opt/                 # MLIR optimization (like mlir-opt)
│   ├── ac-translate/           # IR translation, ONNX import, etc.
│   └── ac-runner/              # JIT execution
├── test/                       # lit + FileCheck tests
├── unittests/                  # C++ unit tests (GoogleTest)
├── examples/e2e/               # E2E examples (Triton→ACC, PyTorch+Triton+ACC)
├── scripts/
│   ├── build_llvm.sh           # Build LLVM/MLIR
│   ├── build.sh                # Build ACC
│   ├── acc_env.sh              # Add build/bin to PATH
│   └── run_tests.sh            # Run tests
└── docs/                       # PROJECT_PLAN, etc.
```

For more detail see [PROJECT_PLAN](PROJECT_PLAN) Section 2 (Project directory structure).

---

## 4. Where to read the code

| Goal | Location | Notes |
|------|----------|-------|
| **ACHigh op definitions** | `include/ACC/Dialect/ACHigh/ACHighOps.td` | TableGen: MatMul, Conv2D, ReLU, GELU, BatchNorm, Dense... |
| **ACHigh types** | `include/ACC/Dialect/ACHigh/ACHighTypes.td` | Tensor, quantized types, etc. |
| **ACMid dialect** | `include/ACC/Dialect/ACMid/*.td` | Tile/Loop abstraction |
| **MLIR pass registration** | `include/ACC/Transforms/Passes.td`, `Passes.h` | Fusion, constant folding, shape inference, layout, DCE |
| **Lowering** | `include/ACC/Conversion/*/` | ACHighToACMid, ACMidToLinalg, ACMidToLLVM |
| **ac-opt entry** | `tools/ac-opt/main.cpp` | Registers dialects, passes, parses MLIR, runs pipeline |
| **ac-translate** | `tools/ac-translate/main.cpp` | `--import-onnx`, etc. |
| **acc compiler** | `tools/acc-compiler/` | Driver entry |
| **Dialect tests** | `test/Dialect/ACHigh/ops.mlir` | FileCheck for achigh.matmul/conv2d/relu/add |
| **Pass tests** | `test/Transforms/*.mlir` | FileCheck for each transform |
| **Conversion tests** | `test/Conversion/ACHighToACMid/basic.mlir` | Lowering tests |

---

## 5. Running tests (TODO 1.1 acceptance)

```bash
# Recommended: build then run all tests
./scripts/run_tests.sh

# Run tests only (assumes already built)
./scripts/run_tests.sh --no-build
```

Or from the build directory:

```bash
cd build && ninja check-acc
```

If `llvm-lit` is not found:

- Use LLVM’s lit: `cmake -DLLVM_EXTERNAL_LIT=/path/to/llvm-install/bin/llvm-lit ...`
- Or install lit: `pip install lit`, then `./scripts/run_tests.sh --no-build`

**TODO acceptance**: GoogleTest and lit both run; CI build succeeds.

---

## 6. E2E examples (no C++ build)

```bash
# Triton MatMul → ACC → CPU check
python3 examples/e2e/triton_matmul_to_acc.py

# PyTorch + Triton + ACC E2E
python3 examples/e2e/pytorch_triton_acc_e2e.py
```

---

## 7. Suggested next steps (aligned with TODO)

1. **Finish 1.1.4 and 1.1 acceptance**  
   Integrate GoogleTest; ensure `ninja check-acc` and CI pass; clean build and `ac-opt --version` work.

2. **1.2.7 / 1.2.8 / 1.2.10**  
   Implement `verify()` for ACHigh ops; add `InferTypeOpInterface` for Conv2D etc.; add dialect FileCheck tests.

3. **1.3 MLIR passes**  
   Implement constant folding, op fusion, shape inference, layout transform, DCE per TODO, each with FileCheck tests.

4. **1.4 Frontend**  
   ONNX import and DSL frontend (Lexer/Parser/AST → MLIR).

Progress and tasks are defined in Section 8 (Progress & TODO); this doc is both a primer and an index.

---

## 8. Progress & TODO

> When a task is done, change `[ ]` to `[x]` and note completion date or PR in the remarks.  
> Overview: Phase 1 → Phase 2 → Phase 3 → Phase 4 → Phase 5 → Phase 6.

### Overall progress

| Phase | Status | Progress | Period |
|-------|--------|----------|--------|
| Phase 1: Foundation & MLIR intro | 🚧 In progress | ░░░░░░░░░░ 0% | Week 1-4 |
| Phase 2: MLIR Lowering & LLVM IR optimization | ⏳ Not started | ░░░░░░░░░░ 0% | Week 5-8 |
| Phase 3: CodeGen, Runtime & E2E | ⏳ Not started | ░░░░░░░░░░ 0% | Week 9-12 |
| Phase 4: Advanced & multi-backend | ⏳ Not started | ░░░░░░░░░░ 0% | Week 13-14 |
| Phase 5: Triton / PyTorch integration | ⏳ Not started | ░░░░░░░░░░ 0% | Week 15-20 |
| Phase 6: Higher-level features | ⏳ Not started | ░░░░░░░░░░ 0% | Week 20+ |

### Phase 1: Foundation & MLIR intro (Week 1-4)

**1.1 Project setup (Week 1)**

- [x] **1.1.1** LLVM/MLIR build from source
- [x] **1.1.2** CMake project setup
- [x] **1.1.3** Project directory layout
- [ ] **1.1.4** Unit test framework — integrate GoogleTest
- [x] **1.1.5** lit/FileCheck test setup
- [x] **1.1.6** CI/CD config

**Acceptance**: Clean build, `ac-opt --version` works, GoogleTest and lit run, CI passes.

**1.2 MLIR basics (Week 2)**

- [ ] **1.2.1** MLIR core concepts
- [x] **1.2.2** ACHigh dialect definition
- [x] **1.2.3** Basic AI op definitions
- [x] **1.2.4** Extended op definitions
- [x] **1.2.5** Type system design
- [x] **1.2.6** Quantized type support
- [ ] **1.2.7** Op verification verify()
- [ ] **1.2.8** Shape inference InferTypeOpInterface
- [x] **1.2.9** ACMid dialect definition
- [ ] **1.2.10** Dialect tests

**1.3 MLIR pass development (Week 3)**

- [ ] **1.3.1** Pass manager overview
- [ ] **1.3.2** Constant folding pass
- [ ] **1.3.3** Op fusion pass
- [ ] **1.3.4** Shape inference pass
- [ ] **1.3.5** Layout transform pass
- [ ] **1.3.6** Dead code elimination pass
- [ ] **1.3.7** DRR pattern matching
- [ ] **1.3.8** Pass pipeline management
- [ ] **1.3.9** Pass tests

**1.4 Frontend (Week 4)**

- [ ] **1.4.1** ONNX importer design
- [ ] **1.4.2** ONNX op mapping (20+)
- [ ] **1.4.3** Weights/constants handling
- [ ] **1.4.4** DSL Lexer
- [ ] **1.4.5** DSL Parser
- [ ] **1.4.6** AST design and construction
- [ ] **1.4.7** Semantic analysis
- [ ] **1.4.8** AST → MLIR generation
- [ ] **1.4.9** Frontend tests

### Phase 2–6 summary

- **Phase 2** (Week 5-8): ACHigh → ACMid → Linalg → LLVM dialect lowering, LLVM IR passes, instruction selection and SIMD.
- **Phase 3** (Week 9-12): CPU CodeGen, runtime library, E2E execution and performance tuning.
- **Phase 4** (Week 13-14): Advanced features, CUDA/Ascend backend planning.
- **Phase 5** (Week 15-20): Triton/PyTorch integration, E2E validation.
- **Phase 6** (Week 20+): Auto-scheduling, dynamic shapes, higher-level features.

Full task list and acceptance criteria: [PROJECT_PLAN](PROJECT_PLAN) Section 3 (Phased plan).

### Milestones and test coverage

| Milestone | Target | Goal | Status |
|-----------|--------|------|--------|
| M1 | End Week 1 | Environment and project skeleton | 🚧 In progress |
| M2 | End Week 2 | ACHigh 20+ op definitions | ⏳ Not started |
| M3 | End Week 3 | 5 optimization passes | ⏳ Not started |
| M4 | End Week 4 | ONNX 20+ ops import + DSL frontend | ⏳ Not started |
| M5–M8 | End Week 6–12 | Lowering, LLVM opts, runtime, ResNet-18 inference | ⏳ Not started |

| Phase | Target coverage | Status |
|-------|------------------|--------|
| Phase 1 | > 70% | ⏳ |
| Phase 2 | > 75% | ⏳ |
| Phase 3 | > 80% | ⏳ |

---

**Last updated**: 2026-03-01
