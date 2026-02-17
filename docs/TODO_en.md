# ACC Project Progress Tracker

> This file provides hierarchical tracking for every task across all phases of the ACC (AI Compiler Core) project.
> Developers can complete tasks step by step to track overall project progress.
>
> **Usage**: Mark completed tasks by changing `[ ]` to `[x]`, and note the completion date or related PR.
>
> **Progress Overview**: [Phase 1](#phase-1-infrastructure--mlir-basics-week-1-4) → [Phase 2](#phase-2-mlir-lowering--llvm-ir-optimization-week-5-8) → [Phase 3](#phase-3-codegen-runtime--e2e-pipeline-week-9-12) → [Phase 4](#phase-4-advanced-features--multi-backend-week-13-14) → [Phase 5](#phase-5-ai-framework-integration--triton--pytorch-week-15-20) → [Phase 6](#phase-6-advanced-features-week-20)

---

## Overall Progress

| Phase | Status | Progress | Timeline |
|-------|--------|----------|----------|
| Phase 1: Infrastructure & MLIR Basics | 🚧 In Progress | ░░░░░░░░░░ 0% | Week 1-4 |
| Phase 2: MLIR Lowering & LLVM IR Opt | ⏳ Not Started | ░░░░░░░░░░ 0% | Week 5-8 |
| Phase 3: CodeGen, Runtime & E2E | ⏳ Not Started | ░░░░░░░░░░ 0% | Week 9-12 |
| Phase 4: Advanced Features & Multi-Backend | ⏳ Not Started | ░░░░░░░░░░ 0% | Week 13-14 |
| Phase 5: Triton / PyTorch Integration | ⏳ Not Started | ░░░░░░░░░░ 0% | Week 15-20 |
| Phase 6: Advanced Features | ⏳ Not Started | ░░░░░░░░░░ 0% | Week 20+ |

---

## Phase 1: Infrastructure & MLIR Basics (Week 1-4)

### 1.1 Project Setup (Week 1)

- [x] **1.1.1** Build LLVM/MLIR from source — Compile LLVM 17+ and MLIR
- [x] **1.1.2** CMake project setup — Build ACC out-of-tree CMake system
- [x] **1.1.3** Directory structure design — Organize by Dialect/Conversion/Transforms
- [ ] **1.1.4** Unit test framework — Integrate GoogleTest
- [x] **1.1.5** lit/FileCheck test setup — Configure lit + FileCheck
- [x] **1.1.6** CI/CD configuration — Set up GitHub Actions

**Acceptance Criteria**:
- [ ] Project builds successfully with no warnings
- [ ] `ac-opt --version` outputs correctly
- [ ] GoogleTest and lit test frameworks are runnable
- [ ] CI auto-build succeeds

---

### 1.2 MLIR Basics — Custom Dialects & Operations (Week 2)

- [ ] **1.2.1** Learn MLIR core concepts — Dialect, Operation, Type, Attribute, Region, Block
- [x] **1.2.2** Define ACHigh Dialect — ODS/TableGen high-level AI dialect
- [x] **1.2.3** Define basic AI ops — Conv2D, MatMul, BatchMatMul, ReLU, Add, etc.
- [x] **1.2.4** Define enhanced ops — GELU, Sigmoid, BatchNorm, Softmax, Dense, ConvReLU, etc.
- [x] **1.2.5** Type system design — Tensor type system
- [x] **1.2.6** Quantized type support — QuantizedType for INT8/INT4
- [ ] **1.2.7** Op verification logic — Implement verify() for all ops
- [ ] **1.2.8** Shape inference interface — Implement InferTypeOpInterface for Conv2D, etc.
- [x] **1.2.9** Define ACMid Dialect — Mid-level Tile/Loop dialect
- [ ] **1.2.10** Dialect tests — FileCheck tests for dialect correctness

**Acceptance Criteria**:
- [ ] 20+ AI operators defined successfully
- [ ] All operators have verify() implementation
- [ ] Conv2D implements InferTypeOpInterface for auto shape inference
- [ ] Pass `--verify-diagnostics` tests
- [ ] ACMid Dialect basic framework is ready

---

### 1.3 MLIR Pass Development (Week 3)

- [ ] **1.3.1** Learn Pass management — PassManager, OpPassManager, Pass registration
- [ ] **1.3.2** Constant folding Pass — Compile-time constant computation
- [ ] **1.3.3** Op fusion Pass — Conv+BN+ReLU, Conv+ReLU fusion patterns
- [ ] **1.3.4** Shape inference Pass — Forward tensor shape propagation
- [ ] **1.3.5** Layout transform Pass — NCHW/NHWC layout optimization
- [ ] **1.3.6** Dead code elimination Pass — Remove unused ops and values
- [ ] **1.3.7** DRR pattern matching — Declarative Rewrite Rules
- [ ] **1.3.8** Pass Pipeline management — Compose multiple passes into pipeline
- [ ] **1.3.9** Pass tests — FileCheck tests for each Pass

**Acceptance Criteria**:
- [ ] At least 5 optimization Passes implemented
- [ ] Each Pass has FileCheck tests
- [ ] Fusion Pass correctly optimizes Conv+BN+ReLU and Conv+ReLU patterns
- [ ] Shape inference Pass correctly infers Conv2D, MatMul output shapes
- [ ] Pass Pipeline runs in correct order

---

### 1.4 Frontend Support (Week 4)

- [ ] **1.4.1** ONNX importer design — ONNX → ACHigh Dialect mapping framework
- [ ] **1.4.2** ONNX op mapping — Map 20+ ONNX ops to ACHigh ops
- [ ] **1.4.3** Weight/constant handling — ONNX initializer → achigh.constant
- [ ] **1.4.4** DSL lexer — ACC DSL Lexer
- [ ] **1.4.5** DSL parser — Recursive descent Parser
- [ ] **1.4.6** AST design — DSL abstract syntax tree
- [ ] **1.4.7** Semantic analysis — Type checking, symbol table management
- [ ] **1.4.8** AST → MLIR generation — Generate ACHigh Dialect MLIR from AST
- [ ] **1.4.9** Frontend tests — ONNX import and DSL parsing tests

**Acceptance Criteria**:
- [ ] ONNX importer supports at least 20 common operators
- [ ] Successfully import ResNet-18 / MobileNet models
- [ ] DSL frontend can parse basic model definitions and generate MLIR
- [ ] Imported MLIR passes verify validation

---

## Phase 2: MLIR Lowering & LLVM IR Optimization (Week 5-8)

### 2.1 Multi-Level Lowering — ACHigh to Linalg (Week 5)

- [ ] **2.1.1** ACHigh → ACMid Lowering — Graph ops to Tile/Loop level
- [ ] **2.1.2** ACMid → Linalg Lowering — Map ACMid ops to Linalg generic ops
- [ ] **2.1.3** Conv2D Lowering — Conv2D → linalg.conv_2d_nchw_fchw
- [ ] **2.1.4** MatMul Lowering — MatMul → linalg.matmul
- [ ] **2.1.5** Activation Lowering — ReLU/GELU/Sigmoid → linalg.generic
- [ ] **2.1.6** Tiling strategy — Multi-level tiling (L1/L2/Register)
- [ ] **2.1.7** Lowering tests — FileCheck tests for each lowering layer

**Acceptance Criteria**:
- [ ] ACHigh → ACMid → Linalg full lowering pipeline works
- [ ] Conv2D, MatMul, activation core ops all have Lowering implemented
- [ ] Tiling Pass can tile Linalg operations
- [ ] Each lowering layer has FileCheck tests

---

### 2.2 Lowering — Linalg to LLVM Dialect (Week 6)

- [ ] **2.2.1** Bufferization — Tensor → MemRef conversion
- [ ] **2.2.2** Linalg → Loops — Expand into nested loops (SCF/Affine)
- [ ] **2.2.3** Loop optimization — Unrolling, interchange, fusion
- [ ] **2.2.4** Vectorization Lowering — SIMD vectorization via Vector Dialect
- [ ] **2.2.5** Affine → SCF → CF — Progressive lowering to control flow
- [ ] **2.2.6** CF → LLVM Dialect — Final lowering to LLVM Dialect
- [ ] **2.2.7** E2E lowering test — ACHigh to LLVM Dialect full pipeline test

**Acceptance Criteria**:
- [ ] Complete lowering pipeline from ACHigh to LLVM Dialect
- [ ] Generated LLVM Dialect IR translatable to LLVM IR via mlir-translate
- [ ] Generated LLVM IR compilable via `llc`
- [ ] Bufferization correctly handles Tensor → MemRef conversion

---

### 2.3 LLVM IR Optimization Passes (Week 7)

- [ ] **2.3.1** Learn LLVM IR basics — SSA form, basic block structure
- [ ] **2.3.2** MLIR → LLVM IR export — Export standard LLVM IR from LLVM Dialect
- [ ] **2.3.3** Custom Function Pass — AI compute pattern specialization
- [ ] **2.3.4** Custom Vectorize Pass — AI workload vectorization
- [ ] **2.3.5** Standard Pass integration — Integrate LLVM standard Pipeline (O0-O3)
- [ ] **2.3.6** Loop optimization — Loop unrolling/vectorization via LLVM Loop infra
- [ ] **2.3.7** Pass Manager integration — Register with LLVM New Pass Manager
- [ ] **2.3.8** Optimization analysis — opt-viewer / opt-remark analysis

**Acceptance Criteria**:
- [ ] Successfully export standard LLVM IR from LLVM Dialect
- [ ] Custom LLVM Passes registered in NPM and runnable
- [ ] At least 10 LLVM standard optimization Passes integrated
- [ ] Loop vectorization applied to convolution/matmul
- [ ] 30%+ performance improvement after optimization

---

### 2.4 Instruction Selection & SIMD Mapping (Week 8)

- [ ] **2.4.1** Learn LLVM backend architecture — SelectionDAG, ISel, GlobalISel
- [ ] **2.4.2** SIMD instruction mapping — Vector Dialect/LLVM Intrinsics → AVX/NEON
- [ ] **2.4.3** Instruction cost model — x86 instruction latency/throughput model
- [ ] **2.4.4** FMA instruction utilization — Ensure matmul uses FMA instructions
- [ ] **2.4.5** Assembly analysis — Verify generated assembly contains expected SIMD instructions

**Acceptance Criteria**:
- [ ] Conv/matmul core loops use SIMD instructions
- [ ] Generated assembly contains AVX/AVX2 (x86) or NEON (AArch64) instructions
- [ ] SIMD optimization yields 2-4x performance improvement

---

## Phase 3: CodeGen, Runtime & E2E Pipeline (Week 9-12)

### 3.1 CPU Backend CodeGen (Week 9)

- [ ] **3.1.1** CodeGen driver — LLVM IR → target code driver
- [ ] **3.1.2** TargetMachine configuration — x86-64 / AArch64 setup
- [ ] **3.1.3** Object file generation — Generate .o files
- [ ] **3.1.4** Assembly generation — Generate .s files
- [ ] **3.1.5** JIT compilation engine — LLVM ORC JIT
- [ ] **3.1.6** E2E compiler driver — Frontend→MLIR Opt→Lowering→LLVM Opt→CodeGen

**Acceptance Criteria**:
- [ ] Can generate runnable .o / .so files from MLIR
- [ ] JIT engine can directly execute MLIR/LLVM IR
- [ ] Compiler driver chains the complete pipeline

---

### 3.2 Runtime Library (Week 10)

- [x] **3.2.1** Runtime C API design — Handle-based C API (runtime/tensor/model)
- [x] **3.2.2** Memory management — 64-byte aligned memory pool
- [x] **3.2.3** Naive kernels — Naive reference implementations for all ops
- [x] **3.2.4** im2col + GEMM convolution — High-performance Conv2D
- [x] **3.2.5** Tiled MatMul — Cache-friendly blocked matrix multiplication
- [ ] **3.2.6** Multi-threading support — OpenMP parallel computation
- [ ] **3.2.7** Model load/execute — ac_load_model / ac_execute implementation
- [ ] **3.2.8** Runtime tests — Kernel correctness and API tests

**Acceptance Criteria**:
- [ ] Runtime API is complete with documentation
- [ ] All CPU kernels pass correctness tests
- [ ] im2col+GEMM outperforms naive by 5x+
- [ ] Memory pool works correctly, no memory leaks
- [ ] Multi-threaded execution works correctly

---

### 3.3 E2E Execution & Performance Optimization (Week 11-12)

- [ ] **3.3.1** E2E MatMul test — DSL/MLIR → execution full pipeline
- [ ] **3.3.2** E2E Conv test — MLIR → execution full pipeline
- [ ] **3.3.3** ResNet-18 inference — ONNX import → compile → inference
- [x] **3.3.4** Performance benchmark framework — Benchmark with stats, GFLOPS
- [x] **3.3.5** Single-op benchmarks — Conv2D, MatMul multi-size tests
- [ ] **3.3.6** Model benchmarks — ResNet-18, MobileNet E2E performance
- [ ] **3.3.7** Performance profiling — perf/VTune bottleneck identification
- [ ] **3.3.8** Bottleneck optimization — At least 3 rounds of Profile→Optimize

**Acceptance Criteria**:
- [ ] ResNet-18 E2E inference succeeds (rtol=1e-3)
- [ ] Conv2D single-op performance meets target (60%+ ONNX Runtime)
- [ ] Complete performance benchmark data established
- [ ] At least 3 performance bottlenecks identified and optimized
- [ ] Performance analysis report completed

---

## Phase 4: Advanced Features & Multi-Backend (Week 13-14)

### 4.1 Advanced Compiler Features

- [ ] **4.1.1** Memory optimization Pass — Buffer reuse, allocation strategy
- [ ] **4.1.2** Parallelization Pass — Loop parallelization (OpenMP)
- [ ] **4.1.3** Quantization support — INT8/FP16 quantized compilation
- [ ] **4.1.4** AutoTuning framework — Tiling Size, Unroll Factor auto-tuning
- [ ] **4.1.5** Performance profiling tool — Built-in compiler profiler

### 4.2 [TODO] NVIDIA GPU CUDA Backend

- [ ] **4.2.1** ACGPU Dialect definition — Grid/Block/Thread/SharedMem
- [ ] **4.2.2** ACMid → ACGPU Lowering — Tile level to GPU Kernel
- [ ] **4.2.3** GPU → NVVM Lowering — MLIR GPU/NVVM Dialect to PTX
- [ ] **4.2.4** Kernel optimization — Shared Memory, Tensor Core, Bank Conflict
- [ ] **4.2.5** CUDA Runtime integration — runtime/cuda/ implementation

### 4.3 [TODO] Huawei Ascend NPU Backend

- [ ] **4.3.1** ACAscend Dialect definition — AI Core/Vector Core/Cube Unit
- [ ] **4.3.2** ACMid → ACAscend Lowering — Lowering to Ascend dialect
- [ ] **4.3.3** Ascend IR generation — CCE-C / TBE operator description
- [ ] **4.3.4** CANN toolchain integration — Interface with Huawei CANN
- [ ] **4.3.5** Ascend Runtime integration — runtime/ascend/ implementation

---

## Phase 5: AI Framework Integration — Triton / PyTorch (Week 15-20)

### 5.1 OpenAI Triton Frontend (Week 15-16)

- [ ] **5.1.1** Triton IR analysis — Analyze TTIR / TTGIR structure and semantics
- [x] **5.1.2** TritonImporter framework — Triton IR → ACC MLIR import framework
- [ ] **5.1.3** Triton IR → ACC MLIR conversion — Complete op mapping
- [ ] **5.1.4** Triton op mapping — tl.load/tl.store/tl.dot/tl.reduce → ACC ops
- [ ] **5.1.5** Triton semantics preservation — Block Program, mask, etc.
- [ ] **5.1.6** Triton → ACC → CPU — E2E Triton kernel compilation

**Acceptance Criteria**:
- [ ] Successfully parse Triton matmul kernel IR
- [ ] tl.load/tl.store/tl.dot mapped to ACC MLIR ops
- [ ] Triton kernel → ACC → CPU E2E execution correct
- [ ] Numerical results match Triton native execution (rtol=1e-3)

---

### 5.2 PyTorch Integration (Week 17-18)

- [ ] **5.2.1** PyTorch export analysis — torch.export / torch.compile IR formats
- [x] **5.2.2** TorchImporter framework — Torch-MLIR / ONNX dual pathway
- [ ] **5.2.3** ONNX pathway enhancement — Enhance PyTorch → ONNX → ACC conversion
- [ ] **5.2.4** Custom op registration — PyTorch calls ACC-compiled ops (torch.library)
- [ ] **5.2.5** Triton op integration — Triton ops in PyTorch compiled by ACC
- [ ] **5.2.6** Python binding — pybind11 ACC Python module

**Acceptance Criteria**:
- [ ] PyTorch model successfully imported to ACC via ONNX pathway
- [ ] Triton kernel compiled and callable from PyTorch
- [ ] E2E numerical precision verification passes
- [ ] Python binding is functional

---

### 5.3 End-to-End Verification (Week 19-20)

- [x] **5.3.1** Triton→ACC→CPU E2E verification — Triton matmul compilation
- [x] **5.3.2** PyTorch+Triton+ACC E2E verification — MLP model E2E pipeline
- [ ] **5.3.3** PyTorch→ACC whole-graph compilation — Small model complete compilation
- [ ] **5.3.4** Accuracy comparison — vs PyTorch eager / Triton GPU
- [ ] **5.3.5** Performance comparison — vs PyTorch eager / TorchInductor

**Acceptance Criteria**:
- [ ] PyTorch MLP model E2E compilation and inference succeeds
- [ ] Triton matmul/softmax kernel compiled and executed correctly via ACC
- [ ] All E2E tests pass accuracy verification
- [ ] Complete accuracy and performance comparison report generated

---

## Phase 6: Advanced Features (Week 20+)

### 6.1 Core Advanced Features

- [ ] **6.1.1** Auto-Scheduling — Cost model + search strategy auto-tuning
- [ ] **6.1.2** Polyhedral optimization — Polyhedral model loop transformations
- [ ] **6.1.3** Dynamic shape support — Symbolic dimensions + Shape Guard + specialization
- [ ] **6.1.4** Mixed precision compilation — FP16/BF16/INT8 auto mixed precision
- [ ] **6.1.5** Graph partitioning — Large model intelligent partitioning
- [ ] **6.1.6** Kernel fusion engine — Aggressive cross-operator kernel fusion
- [ ] **6.1.7** Memory planning — Compile-time global memory planning
- [ ] **6.1.8** Heterogeneous scheduling — CPU + GPU automatic task distribution
- [ ] **6.1.9** Distributed compilation — Multi-device/multi-node model compilation
- [ ] **6.1.10** Debug/Profile integration — Compiler-level debugging and profiling tools

### 6.2 Auto-Scheduling Framework

- [ ] **6.2.1** Search space definition — Tiling/Unroll/Vectorize tunable parameters
- [ ] **6.2.2** Analytical cost model — Static cost estimation based on hardware params
- [ ] **6.2.3** ML cost model — ML-based cost model trained on historical data
- [ ] **6.2.4** Search algorithms — Grid Search / Random / Bayesian Optimization
- [ ] **6.2.5** Auto-tuning integration — Integrate search results into Pass Pipeline

### 6.3 Dynamic Shape Compilation

- [ ] **6.3.1** Symbolic shape representation — Support symbolic dynamic dimensions in IR
- [ ] **6.3.2** Shape Guard generation — Runtime shape checking code
- [ ] **6.3.3** Specialization + fallback — Common shape specialization + dynamic fallback

---

## Milestone Tracking

| Milestone | Target | Goal | Status | Completed |
|-----------|--------|------|--------|-----------|
| **M1** | Week 1 | Environment ready, project skeleton built | 🚧 In Progress | — |
| **M2** | Week 2 | ACHigh 20+ operators defined | ⏳ Not Started | — |
| **M3** | Week 3 | 5 optimization Passes implemented | ⏳ Not Started | — |
| **M4** | Week 4 | ONNX 20+ op import + DSL frontend | ⏳ Not Started | — |
| **M5** | Week 6 | ACHigh → LLVM Dialect lowering pipeline | ⏳ Not Started | — |
| **M6** | Week 8 | LLVM optimization Passes + vectorization | ⏳ Not Started | — |
| **M7** | Week 10 | Runtime API + im2col+GEMM | ⏳ Not Started | — |
| **M8** | Week 12 | ResNet-18 inference + 60%+ performance | ⏳ Not Started | — |
| **M9** | Week 16 | Triton → ACC → CPU E2E | ⏳ Not Started | — |
| **M10** | Week 20 | PyTorch + Triton + ACC E2E | ⏳ Not Started | — |

---

## Test Coverage Tracking

| Phase | Target Coverage | Current Coverage | Status |
|-------|----------------|-----------------|--------|
| Phase 1 | > 70% | — | ⏳ |
| Phase 2 | > 75% | — | ⏳ |
| Phase 3 | > 80% | — | ⏳ |

---

> **Last Updated**: 2026-02-16
> **Maintainer**: ACC Team
