# ACC Capability Radar and Priority Roadmap

> Audience: project review / technical planning
> Version: v1.0
> Date: 2026-02-17

---

## 1) Capability Radar (for reporting)

### 1.1 Scoring standard

- Score range: 0-5 (0 = not started, 5 = production-ready)
- Dimensions:
  - Architecture completeness
  - Core implementation depth
  - End-to-end usability
  - Performance optimization
  - Multi-backend extensibility
  - Engineering quality (build/test/CI/docs)

### 1.2 Current radar scores

| Dimension | Score (0-5) | Status | Evidence |
|---|---:|---|---|
| Architecture completeness | 4.5 | Strong | Multi-level design and module layout are complete (`README.md`, `include/ACC/`, `lib/`) |
| Core implementation depth | 2.0 | Early | Many modules are scaffolded but still TODO-heavy (`docs/TODO.md`) |
| End-to-end usability | 2.0 | Early | E2E Python validation exists, full compiler chain is not fully closed (`examples/e2e/`, `docs/TODO.md`) |
| Performance optimization | 2.5 | Partial | Runtime kernels and benchmark framework exist; pass-level optimization remains incomplete (`runtime/cpu/`, `test/Performance/`) |
| Multi-backend extensibility | 1.5 | Planned | CPU is WIP; CUDA/Ascend still planned (`CMakeLists.txt`, `docs/TODO.md`) |
| Engineering quality | 3.5 | Good base | CMake/Ninja, lit/FileCheck, CI already in place (`CMakeLists.txt`, `test/`, `.github/workflows/`) |

### 1.3 Radar summary (text form)

- Strengths:
  - High-quality architecture and clean compiler layering
  - Good engineering foundations for iterative delivery
- Gaps:
  - Critical path (Frontend -> Lowering -> CodeGen -> Runtime) is not fully integrated
  - Optimization and backend expansion are still framework-first

### 1.4 Overall maturity

- Current maturity: **Prototype (architecture-complete, implementation-in-progress)**
- Estimated completion level (feature + execution readiness): **~35%-45%**

---

## 2) Priority Roadmap (next 4 weeks)

### 2.1 Priority principles

- P0: unblock end-to-end compiler chain first
- P1: improve usability for real model input
- P2: improve optimization quality and measurable performance
- P3: start backend extension only after P0/P1 are stable

### 2.2 Week-by-week plan

| Week | Priority | Theme | Key deliverables | Exit criteria |
|---|---|---|---|---|
| Week 1 | P0 | Close minimum compiler chain | Implement minimal ACHigh -> ACMid -> Linalg path for `MatMul` + `ReLU`; CLI path can emit valid lowered IR | One toy model runs through full pipeline and generates executable artifact |
| Week 2 | P0/P1 | Runtime execution closure | Connect runtime load/execute path for core ops (`MatMul`, basic elementwise); stable E2E script from input model to output tensor | E2E correctness pass with numerical tolerance (rtol/atol) |
| Week 3 | P1/P2 | Frontend and optimization minimum set | Implement practical ONNX operator mapping subset (at least 8-10 common ops); enable `ConstantFolding + DCE + basic fusion` | Import and run one small ONNX model end-to-end |
| Week 4 | P2 | Performance and quality hardening | Benchmark report for `MatMul/Conv` (baseline vs optimized), add core FileCheck + runtime tests, CI gate for E2E smoke test | Have reproducible perf numbers and regression-protected CI |

---

## 3) Milestones and KPIs (for review dashboard)

### 3.1 Milestones

- M1 (end of Week 1): first full lowering chain for key ops
- M2 (end of Week 2): runnable compiler + runtime closed loop
- M3 (end of Week 3): ONNX small-model import and execution
- M4 (end of Week 4): measurable optimization gain and test gate

### 3.2 KPIs

- Functional KPIs:
  - End-to-end pass rate >= 90% on defined smoke cases
  - At least 1 real model (small ONNX) completes compile + run
- Performance KPIs:
  - Optimized `MatMul/Conv` >= 1.5x vs naive runtime baseline (same hardware/config)
- Quality KPIs:
  - Core pass tests + runtime tests in CI
  - No critical pipeline breakage for 1 week

---

## 4) Recommended execution order by module

1. `lib/Conversion/` (P0): minimal lowering for key ops first
2. `lib/CodeGen/` + `runtime/cpu/` (P0): executable closure
3. `lib/Frontend/ONNXImporter.cpp` (P1): model ingress
4. `lib/Transforms/` (P2): optimization quality and performance
5. `lib/IROptimizer/` + backend extension (P2/P3): after chain stability

---

## 5) Main risks and mitigations

- Risk A: lowering complexity causes schedule slip
  - Mitigation: constrain scope to `MatMul + ReLU` first; expand op coverage after closure
- Risk B: runtime and codegen interface mismatch
  - Mitigation: define strict ABI contract and golden integration test early
- Risk C: optimization effort lacks measurable outcomes
  - Mitigation: establish fixed benchmark harness and publish weekly delta report

---

## 6) One-page reporting version (CN + EN)

### CN (用于汇报口径)

- **定位**: ACC 当前处于“架构完整、实现推进中”的原型阶段。  
- **优势**: 分层 IR 架构清晰，工程化基础完备（构建/测试/CI）。  
- **核心短板**: 端到端主链路未完全打通，前端导入与优化实装不足。  
- **未来 4 周策略**: 先打通主链路，再补前端可用性，最后做性能与质量收敛。  
- **目标**: Week 4 前实现“可编译、可执行、可对比性能、可持续回归”的最小可用编译器闭环。  

### EN (for reporting)

- **Positioning**: ACC is in a prototype stage with strong architecture and ongoing implementation.  
- **Strengths**: clear multi-level IR layering and solid engineering foundation (build/test/CI).  
- **Primary gaps**: incomplete end-to-end chain, limited frontend practical coverage, and partial optimization implementation.  
- **4-week strategy**: close the core pipeline first, then improve frontend usability, then stabilize performance and quality gates.  
- **Goal**: by Week 4, deliver a minimal but complete loop that is compilable, executable, benchmarkable, and regression-protected.  

