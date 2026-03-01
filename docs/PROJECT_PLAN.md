# ACC Project Plan

## 1. Project overview

**ACC** is an AI compiler core project built on LLVM/MLIR. It is learning-by-doing: building a complete, extensible AI compiler step by step, both as a hands-on platform for learning compiler technology and as a base for a future AI Compiler Core.

The project follows a **top-down, incremental** path: starting from MLIR, from high-level graph optimization down to LLVM IR optimization, instruction mapping, and code generation, covering the full AI compiler stack.

### 1.1 Goals

| Goal type | Target | Acceptance |
|-----------|--------|------------|
| **Learning** | Master LLVM/MLIR compiler framework | Implement custom Dialect, Pass, and Lowering independently |
| **Technical** | Extensible multi-backend AI compiler | Successfully compile and run ResNet-18 |
| **Engineering** | Production-grade engineering | Code coverage >80%, complete API docs |
| **Performance** | Usable CPU backend | ≥60% of ONNX Runtime performance |
| **Extension** | Future multi-backend and framework integration | Architecture extensible to GPU/NPU |

### 1.2 Core features

- **Incremental learning**: Simple to complex, runnable code and tests at each stage
- **Example-driven**: Code examples and exercises for each compiler concept
- **Production-style architecture**: Modular design as a base for AI Compiler Core
- **Multi-backend**: CPU (implemented) + CUDA (planned) + Ascend NPU (planned)
- **Documentation**: Design docs, API docs, tutorials, and notes
- **Testing**: Unit tests + FileCheck integration tests + performance benchmarks

### 1.3 Technical roadmap

```
AI Model (ONNX / ACC DSL)
        │
        ▼
┌──────────────────────────────────────────────────────┐
│  Frontend (model import / DSL parsing)               │
│  Lexer → Parser → AST → Sema → MLIRGen               │
├──────────────────────────────────────────────────────┤
│  ACHigh Dialect  [MLIR high-level]                   │
│  Graph: op fusion, constant folding, shape inference  │
├──────────────────────────────────────────────────────┤
│  ACMid Dialect  [MLIR mid-level]                     │
│  Tile/Loop: Tiling, loop interchange, vectorization  │
├──────────────────────────────────────────────────────┤
│  MLIR built-in (Linalg / Affine / SCF / Arith)       │
│  Bufferization, Loop Lowering                         │
├──────────────────────────────────────────────────────┤
│  LLVM Dialect → LLVM IR                              │
│  LLVM opts: unroll, SLP/Loop vectorization, LTO      │
├──────────────────────────────────────────────────────┤
│  CodeGen (instruction selection / regalloc / sched)  │
│  x86-64 (SSE/AVX/AVX-512) / AArch64 (NEON/SVE)       │
├──────────────────────────────────────────────────────┤
│  Runtime + Target Binary                             │
└──────────────────────────────────────────────────────┘
        │
   ┌────┴──────────────┬──────────────────┐
   ▼                   ▼                  ▼
 CPU (x86/ARM)    NVIDIA GPU (CUDA)   Ascend NPU
 [Phase 1-3]      [TODO Phase 4]      [TODO Phase 4]
```

---

## 2. Project directory structure

### Top-level layout

```
ACC/
├── CMakeLists.txt                # Top-level CMake
├── README.md                     # Project readme
├── LICENSE                       # License
├── .gitignore                    # Git ignore
├── .github/                      # GitHub CI/CD
├── cmake/                        # CMake modules and toolchains
├── docs/                         # Project docs
├── include/                      # Public headers
│   └── ACC/
├── lib/                          # Core library implementation
│   ├── Dialect/                  # MLIR dialect definitions and impl
│   ├── Conversion/               # Lowering/Conversion between dialects
│   ├── Transforms/               # MLIR transform passes
│   ├── Frontend/                 # Lexer/Parser/AST/Sema
│   ├── IROptimizer/              # LLVM IR passes
│   ├── CodeGen/                  # Code generation
│   └── Support/                  # Shared utilities
├── runtime/                      # Runtime support
│   ├── cpu/                      # CPU Runtime
│   ├── cuda/                     # [TODO] CUDA Runtime
│   └── ascend/                   # [TODO] Ascend Runtime
├── tools/                        # Executables
│   ├── acc/                      # Main compiler driver
│   ├── ac-opt/                   # MLIR optimization (like mlir-opt)
│   ├── ac-translate/             # IR translation
│   └── ac-runner/                # JIT execution
├── test/                         # Tests
│   ├── lit.cfg.py                # lit config
│   ├── Dialect/                  # Dialect tests
│   ├── Conversion/               # Lowering tests
│   ├── Transforms/               # Transform tests
│   ├── Frontend/                 # Frontend tests
│   ├── IROptimizer/              # LLVM IR tests
│   ├── CodeGen/                  # CodeGen tests
│   └── E2E/                      # E2E tests
├── unittests/                    # C++ unit tests (GoogleTest)
│   ├── Dialect/
│   ├── Frontend/
│   └── Support/
├── examples/                     # Examples
│   ├── models/                   # Sample AI models
│   ├── dsl/                      # DSL examples
│   └── tutorials/                # Tutorials (01-build-llvm to 08-e2e-pipeline)
├── scripts/                      # Scripts
│   ├── build_llvm.sh             # Build LLVM
│   ├── build.sh                  # Build project
│   └── run_tests.sh              # Run tests
└── third_party/                  # Third-party
    └── onnx/                     # ONNX protobuf
```

### Directory notes

#### 1. `cmake/` — CMake modules

CMake modules and cross-compilation toolchains; LLVM/MLIR find and TableGen (FindLLVM.cmake, FindMLIR.cmake, AddACC.cmake, TableGen.cmake, etc.).

#### 2. `include/ACC/` — Public headers

Organized by module: Dialect (ACHigh, ACMid, ACGPU), Conversion, Transforms, Frontend, IROptimizer, CodeGen, Support; `.td` and `.h` live together and map to `lib/`.

#### 3. `lib/` — Core implementation

C++ implementation mirroring `include/ACC`; each subdir is a CMake library (Dialect, Conversion, Transforms, Frontend, IROptimizer, CodeGen, Support).

#### 4. `runtime/` — Runtime support

`ac_runtime.h` C API (handle-based); `cpu/` for CPU runtime and kernels (im2col+GEMM, etc.); `cuda/`, `ascend/` planned.

#### 5. `tools/` — Executables

`acc` main driver, `ac-opt` (MLIR), `ac-translate` (IR translation), `ac-runner` (JIT).

#### 6. `test/` and `unittests/`

`test/`: lit + FileCheck (Dialect, Conversion, Transforms, Frontend, IROptimizer, CodeGen, Performance, E2E). `unittests/`: GoogleTest.

#### 7. `examples/` — Examples and tutorials

`models/` sample ONNX, `dsl/` DSL, `tutorials/` (01-build-llvm through 08-e2e-pipeline).

#### 8. `scripts/` and `.github/`

`build_llvm.sh`, `build.sh`, `run_tests.sh`, `format.sh`; `.github/workflows` for CI/CD.

### Module dependencies

```
Frontend ──→ Dialect/ACHigh ──→ Transforms ──→ Conversion/HighToMid
                                                      │
                                               Dialect/ACMid
                                                      │
                                            Conversion/MidToLinalg
                                                      │
                                            Conversion/MidToLLVM
                                                      │
                                               IROptimizer
                                                      │
                                                  CodeGen
                                                      │
                                                  Runtime
```

### Build output

Output under `build/`: `build/bin/` (acc, ac-opt, ac-translate, ac-runner), `build/lib/` (static libs and libACRuntime.so), `build/test/`.

---

## 3. Phased plan

### Phase 1: Foundation and MLIR intro (Week 1-4)

**Goal**: Project setup, MLIR core concepts, AI dialect (20+ ops), basic optimization passes, frontend import

---

#### 1.1 Project setup (Week 1)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 1.1.1 | LLVM/MLIR build | Build LLVM 17+ and MLIR from source | Usable LLVM/MLIR dev environment |
| 1.1.2 | CMake project | Out-of-tree CMake for ACC | CMakeLists.txt and build scripts |
| 1.1.3 | Directory layout | Modular layout (Dialect/Conversion/Transforms, etc.) | Project skeleton |
| 1.1.4 | Unit test framework | Integrate GoogleTest | Runnable test framework |
| 1.1.5 | lit/FileCheck | Configure lit + FileCheck for MLIR/LLVM IR | lit.cfg.py and sample tests |
| 1.1.6 | CI/CD | GitHub Actions build and test | .github/workflows |

**Concepts**:
- LLVM CMake build (CMake + Ninja)
- LLVM/MLIR out-of-tree setup
- TableGen — LLVM DSL for code generation
- ODS (Operation Definition Specification)
- lit + FileCheck testing

**Steps**:

```bash
# 1. Build LLVM/MLIR
cd third_party && git clone --depth 1 --branch llvmorg-17.0.0 \
  https://github.com/llvm/llvm-project.git
cd llvm-project && mkdir build && cd build
cmake -G Ninja ../llvm \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON
ninja

# 2. Build ACC
cd /workspace && mkdir build && cd build
cmake -G Ninja .. \
  -DLLVM_DIR=<llvm-install>/lib/cmake/llvm \
  -DMLIR_DIR=<llvm-install>/lib/cmake/mlir
ninja
```

**Acceptance**:
- ✓ Clean build, no warnings
- ✓ `ac-opt --version` works
- ✓ GoogleTest and lit both run
- ✓ CI build passes

---

#### 1.2 MLIR basics — custom dialect and ops (Week 2)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 1.2.1 | MLIR concepts | Dialect, Operation, Type, Attribute, Region, Block | Notes |
| 1.2.2 | ACHigh dialect | Define high-level AI dialect with ODS/TableGen | ACHighDialect.td and generated C++ |
| 1.2.3 | Basic AI ops | Conv2D, MatMul, BatchMatMul, ReLU, Add, etc. | ACHighOps.td (20+ ops) |
| 1.2.4 | Extended ops | GELU, Sigmoid, BatchNorm, Softmax, Dense, ConvReLU, etc. | Full op set |
| 1.2.5 | Type system | Tensor types, shapes, dtypes | ACHighTypes.td |
| 1.2.6 | Quantized types | QuantizedType for INT8/INT4 | QuantizedType defs |
| 1.2.7 | Verification | verify() for each op | C++ verifier |
| 1.2.8 | Shape inference | InferTypeOpInterface for Conv2D, etc. | Shape inference |
| 1.2.9 | ACMid dialect | Mid-level Tile/Loop dialect | ACMidDialect.td + ACMidOps.td |
| 1.2.10 | Dialect tests | FileCheck tests for dialect | test/Dialect/*.mlir |

**Concepts**:
- Dialect definition and registration
- TableGen/ODS syntax and codegen
- Operation traits (Pure, Commutative, SameOperandsAndResultType, etc.)
- Interfaces (InferTypeOpInterface, SideEffectInterfaces)
- Region and Block
- Verification
- Assembly format (print/parse)
- Custom types and attributes

**Example — Dialect definition**:

```text
def ACHigh_Dialect : Dialect {
  let name = "achigh";
  let cppNamespace = "::acc::achigh";
  let summary = "High-level AI dialect for graph-level representation";
  let description = [{
    ACHigh provides high-level abstractions for common DL ops:
    convolution, matmul, activations, pooling, normalization, etc.
  }];
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
}
```

**Example — Op definition (Conv2D)**:

```tablegen
def ACHigh_Conv2DOp : ACHigh_Op<"conv2d",
    [Pure, DeclareOpInterfaceMethods<InferTypeOpInterface>]> {
  let summary = "2D convolution operation";
  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$weight,
    Optional<AnyRankedTensor>:$bias,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$strides,
    DefaultValuedAttr<I64ArrayAttr, "{0, 0}">:$padding,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$dilation,
    DefaultValuedAttr<I64Attr, "1">:$groups
  );
  let results = (outs AnyRankedTensor:$result);
  let hasVerifier = 1;
  let hasFolder = 1;
}
```

**Example — Verification**:

```cpp
LogicalResult Conv2DOp::verify() {
  auto inputType = getInput().getType().cast<RankedTensorType>();
  auto weightType = getWeight().getType().cast<RankedTensorType>();
  if (inputType.getRank() != 4)
    return emitOpError("input must be 4D tensor (NCHW format)");
  if (weightType.getRank() != 4)
    return emitOpError("weight must be 4D tensor (KCHW format)");
  int64_t inputChannels = inputType.getShape()[1];
  int64_t weightChannels = weightType.getShape()[1];
  if (inputChannels != weightChannels * getGroups())
    return emitOpError("input channels must equal weight channels * groups");
  return success();
}
```

**Example — MLIR IR**:

```text
func.func @simple_model(%input: tensor<1x3x224x224xf32>) -> tensor<1x1000xf32> {
  %w_conv = achigh.constant dense<...> : tensor<64x3x7x7xf32>
  %conv = achigh.conv2d %input, %w_conv {strides = [2, 2], padding = [3, 3]}
    : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
  %relu = achigh.relu(%conv) : tensor<1x64x112x112xf32>
  // ... more operations
  return %result : tensor<1x1000xf32>
}
```

**Acceptance**:
- ✓ 20+ AI ops defined (Conv2D, MatMul, BatchMatMul, ReLU, GELU, Sigmoid, Tanh, Add, Mul, Softmax, MaxPool2D, AvgPool2D, BatchNorm, Dense, Flatten, Reshape, Transpose, ConvReLU, Constant, etc.)
- ✓ All ops have verify()
- ✓ Conv2D has InferTypeOpInterface for shape inference
- ✓ Passes MLIR `--verify-diagnostics`
- ✓ ACMid dialect skeleton ready

---

#### 1.3 MLIR Pass development (Week 3)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 1.3.1 | Pass framework | PassManager, OpPassManager, registration | Notes |
| 1.3.2 | Constant folding | Compile-time constant folding | ConstantFoldingPass |
| 1.3.3 | Op fusion | Conv+BN+ReLU, Conv+ReLU fusion | OpFusionPass |
| 1.3.4 | Shape inference | Tensor shape propagation | ShapeInferencePass |
| 1.3.5 | Layout transform | NCHW/NHWC layout optimization | LayoutTransformPass |
| 1.3.6 | DCE | Remove unused ops and values | DeadCodeEliminationPass |
| 1.3.7 | DRR | Declarative Rewrite Rules | .td pattern files |
| 1.3.8 | Pass pipeline | Combine passes into pipeline | PassPipeline config |
| 1.3.9 | Pass tests | FileCheck tests per pass | test/Transforms/ |

**Concepts**:
- PassManager / OpPassManager
- FunctionPass vs. OperationPass
- Pattern rewriting (RewritePattern / matchAndRewrite)
- Greedy rewrite driver
- Declarative Rewrite Rules (DRR)
- Canonicalization
- DialectConversion vs. Greedy Rewrite
- Pass dependency and scheduling

**Example — Conv+BN+ReLU fusion**:

```cpp
struct ConvBNReLUFusionPattern : public OpRewritePattern<achigh::ReLUOp> {
  using OpRewritePattern::OpRewritePattern;

  LogicalResult matchAndRewrite(achigh::ReLUOp reluOp,
                                PatternRewriter &rewriter) const override {
    // Match: Conv -> BatchNorm -> ReLU
    auto bnOp = reluOp.getInput().getDefiningOp<achigh::BatchNormOp>();
    if (!bnOp || !bnOp->hasOneUse()) return failure();
    auto convOp = bnOp.getInput().getDefiningOp<achigh::Conv2DOp>();
    if (!convOp || !convOp->hasOneUse()) return failure();

    // Fold BN into Conv: weight_new = weight * scale/sqrt(var+eps), bias_new = ...
    auto fusedParams = foldBatchNormIntoConv(convOp, bnOp);

    // Replace with fused op
    rewriter.replaceOpWithNewOp<achigh::ConvReLUOp>(
        reluOp, reluOp.getType(),
        convOp.getInput(), fusedParams.weight, fusedParams.bias,
        convOp.getStridesAttr(), convOp.getPaddingAttr());
    return success();
  }
};
```

**Example — Shape inference**:

```cpp
struct ShapeInferencePass
    : public PassWrapper<ShapeInferencePass, OperationPass<ModuleOp>> {
  void runOnOperation() override {
    getOperation().walk([&](Operation *op) {
      if (auto shapeOp = dyn_cast<InferTypeOpInterface>(op)) {
        SmallVector<Type> inferredTypes;
        if (succeeded(shapeOp.inferReturnTypes(
                op->getContext(), op->getLoc(), op->getOperands(),
                op->getAttrDictionary(), op->getRegions(), inferredTypes))) {
          for (auto [result, type] :
               llvm::zip(op->getResults(), inferredTypes))
            result.setType(type);
        }
      }
    });
  }
};
```

**Acceptance**:
- ✓ At least 5 optimization passes
- ✓ FileCheck tests for each pass
- ✓ Fusion pass handles Conv+BN+ReLU and Conv+ReLU
- ✓ Shape inference for Conv2D, MatMul, etc.
- ✓ Pass pipeline runs in correct order

---

#### 1.4 Frontend (Week 4)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 1.4.1 | ONNX importer design | ONNX → ACHigh mapping | ONNXImporter design |
| 1.4.2 | ONNX op mapping | 20+ ONNX ops → ACHigh | Op mapping impl |
| 1.4.3 | Weights/constants | ONNX initializer → achigh.constant | Constant loading |
| 1.4.4 | DSL lexer | ACC DSL Lexer | Lexer |
| 1.4.5 | DSL parser | Recursive descent parser | Parser |
| 1.4.6 | AST | DSL AST | AST |
| 1.4.7 | Sema | Type check, symbol table | Sema |
| 1.4.8 | AST → MLIR | Generate ACHigh from AST | MLIRGen |
| 1.4.9 | Frontend tests | ONNX import and DSL tests | test/Frontend/ |

**Concepts**:
- ONNX format (protobuf) and graph
- ONNX op semantics → MLIR ops
- Frontend: Lexer → Parser → AST → Sema → CodeGen
- Recursive descent parsing
- Symbol table and scopes
- AST to MLIR (SSA construction)

**Example — ONNX importer**:

```cpp
class ONNXImporter {
  LogicalResult convertNode(const onnx::NodeProto &node) {
    StringRef opType = node.op_type();
    if (opType == "Conv")       return convertConvOp(node);
    if (opType == "MatMul")     return convertMatMulOp(node);
    if (opType == "Relu")       return convertReLUOp(node);
    if (opType == "BatchNormalization") return convertBatchNormOp(node);
    if (opType == "Add")        return convertAddOp(node);
    if (opType == "Gemm")       return convertGemmOp(node);
    if (opType == "Softmax")    return convertSoftmaxOp(node);
    if (opType == "MaxPool")    return convertMaxPoolOp(node);
    if (opType == "Reshape")    return convertReshapeOp(node);
    if (opType == "Flatten")    return convertFlattenOp(node);
    // ... 20+ ONNX ops
    return failure();
  }
};
```

**Example — ACC DSL**:

```
model SimpleNet {
  input x: tensor<1, 3, 224, 224, f32>

  layer conv1 = Conv2D(x, filters=64, kernel=7, stride=2, padding=same)
  layer relu1 = ReLU(conv1)
  layer pool1 = MaxPool2D(relu1, kernel=3, stride=2)
  layer fc1 = Dense(flatten(pool1), units=1000)
  output = Softmax(fc1)
}

compile SimpleNet -> target("cpu") {
  optimize { fuse_ops = true, vectorize = true, tiling_size = [32, 32] }
}
```

**Acceptance**:
- ✓ ONNX importer supports 20+ common ops
- ✓ Can import ResNet-18 / MobileNet
- ✓ DSL parses model and generates MLIR
- ✓ Imported MLIR passes verify

---

### Phase 2: MLIR lowering and LLVM IR optimization (Week 5-8)

**Goal**: ACHigh → ACMid → Linalg → LLVM Dialect lowering; custom LLVM IR passes; SIMD mapping

---

#### 2.1 MLIR multi-level lowering — ACHigh to Linalg (Week 5)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 2.1.1 | ACHigh → ACMid | Graph ops to Tile/Loop | ACHighToACMidPass |
| 2.1.2 | ACMid → Linalg | ACMid to Linalg generic ops | ACMidToLinalgPass |
| 2.1.3 | Conv2D lowering | Conv2D → linalg.conv_2d_nchw_fchw | Conv2D lowering |
| 2.1.4 | MatMul lowering | MatMul → linalg.matmul | MatMul lowering |
| 2.1.5 | Activation lowering | ReLU/GELU/Sigmoid → linalg.generic | Elementwise lowering |
| 2.1.6 | Tiling | Linalg tiling (L1/L2/register) | TilingPass |
| 2.1.7 | Lowering tests | FileCheck per conversion | test/Conversion/ |

**Concepts**:
- Multi-level lowering design
- DialectConversion: ConversionTarget / TypeConverter / ConversionPattern
- Partial vs. full lowering
- Linalg: Generic Op, Named Op, Indexing Maps
- Linalg on Tensors vs. Buffers
- Linalg Tiling and Fusion API
- tensor → memref mapping

**Example — Conv2D lowering**:

```cpp
struct ConvOpLowering : public OpConversionPattern<achigh::Conv2DOp> {
  using OpConversionPattern::OpConversionPattern;

  LogicalResult matchAndRewrite(achigh::Conv2DOp op, OpAdaptor adaptor,
      ConversionPatternRewriter &rewriter) const override {
    auto loc = op.getLoc();
    auto outputType = op.getType().cast<RankedTensorType>();

    // Create zero-initialized output tensor
    Value init = rewriter.create<tensor::EmptyOp>(
        loc, outputType.getShape(), outputType.getElementType());
    Value zero = rewriter.create<arith::ConstantOp>(
        loc, rewriter.getZeroAttr(outputType.getElementType()));
    Value initFilled = rewriter.create<linalg::FillOp>(loc, zero, init)
        .getResult(0);

    // Map to linalg.conv_2d_nchw_fchw
    auto convOp = rewriter.create<linalg::Conv2DNchwFchwOp>(
        loc, outputType,
        ValueRange{adaptor.getInput(), adaptor.getWeight()},
        ValueRange{initFilled}, op.getStrides(), op.getDilation());

    Value result = convOp.getResult(0);
    if (op.getBias())
      result = rewriter.create<linalg::AddOp>(loc, result, adaptor.getBias());

    rewriter.replaceOp(op, result);
    return success();
  }
};
```

**Example — MLIR after lowering**:

```text
// ACHigh → Linalg result
%result = linalg.conv_2d_nchw_fchw
  ins(%input, %weight : tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>)
  outs(%init : tensor<1x64x224x224xf32>) -> tensor<1x64x224x224xf32>
```

**Acceptance**:
- ✓ ACHigh → ACMid → Linalg pipeline runs
- ✓ Conv2D, MatMul, activations lowered
- ✓ Tiling pass tiles Linalg ops
- ✓ FileCheck tests per conversion

---

#### 2.2 MLIR lowering — Linalg to LLVM Dialect (Week 6)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 2.2.1 | Bufferization | Tensor → MemRef | Bufferization config |
| 2.2.2 | Linalg → Loops | Linalg to nested loops (SCF/Affine) | LinalgToLoopsPass |
| 2.2.3 | Loop opts | Unroll, interchange, fuse | LoopOptimizationPass |
| 2.2.4 | Vector lowering | Vector Dialect SIMD | VectorizationPass |
| 2.2.5 | Affine → SCF → CF | Lower to control-flow dialects | AffineToSCF, SCFToCF |
| 2.2.6 | CF → LLVM | Final lower to LLVM Dialect | ToLLVMDialectPass |
| 2.2.7 | E2E lowering test | ACHigh → LLVM Dialect test | E2E lowering test |

**Concepts**:
- One-Shot Bufferization
- Tensor vs. MemRef semantics
- Affine: dependence analysis, scheduling
- SCF and CF dialects
- Vector Dialect and SIMD strategy
- memref → LLVM type mapping
- MemRef descriptor conversion

**Example — Full lowering pipeline**:

```
achigh.conv2d
  ↓ ConvertACHighToACMid
acmid.tiled_conv2d
  ↓ ConvertACMidToLinalg
linalg.conv_2d_nchw_fchw
  ↓ Bufferization (tensor → memref)
linalg.conv_2d_nchw_fchw (on memrefs)
  ↓ ConvertLinalgToLoops
scf.for / affine.for (7-level nested loops)
  ↓ ConvertAffineToSCF
scf.for (pure SCF)
  ↓ ConvertSCFToCF
cf.br / cf.cond_br (basic blocks + control flow)
  ↓ ConvertToLLVM
llvm.br / llvm.load / llvm.store / llvm.fadd / llvm.fmul
```

**Acceptance**:
- ✓ Full ACHigh → LLVM Dialect pipeline
- ✓ LLVM Dialect → LLVM IR via mlir-translate
- ✓ LLVM IR → object code via llc
- ✓ Bufferization handles Tensor → MemRef

---

#### 2.3 LLVM IR optimization passes (Week 7)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 2.3.1 | LLVM IR basics | SSA, basic blocks | Notes and examples |
| 2.3.2 | MLIR → LLVM IR | Export from LLVM Dialect | IR export |
| 2.3.3 | Custom function pass | AI-oriented function pass | CustomLoopOptPass |
| 2.3.4 | Custom vectorize pass | AI workload vectorization | CustomVectorizePass |
| 2.3.5 | LLVM standard passes | O0–O3 pipeline | Pipeline config |
| 2.3.6 | Loop opts | Unroll, vectorize via Loop infra | Loop passes |
| 2.3.7 | Pass Manager | Register with NPM | PassManager integration |
| 2.3.8 | Opt analysis | opt-viewer / opt-remark | Analysis report |

**Concepts**:
- LLVM IR SSA and basic blocks
- NPM: PassInfoMixin, PreservedAnalyses
- Analyses: DominatorTree, LoopInfo, AliasAnalysis, ScalarEvolution
- Transforms: unroll, SLP/Loop vectorization
- Intrinsics and target intrinsics
- LTO and optimization remarks

**Example — Custom LLVM IR pass**:

```cpp
struct CustomLoopOptPass : public PassInfoMixin<CustomLoopOptPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    auto &LI = AM.getResult<LoopAnalysis>(F);
    for (auto *L : LI) {
      // Identify AI compute-intensive loops
      // Apply aggressive unroll, prefetch insertion
    }
    return PreservedAnalyses::all();
  }
};
```

**Example — LLVM IR vectorization**:

```llvm
; Before vectorization
for.body:
  %val = load float, float* %ptr
  %result = fadd float %val, %const
  store float %result, float* %ptr

; After vectorization (AVX2, 8-wide)
for.body.vec:
  %vec_val = load <8 x float>, <8 x float>* %ptr_vec
  %vec_result = fadd <8 x float> %vec_val, %vec_const
  store <8 x float> %vec_result, <8 x float>* %ptr_vec
```

**Acceptance**:
- ✓ Export LLVM IR from LLVM Dialect
- ✓ Custom pass in NPM and runs
- ✓ At least 10 LLVM standard passes integrated
- ✓ Loop vectorization on conv/matmul
- ✓ 30%+ perf improvement

---

#### 2.4 Instruction selection and SIMD mapping (Week 8)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 2.4.1 | LLVM backend | SelectionDAG, instruction selection, GlobalISel | Notes |
| 2.4.2 | SIMD mapping | Vector/LLVM intrinsics → AVX/NEON | SIMD mapping |
| 2.4.3 | Cost model | x86 latency/throughput | CostModel |
| 2.4.4 | FMA | Use FMA for matmul | FMA optimization |
| 2.4.5 | Asm analysis | Verify SIMD in generated asm | Asm tests |

**Concepts**:
- SelectionDAG and GlobalISel
- x86 SIMD (SSE → AVX → AVX2 → AVX-512)
- AArch64 (NEON → SVE/SVE2)
- Vector Dialect → target SIMD
- FMA and numerical impact
- Cost model in codegen

**Example — SIMD vectorization**:

```cpp
void generateAVX2MatMul(OpBuilder &builder, Value A, Value B, Value C) {
  auto f32x8Type = VectorType::get({8}, builder.getF32Type());
  // Load 8 floats (vmovups)
  auto aVec = builder.create<vector::LoadOp>(loc, f32x8Type, A, indices);
  auto bVec = builder.create<vector::BroadcastOp>(loc, f32x8Type, bScalar);
  // FMA: acc += a * b (vfmaddps)
  auto result = builder.create<vector::FMAOp>(loc, aVec, bVec, accum);
  builder.create<vector::StoreOp>(loc, result, C, indices);
}
```

**Acceptance**:
- ✓ Conv/matmul hot loops use SIMD
- ✓ Asm contains AVX/AVX2 (x86) or NEON (AArch64)
- ✓ 2–4x speedup from SIMD

---

### Phase 3: Codegen, runtime, and E2E pipeline (Week 9-12)

**Goal**: CPU codegen, high-perf runtime (im2col+GEMM), E2E compile-run, performance baseline

---

#### 3.1 CPU backend codegen (Week 9)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 3.1.1 | CodeGen driver | LLVM IR → object code | CodeGenDriver |
| 3.1.2 | TargetMachine | x86-64 / AArch64 config | CPU backend config |
| 3.1.3 | Object file | Emit .o | emitObjectFile |
| 3.1.4 | Assembly | Emit .s (for analysis) | emitAssembly |
| 3.1.5 | JIT | LLVM ORC JIT | JITEngine |
| 3.1.6 | E2E driver | Frontend→MLIR Opt→Lowering→LLVM Opt→CodeGen | CompilerDriver |

**Concepts**:
- TargetMachine and DataLayout
- MC (Machine Code) layer
- Object format (ELF / Mach-O)
- LLVM ORC JIT (LLJIT, ThreadSafeModule)
- Compiler driver design

**Example — Compiler driver**:

```cpp
class CompilerDriver {
public:
  LogicalResult compile(const std::string &inputFile,
                        const std::string &outputFile,
                        const CompilerOptions &options) {
    auto module = loadModel(inputFile);            // 1. Frontend
    PassManager pm(module.getContext());
    buildMLIRPipeline(pm, options);                // 2. MLIR opts
    if (failed(pm.run(module))) return failure();
    if (failed(lowerToLLVMDialect(module)))        // 3. Lowering
      return failure();
    auto llvmModule = translateToLLVMIR(module);   // 4. LLVM IR
    optimizeLLVMModule(llvmModule, options.optLevel); // 5. LLVM opts
    return emitObjectFile(llvmModule, outputFile); // 6. CodeGen
  }
};
```

**Acceptance**:
- ✓ Emit runnable .o / .so from MLIR
- ✓ JIT can execute MLIR/LLVM IR
- ✓ Driver runs full pipeline

---

#### 3.2 Runtime (Week 10)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 3.2.1 | Runtime C API | Handle-based C API (runtime/tensor/model) | ac_runtime.h |
| 3.2.2 | Memory | 64-byte aligned pool | MemoryPool |
| 3.2.3 | Naive kernels | Reference impl for all ops | cpu_kernels.cpp |
| 3.2.4 | im2col + GEMM | High-perf Conv2D | conv2d_im2col |
| 3.2.5 | Tiled MatMul | Cache-friendly tiled matmul | matmul_tiled |
| 3.2.6 | Multithread | OpenMP | Multithread runtime |
| 3.2.7 | Load/execute | Load compiled artifact, run inference | ac_load_model / ac_execute |
| 3.2.8 | Runtime tests | Kernel + API tests | Unit tests |

**Concepts**:
- C API (handles, error handling, ABI stability)
- 64-byte alignment (AVX-512 cache line)
- im2col: conv as matmul
- GEMM: tiling, vectorization, prefetch
- OpenMP
- Memory pool and buffer reuse

**Example — im2col + GEMM conv**:

```cpp
void ac_cpu_conv2d_im2col_f32(
    const float *input, const float *weight, const float *bias,
    float *output,
    int64_t N, int64_t C, int64_t H, int64_t W,
    int64_t K, int64_t R, int64_t S,
    int64_t stride, int64_t padding) {
  int64_t H_out = (H + 2*padding - R) / stride + 1;
  int64_t W_out = (W + 2*padding - S) / stride + 1;

  // 1. im2col: unfold input to [N*H'*W', C*R*S]
  std::vector<float> col(N * H_out * W_out * C * R * S);
  ac_cpu_im2col_f32(input, col.data(), N, C, H, W, R, S,
                     stride, stride, padding, padding, H_out, W_out);

  // 2. GEMM: output = col @ weight^T
  ac_cpu_matmul_f32(col.data(), weight, output,
                     N * H_out * W_out, K, C * R * S);

  // 3. Add bias
  if (bias) { /* ... */ }
}
```

**Example — Runtime C API**:

```c
typedef struct ACCRuntime *ac_runtime_t;
typedef struct ACCTensor  *ac_tensor_t;
typedef struct ACCModel   *ac_model_t;

ac_runtime_t ac_create_runtime(const char *backend);
ac_tensor_t  ac_alloc_tensor(ac_runtime_t rt, const int64_t *shape,
                              int ndim, ac_dtype_t dtype);
ac_model_t   ac_load_model(ac_runtime_t rt, const char *model_path);
void         ac_execute(ac_model_t model,
                         ac_tensor_t *inputs, int num_inputs,
                         ac_tensor_t *outputs, int num_outputs);
```

**Acceptance**:
- ✓ Runtime API complete and documented
- ✓ All CPU kernels pass correctness tests
- ✓ im2col+GEMM conv 5x+ over naive
- ✓ No memory leaks
- ✓ Multithread runs correctly

---

#### 3.3 E2E execution and perf tuning (Week 11-12)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 3.3.1 | E2E MatMul | DSL/MLIR → execution | E2E MatMul demo |
| 3.3.2 | E2E Conv | MLIR → execution | E2E Conv demo |
| 3.3.3 | ResNet-18 | ONNX → compile → inference | ResNet-18 demo |
| 3.3.4 | Benchmark framework | Stats, GFLOPS | benchmark.h |
| 3.3.5 | Op benchmark | Conv2D, MatMul multi-size | Op perf data |
| 3.3.6 | Model benchmark | ResNet-18, MobileNet E2E | Model perf data |
| 3.3.7 | Profiling | perf/VTune | Perf report |
| 3.3.8 | Bottleneck opts | ≥3 optimization rounds | Optimized code |

**Concepts**:
- E2E pipeline integration testing
- Benchmark: warm-up, stats, GFLOPS
- perf / VTune
- Cache analysis (miss rate, L1/L2/L3)
- Profile → Analyze → Optimize → Verify

**Performance targets**:

| Model / Op | Input | ACC target | ONNX Runtime ref | Relative |
|------------|-------|------------|------------------|----------|
| Conv2D | 1x64x56x56, k=3x3 | ≤ 2.5 ms | ~1.5 ms | 60%+ |
| MatMul | 1024x1024x1024 | ≤ 15 ms | ~10 ms | 65%+ |
| ResNet-18 | 1x3x224x224 | ≤ 30 ms | ~20 ms | 65%+ |
| MobileNetV2 | 1x3x224x224 | ≤ 20 ms | ~15 ms | 75%+ |

**Acceptance**:
- ✓ ResNet-18 E2E inference, match ONNX Runtime (rtol=1e-3)
- ✓ Conv2D op meets target (60%+ ONNX Runtime)
- ✓ Full perf baseline data
- ✓ Identify and optimize ≥3 bottlenecks
- ✓ Perf analysis report

---

### Phase 4: Advanced features and multi-backend (Week 13+)

**Goal**: Quantization, AutoTuning, parallelization; GPU/NPU backend extension framework

---

#### 4.1 Advanced compiler features (Week 13-14)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 4.1.1 | Memory opt pass | Allocation, buffer reuse | MemoryOptPass |
| 4.1.2 | Parallelization pass | Loop parallelization | ParallelizationPass |
| 4.1.3 | Quantization | INT8/FP16 | QuantizationPass |
| 4.1.4 | AutoTuning | Tiling size, unroll factor, etc. | AutoTuner |
| 4.1.5 | Profiler | Built-in perf analysis | Profiler |

**Concepts**:
- Memory opts: liveness, buffer reuse
- Quantization: symmetric/asymmetric, scale/zeropoint
- AutoTuning: grid search, Bayesian
- Loop/task parallelization

---

#### 4.2 [TODO] NVIDIA GPU CUDA backend (Week 15+)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 4.2.1 | ACGPU dialect | Grid/Block/Thread/SharedMem | ACGPU Dialect |
| 4.2.2 | ACMid → ACGPU | Tile to GPU kernel | ACMidToGPU Pass |
| 4.2.3 | GPU → NVVM | MLIR GPU/NVVM → PTX | GPUToNVVM Pass |
| 4.2.4 | Kernel opts | Shared mem, Tensor Core, bank conflict | Optimized kernels |
| 4.2.5 | CUDA runtime | CUDA runtime lib | runtime/cuda/ |

**Concepts**:
- GPU model: Grid → Block → Thread
- Shared memory, warp primitives (shuffle, reduce)
- Tensor Core (WMMA/MMA)
- LLVM NVPTX, CUDA Driver API

**Approach**:

```
ACMid Dialect
  ↓ (ACMidToGPU)
ACGPU Dialect
  ↓
MLIR GPU Dialect + NVVM Dialect
  ↓ (LLVM NVPTX Backend)
PTX Assembly → CUDA Binary
```

---

#### 4.3 [TODO] Huawei Ascend NPU backend (Week 15+)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 4.3.1 | ACAscend dialect | AI Core/Vector Core/Cube Unit | ACAscend Dialect |
| 4.3.2 | ACMid → ACAscend | Lower to Ascend dialect | MidToAscend Pass |
| 4.3.3 | Ascend IR codegen | CCE-C / TBE op desc | AscendIR CodeGen |
| 4.3.4 | CANN toolchain | CANN integration | CANN integration |
| 4.3.5 | Ascend runtime | ACL runtime lib | runtime/ascend/ |

**Concepts**:
- Ascend AI Core, Vector Core, Cube Unit
- Ascend C, TBE (Tensor Boost Engine)
- CANN toolchain
- Double-buffer, multi-core scheduling

**Approach**:

```
ACMid Dialect
  ↓ (ACMidToAscend)
ACAscend Dialect
  ↓ (Ascend Codegen)
Ascend IR (AIR) → OM Model
  ↓ (ACL Runtime)
NPU execution
```

---

## 4. Performance criteria

### 4.1 Compile-time

| Metric | Target | Measurement |
|--------|--------|-------------|
| Small model compile | < 5s | ResNet-18 |
| Large model compile | < 30s | ResNet-50 |
| Single pass time | < 1s | Any one pass |
| Peak compile memory | < 2GB | During compile |

### 4.2 Runtime

| Model | Input | ACC | ONNX Runtime | Relative |
|-------|-------|-----|--------------|----------|
| ResNet-18 | 1x3x224x224 | ≤ 30ms | ~20ms | 65% |
| ResNet-50 | 1x3x224x224 | ≤ 95ms | ~60ms | 63% |
| MobileNetV2 | 1x3x224x224 | ≤ 20ms | ~15ms | 75% |
| BERT-Base | seq=128 | ≤ 150ms | ~100ms | 65% |

### 4.3 Code quality

| Metric | Target |
|--------|--------|
| Test coverage | > 80% |
| Doc coverage | 100% (all public APIs) |
| Compile warnings | 0 |
| Clang-tidy warnings | < 10 |

---

## 5. Tech stack

| Area | Tech/Tool | Use |
|------|------------|-----|
| Compiler infra | LLVM 17+ | IR optimization, codegen |
| Multi-level IR | MLIR | Dialects, graph opts, lowering |
| Build | CMake + Ninja | Build |
| Codegen | TableGen | Dialect/op/Pass registration |
| Test (IR) | lit + FileCheck | MLIR/LLVM IR regression |
| Unit test | GoogleTest | C++ unit tests |
| Perf test | In-house benchmark | Op/model baseline |
| Model format | ONNX (protobuf) | Model import |
| Language | C++17 | Main dev language |
| Docs | Doxygen + Markdown | Code docs and notes |
| VCS | Git + GitHub | Version control, CI/CD |

---

## 6. Milestones

| Milestone | Time | Goal | Acceptance |
|-----------|------|------|------------|
| **M1** | End Week 1 | Env ready | LLVM/MLIR build, skeleton, CI |
| **M2** | End Week 2 | Dialect done | ACHigh 20+ ops, verify() |
| **M3** | End Week 3 | Passes done | 5 passes, FileCheck pass |
| **M4** | End Week 4 | Frontend done | ONNX 20+ ops, DSL parse |
| **M5** | End Week 6 | Lowering done | ACHigh → LLVM Dialect |
| **M6** | End Week 8 | LLVM opts done | Custom + standard pipeline, vectorize |
| **M7** | End Week 10 | Runtime done | im2col+GEMM, Runtime API |
| **M8** | End Week 12 | E2E done | ResNet-18 inference, 60%+ ONNX RT |

---

## 7. Test strategy

### 7.1 Test layers

| Layer | Framework | Purpose | Location |
|-------|-----------|---------|----------|
| Unit | GoogleTest | C++ correctness | unittests/ |
| Integration | lit + FileCheck | MLIR/LLVM IR transforms | test/ |
| Performance | In-house benchmark | Op/model perf | test/Performance/ |
| E2E | Python + ONNX Runtime | Numerical comparison | test/E2E/ |

### 7.2 Coverage

- **Phase 1**: > 70%
- **Phase 2**: > 75%
- **Phase 3**: > 80%
- Every new pass must have FileCheck tests
- Every new kernel must have correctness unit test

---

## 8. Learning resources

### Papers

1. **MLIR**: "MLIR: A Compiler Infrastructure for the End of Moore's Law" (CGO 2020)
2. **Polyhedral**: "Polyhedral Compilation as a Design Pattern for Compilers" (PLDI 2018)
3. **TVM**: "TVM: An Automated End-to-End Optimizing Compiler for Deep Learning" (OSDI 2018)
4. **XLA**: "XLA - TensorFlow, Compiled" (TensorFlow Dev Summit 2017)

### Books

1. **Compilers**: "Engineering a Compiler" (2nd Ed) — Cooper & Torczon
2. **LLVM**: "Getting Started with LLVM Core Libraries" — Bruno Cardoso Lopes
3. **Optimization**: "Optimizing Compilers for Modern Architectures" — Allen & Kennedy
4. **Architecture**: "Computer Architecture: A Quantitative Approach" — Hennessy & Patterson

### Online

1. [MLIR docs](https://mlir.llvm.org/) / [MLIR Toy Tutorial](https://mlir.llvm.org/docs/Tutorials/Toy/)
2. [LLVM LangRef](https://llvm.org/docs/LangRef.html) / [Writing an LLVM Pass](https://llvm.org/docs/WritingAnLLVMNewPMPass.html)
3. Stanford CS143 / MIT 6.172 / Cornell CS6120

### Open-source references

1. [IREE](https://github.com/iree-org/iree) — Google's MLIR AI compiler
2. [Torch-MLIR](https://github.com/llvm/torch-mlir) — PyTorch → MLIR
3. [ONNX-MLIR](https://github.com/onnx/onnx-mlir) — ONNX → MLIR
4. [TVM](https://github.com/apache/tvm) — Apache AI compiler

---

### Phase 5: AI framework integration — Triton / PyTorch (Week 15+)

**Goal**: Integrate ACC with PyTorch/Triton; PyTorch → Triton DSL → ACC → backend E2E compile and validation

---

#### 5.1 OpenAI Triton frontend (Week 15-16)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 5.1.1 | Triton IR analysis | TTIR/TTGIR structure and semantics | Analysis doc |
| 5.1.2 | Triton dialect | Triton-compatible dialect in ACC | ACTriton Dialect (.td) |
| 5.1.3 | Triton IR → ACC MLIR | Import Triton IR to ACHigh/ACMid | TritonImporter |
| 5.1.4 | Triton op mapping | tl.load/store/dot/reduce → ACHigh | Op mapping |
| 5.1.5 | Triton semantics | Block program, mask, etc. | Semantics tests |
| 5.1.6 | Triton → ACC → CPU | Triton kernel → ACC → CPU | E2E demo |

**Concepts**:
- Triton block-level programming
- TTIR → TTGIR
- tl.load / tl.store / tl.dot / tl.reduce
- Block pointer and mask
- Triton IR → Linalg/SCF mapping
- Dialect import/conversion

**Approach**:

```
Triton Python DSL (@triton.jit)
    │
    ▼ (Triton Compiler Frontend)
Triton IR (TTIR)
    │
    ▼ (TritonImporter)
ACC MLIR (ACHigh/ACMid)
    │
    ▼ (ACC Optimization + Lowering Pipeline)
LLVM IR → CodeGen
    │
    ├──→ CPU (x86/ARM)       [Phase 1-3]
    ├──→ NVIDIA GPU (PTX)    [TODO Phase 4]
    └──→ Ascend NPU          [TODO Phase 4]
```

**Example — Triton kernel**:

```python
import triton
import triton.language as tl

@triton.jit
def matmul_kernel(
    A_ptr, B_ptr, C_ptr,
    M, N, K,
    stride_am, stride_ak,
    stride_bk, stride_bn,
    stride_cm, stride_cn,
    BLOCK_M: tl.constexpr, BLOCK_N: tl.constexpr, BLOCK_K: tl.constexpr,
):
    pid_m = tl.program_id(0)
    pid_n = tl.program_id(1)

    offs_m = pid_m * BLOCK_M + tl.arange(0, BLOCK_M)
    offs_n = pid_n * BLOCK_N + tl.arange(0, BLOCK_N)
    offs_k = tl.arange(0, BLOCK_K)

    a_ptrs = A_ptr + offs_m[:, None] * stride_am + offs_k[None, :] * stride_ak
    b_ptrs = B_ptr + offs_k[:, None] * stride_bk + offs_n[None, :] * stride_bn

    acc = tl.zeros((BLOCK_M, BLOCK_N), dtype=tl.float32)
    for k in range(0, K, BLOCK_K):
        a = tl.load(a_ptrs, mask=offs_k[None, :] < K - k, other=0.0)
        b = tl.load(b_ptrs, mask=offs_k[:, None] < K - k, other=0.0)
        acc += tl.dot(a, b)
        a_ptrs += BLOCK_K * stride_ak
        b_ptrs += BLOCK_K * stride_bk

    c_ptrs = C_ptr + offs_m[:, None] * stride_cm + offs_n[None, :] * stride_cn
    tl.store(c_ptrs, acc, mask=(offs_m[:, None] < M) & (offs_n[None, :] < N))
```

**Example — Triton IR to ACC MLIR mapping**:

```cpp
// lib/Frontend/TritonImporter.cpp
class TritonImporter {
  LogicalResult convertTritonOp(Operation *op) {
    return llvm::TypeSwitch<Operation *, LogicalResult>(op)
        .Case<triton::LoadOp>([&](auto loadOp) {
          // tl.load → memref.load (with mask → scf.if guard)
          return convertTritonLoad(loadOp);
        })
        .Case<triton::StoreOp>([&](auto storeOp) {
          // tl.store → memref.store (with mask)
          return convertTritonStore(storeOp);
        })
        .Case<triton::DotOp>([&](auto dotOp) {
          // tl.dot → achigh.matmul (block-level matmul)
          return convertTritonDot(dotOp);
        })
        .Case<triton::ReduceOp>([&](auto reduceOp) {
          // tl.reduce → achigh.reduce
          return convertTritonReduce(reduceOp);
        })
        .Default([](Operation *) { return failure(); });
  }
};
```

**Acceptance**:
- ✓ Parse Triton matmul kernel IR
- ✓ Map tl.load/store/dot to ACC MLIR ops
- ✓ Triton kernel → ACC → CPU E2E correct
- ✓ Numerics match Triton native (rtol=1e-3)

---

#### 5.2 PyTorch integration (Week 17-18)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 5.2.1 | PyTorch export analysis | torch.export / torch.compile IR format | Analysis doc |
| 5.2.2 | Torch-MLIR | PyTorch → ACC MLIR via Torch-MLIR | TorchImporter |
| 5.2.3 | ONNX path | PyTorch → ONNX → ACC | ONNX import enhancement |
| 5.2.4 | Custom op registration | PyTorch calling ACC-compiled ops | torch.library |
| 5.2.5 | Triton op integration | Triton kernels compiled by ACC in PyTorch | Triton + ACC |
| 5.2.6 | Python binding | pybind11 | acc Python module |

**Concepts**:
- PyTorch 2.0 stack (torch.compile / TorchDynamo / TorchInductor)
- torch.export, ExportedProgram
- Torch-MLIR, torch.library, pybind11
- PyTorch + custom backend integration

**Approach**:

```
PyTorch Model (nn.Module)
    │
    ├──→ torch.export → FX Graph → Torch-MLIR → ACC MLIR  [Path 1: full graph]
    │
    ├──→ torch.onnx.export → ONNX → ACC MLIR             [Path 2: ONNX]
    │
    └──→ @triton.jit (Triton Kernel)                     [Path 3: op-level]
              │
              ▼ (TritonImporter)
         ACC MLIR → Lowering → CodeGen → Runtime
              │
              ▼
         PyTorch custom op (torch.library)
```

**Example — PyTorch + Triton + ACC E2E**:

```python
import torch
import triton
import triton.language as tl
import acc  # ACC Python binding

# ============================================================
# Step 1: Define PyTorch model
# ============================================================
class SimpleNet(torch.nn.Module):
    def __init__(self):
        super().__init__()
        self.linear1 = torch.nn.Linear(784, 256)
        self.linear2 = torch.nn.Linear(256, 10)

    def forward(self, x):
        x = torch.relu(self.linear1(x))
        x = self.linear2(x)
        return x

# ============================================================
# Step 2: Define Triton custom op
# ============================================================
@triton.jit
def fused_linear_relu_kernel(
    X_ptr, W_ptr, B_ptr, Y_ptr,
    M, N, K,
    BLOCK_M: tl.constexpr, BLOCK_N: tl.constexpr, BLOCK_K: tl.constexpr,
):
    pid_m = tl.program_id(0)
    pid_n = tl.program_id(1)
    offs_m = pid_m * BLOCK_M + tl.arange(0, BLOCK_M)
    offs_n = pid_n * BLOCK_N + tl.arange(0, BLOCK_N)

    acc = tl.zeros((BLOCK_M, BLOCK_N), dtype=tl.float32)
    for k in range(0, K, BLOCK_K):
        offs_k = k + tl.arange(0, BLOCK_K)
        a = tl.load(X_ptr + offs_m[:, None] * K + offs_k[None, :],
                     mask=(offs_m[:, None] < M) & (offs_k[None, :] < K))
        b = tl.load(W_ptr + offs_k[:, None] * N + offs_n[None, :],
                     mask=(offs_k[:, None] < K) & (offs_n[None, :] < N))
        acc += tl.dot(a, b)

    # Fused bias add + ReLU
    bias = tl.load(B_ptr + offs_n, mask=offs_n < N)
    acc = acc + bias[None, :]
    acc = tl.maximum(acc, 0.0)  # ReLU

    tl.store(Y_ptr + offs_m[:, None] * N + offs_n[None, :], acc,
             mask=(offs_m[:, None] < M) & (offs_n[None, :] < N))

# ============================================================
# Step 3: Compile Triton kernel with ACC
# ============================================================
compiler = acc.Compiler()
compiled_kernel = compiler.compile_triton(
    fused_linear_relu_kernel,
    target="cpu",
    opt_level=3,
    block_config={"BLOCK_M": 32, "BLOCK_N": 32, "BLOCK_K": 32}
)

# ============================================================
# Step 4: Register and use ACC-compiled op in PyTorch
# ============================================================
@torch.library.custom_op("acc::fused_linear_relu", mutates_args=())
def fused_linear_relu(x: torch.Tensor, w: torch.Tensor,
                       b: torch.Tensor) -> torch.Tensor:
    M, K = x.shape
    N = w.shape[1]
    y = torch.empty(M, N, dtype=x.dtype, device=x.device)
    compiled_kernel.launch(x, w, b, y, M, N, K)
    return y

# ============================================================
# Step 5: End-to-end verification
# ============================================================
model = SimpleNet()
x = torch.randn(32, 784)

# Reference: PyTorch eager
y_ref = model(x)

# ACC-compiled path
y_acc = fused_linear_relu(x, model.linear1.weight.T,
                           model.linear1.bias)

# Numerical comparison
print(f"Max diff: {(y_ref - y_acc).abs().max().item():.6f}")
assert torch.allclose(y_ref, y_acc, rtol=1e-3, atol=1e-5)
print("✓ End-to-end verification passed!")
```

**Acceptance**:
- ✓ PyTorch model imports via ONNX path
- ✓ Triton kernel compiles and runs in PyTorch
- ✓ E2E numerical verification passes
- ✓ Python binding works

---

#### 5.3 E2E validation (Week 19-20)

| # | Task | Description | Output |
|---|------|--------------|--------|
| 5.3.1 | E2E test framework | PyTorch↔ACC automated E2E tests | E2E framework |
| 5.3.2 | Triton→ACC→CPU | Triton matmul/softmax via ACC CPU | Triton E2E tests |
| 5.3.3 | PyTorch→ACC | PyTorch small models (MLP/CNN) | PyTorch E2E tests |
| 5.3.4 | Accuracy comparison | vs PyTorch eager / Triton GPU | Accuracy report |
| 5.3.5 | Perf comparison | vs PyTorch eager / TorchInductor | Perf report |

**E2E validation flow**:

```
┌─────────────────────────────────────────────────────────────┐
│                     AI Framework Layer                       │
│  ┌──────────┐    ┌──────────┐    ┌──────────────────────┐   │
│  │ PyTorch  │    │ Triton   │    │ Custom DSL (.ac)     │   │
│  │ nn.Module│    │ @jit     │    │ model SimpleNet{...} │   │
│  └────┬─────┘    └────┬─────┘    └──────────┬───────────┘   │
│       │               │                      │               │
├───────┼───────────────┼──────────────────────┼───────────────┤
│       │   Frontend    │   Import Layer        │               │
│       ▼               ▼                      ▼               │
│  ┌─────────┐   ┌──────────────┐    ┌──────────────────┐     │
│  │ONNX     │   │Triton        │    │ACC DSL           │     │
│  │Importer │   │Importer      │    │Frontend          │     │
│  └────┬────┘   └──────┬───────┘    └────────┬─────────┘     │
│       │               │                      │               │
├───────┴───────────────┴──────────────────────┴───────────────┤
│                    ACC MLIR Layer                             │
│  ┌──────────────────────────────────────────────────────┐    │
│  │  ACHigh Dialect (Graph Level)                         │    │
│  │  Op fusion / constant fold / shape inference / layout │    │
│  ├──────────────────────────────────────────────────────┤    │
│  │  ACMid Dialect (Tile/Loop Level)                      │    │
│  │  Tiling / loop opts / vectorization                   │    │
│  ├──────────────────────────────────────────────────────┤    │
│  │  Linalg / Affine / SCF → LLVM Dialect                 │    │
│  └──────────────────────────────────────────────────────┘    │
│                           │                                   │
├───────────────────────────┼───────────────────────────────────┤
│                    LLVM Layer                                 │
│  LLVM IR Optimization → CodeGen                              │
│                           │                                   │
├──────────┬────────────────┼──────────────┬────────────────────┤
│  Backend │                │              │                    │
│    ▼     │                ▼              │         ▼          │
│  CPU     │           NVIDIA GPU          │    Ascend NPU     │
│ x86/ARM  │           CUDA/PTX            │    Ascend IR      │
│ [done]   │           [TODO]             │    [TODO]          │
└──────────┴────────────────┴──────────────┴────────────────────┘
```

**Acceptance**:
- ✓ PyTorch MLP E2E compile and inference
- ✓ Triton matmul/softmax via ACC correct
- ✓ All E2E tests pass accuracy check
- ✓ Accuracy and perf comparison reports

---

### Phase 6: Advanced features (Week 20+)

**Goal**: Production-oriented features; strengthen ACC as AI Compiler Core

---

#### 6.1 Advanced feature plan

| # | Feature | Description | Priority |
|---|---------|-------------|----------|
| 6.1.1 | **Auto-Scheduling** | Cost-model-based schedule search (TVM AutoScheduler-like) | High |
| 6.1.2 | **Polyhedral** | Affine scheduling, loop transforms | High |
| 6.1.3 | **Dynamic shapes** | Dynamic batch and variable sequence length | High |
| 6.1.4 | **Mixed precision** | FP16/BF16/INT8 auto conversion and optimization | High |
| 6.1.5 | **Graph partitioning** | Large-model graph split and subgraph compile | Medium |
| 6.1.6 | **Kernel fusion** | Aggressive cross-op fusion (XLA/TorchInductor-like) | Medium |
| 6.1.7 | **Memory planning** | Compile-time global memory, minimize peak | Medium |
| 6.1.8 | **Heterogeneous** | CPU + GPU task assignment | Low |
| 6.1.9 | **Distributed compile** | Multi-device / multi-node | Low |
| 6.1.10 | **Debug/Profile** | Compiler-level debug and profiling | Medium |

#### 6.2 Auto-Scheduling framework

```
Search space → Cost model → Candidate gen → Compile+measure → Best
                    ↑                                │
                    └────────── Feedback ─────────────┘
```

| # | Task | Description | Output |
|---|------|--------------|--------|
| 6.2.1 | Search space | Tiling/Unroll/Vectorize tunables | SearchSpace |
| 6.2.2 | Analytical cost | Hardware-based static cost | AnalyticalCostModel |
| 6.2.3 | ML cost model | Trained on history | MLCostModel |
| 6.2.4 | Search algorithm | Grid / Random / Bayesian | Searcher |
| 6.2.5 | AutoTuner integration | Integrate into Pass pipeline | AutoTuner |

#### 6.3 Dynamic shape compilation

| # | Task | Description | Output |
|---|------|--------------|--------|
| 6.3.1 | Symbolic shapes | Dynamic dimensions in IR | Symbolic Shape IR |
| 6.3.2 | Shape guard | Runtime shape checks | ShapeGuard Pass |
| 6.3.3 | Specialization + fallback | Specialize common shapes, fallback for dynamic | Specialization Pass |

---

## 9. Contributing

- Follow [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- Use `clang-format`
- Document all public APIs
- Every PR must have tests
- Run `ninja check-acc` before submit

---

## 10. Changelog

### v0.4.0 (2026-02-16)

- Rename: ACompiler → ACC (AI Compiler Core)
- Phase 5: Triton/PyTorch integration, E2E validation
- Phase 6: Advanced features (Auto-Scheduling, dynamic shapes, mixed precision)
- Triton frontend (TritonImporter)
- PyTorch integration (TorchImporter)
- PyTorch + Triton + ACC E2E example

### v0.3.0 (2026-02-16)

- Restructure plan: task tables + concepts + examples + acceptance
- Reposition as AI Compiler Core (not education-only)

### v0.2.0 (2026-02-16)

- Merge plans into 4-phase structure
- Add code examples and quantified acceptance

### v0.1.0 (2026-02-16)

- Initial structure and 8-phase plan

---

**Last updated**: 2026-02-16  
**Doc version**: 4.0  
**Maintainer**: ACC Team
