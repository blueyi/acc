# ACompiler 项目规划

## 一、项目概述

**ACompiler** 是一个基于 LLVM/MLIR 基础设施实现的 AI 编译器教育项目，通过实例驱动的方式系统学习现代编译器技术。项目采用**自上而下、由浅入深**的渐进式学习路线，从 MLIR 高层抽象开始，逐步深入到 LLVM IR 优化和机器码生成。

### 1.1 项目目标

| 目标类型 | 具体目标 | 验收标准 |
|---------|---------|---------|
| **学习目标** | 掌握 LLVM/MLIR 编译器框架 | 能独立实现自定义 Dialect 和 Pass |
| **技术目标** | 实现多后端 AI 编译器 | 成功编译并运行 ResNet-18 模型 |
| **工程目标** | 建立完整工程体系 | 代码覆盖率 >80%，文档完整 |
| **性能目标** | CPU 后端性能优化 | 达到 ONNX Runtime 性能的 60%+ |

### 1.2 核心特性

- **渐进式学习**: 从简单到复杂，逐步掌握编译技术
- **实例驱动**: 每个概念都有完整的代码示例
- **多后端支持**: CPU (实现) + CUDA (规划) + Ascend (规划)
- **完整文档**: 详细的教程、API 文档和实践指南
- **测试完备**: 单元测试、集成测试、性能测试

### 1.3 技术路线总览

```
AI Model (ONNX / ACompiler DSL)
        │
        ▼
┌─────────────────────────────────────────────────┐
│  Frontend (模型导入 / DSL 解析)                    │
│  Lexer → Parser → AST → Sema → MLIRGen          │
├─────────────────────────────────────────────────┤
│  ACHigh Dialect (MLIR 高层优化)                    │
│  图级优化: 算子融合, 常量折叠, 形状推导, 布局变换    │
├─────────────────────────────────────────────────┤
│  ACMid Dialect (MLIR 中层优化)                     │
│  Tile/Loop级: Tiling, 循环交换, 向量化              │
├─────────────────────────────────────────────────┤
│  MLIR 内置方言 (Linalg/Affine/SCF/Arith)           │
│  Bufferization, Loop Lowering                    │
├─────────────────────────────────────────────────┤
│  LLVM Dialect → LLVM IR                          │
│  LLVM 优化: 循环优化, SLP/Loop 向量化, LTO          │
├─────────────────────────────────────────────────┤
│  CodeGen (指令选择 / 寄存器分配 / 指令调度)          │
│  x86-64 (SSE/AVX/AVX-512) / AArch64 (NEON/SVE)  │
├─────────────────────────────────────────────────┤
│  Runtime + Target Binary                         │
└─────────────────────────────────────────────────┘
        │
   ┌────┴──────────────┬──────────────────┐
   ▼                   ▼                  ▼
 CPU (x86/ARM)    NVIDIA GPU (CUDA)   Ascend NPU
 [Phase 1]        [TODO Phase 4]      [TODO Phase 4]
```

---

## 二、项目分期规划

### 第一阶段: 基础架构与 MLIR 入门（Week 1-4）

**目标**: 建立项目基础，掌握 MLIR 核心概念，实现自定义 AI Dialect，完成前端与基础优化 Pass

---

#### 1.1 项目工程搭建（Week 1）

**任务清单**:

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 1.1.1 | LLVM/MLIR 源码编译 | 从源码编译 LLVM 17.0+ 和 MLIR | 可用的开发环境 |
| 1.1.2 | CMake 工程搭建 | out-of-tree 项目构建系统 | CMakeLists.txt 及脚本 |
| 1.1.3 | 项目目录结构 | 模块化目录设计 | 完整项目骨架 |
| 1.1.4 | 测试框架搭建 | GoogleTest + lit/FileCheck | 测试基础设施 |
| 1.1.5 | CI/CD 配置 | GitHub Actions 自动构建 | CI 配置文件 |

**实施步骤**:

```bash
# 1. 编译 LLVM/MLIR
cd third_party
git clone --depth 1 --branch llvmorg-17.0.0 https://github.com/llvm/llvm-project.git
cd llvm-project && mkdir build && cd build
cmake -G Ninja ../llvm \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON
ninja

# 2. 编译 ACompiler
cd /workspace && mkdir build && cd build
cmake -G Ninja .. \
  -DLLVM_DIR=<llvm-install>/lib/cmake/llvm \
  -DMLIR_DIR=<llvm-install>/lib/cmake/mlir
ninja
```

**学习产出**:
- 理解 LLVM 项目的构建体系（CMake + Ninja）
- 掌握 LLVM/MLIR out-of-tree 项目的搭建方式
- 熟悉 TableGen、ODS（Operation Definition Specification）等基础设施

**验收标准**:
- 项目能成功编译
- `ac-opt --version` 正常输出
- 单元测试框架可运行

---

#### 1.2 MLIR 基础概念实践（Week 2）

##### 1.2.1 Dialect 定义

使用 ODS/TableGen 定义 ACHigh Dialect：

```tablegen
def ACHigh_Dialect : Dialect {
  let name = "achigh";
  let cppNamespace = "::acompiler::achigh";
  let summary = "High-level AI dialect for graph-level representation";
  let description = [{
    ACHigh Dialect 提供常见深度学习算子的高层抽象，
    包括卷积、矩阵运算、激活函数、池化、归一化等操作。
  }];
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
  let extraClassDeclaration = [{
    void registerOperations();
    void registerTypes();
  }];
}
```

##### 1.2.2 核心算子定义（10+ 算子）

```tablegen
// Conv2D — 支持 bias、stride、padding、dilation、groups
def ACHigh_Conv2DOp : ACHigh_Op<"conv2d",
    [Pure, DeclareOpInterfaceMethods<InferTypeOpInterface>]> {
  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$weight,
    Optional<AnyRankedTensor>:$bias,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$strides,
    DefaultValuedAttr<I64ArrayAttr, "{0, 0}">:$padding,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$dilation,
    DefaultValuedAttr<I64Attr, "1">:$groups
  );
  let results = (outs AnyRankedTensor:$output);
  let hasVerifier = 1;
  let hasFolder = 1;
}

// BatchMatMul — 支持广播和转置
def ACHigh_BatchMatMulOp : ACHigh_Op<"batch_matmul", [Pure]> {
  let arguments = (ins
    AnyRankedTensor:$lhs,
    AnyRankedTensor:$rhs,
    DefaultValuedAttr<BoolAttr, "false">:$transpose_lhs,
    DefaultValuedAttr<BoolAttr, "false">:$transpose_rhs
  );
  let results = (outs AnyRankedTensor:$output);
}

// GELU — Transformer 核心激活函数
def ACHigh_GELUOp : ACHigh_Op<"gelu", [Pure, SameOperandsAndResultType]> {
  let summary = "GELU(x) = x * Phi(x)";
  let arguments = (ins AnyRankedTensor:$input);
  let results = (outs AnyRankedTensor:$output);
}
```

##### 1.2.3 验证逻辑实现

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
  int64_t groups = getGroups();

  if (inputChannels != weightChannels * groups)
    return emitOpError("input channels must equal weight channels * groups");

  return success();
}
```

##### 1.2.4 Type System

```tablegen
// 量化类型支持 (INT8/INT4)
def ACHigh_QuantizedType : ACHigh_Type<"Quantized", "quant"> {
  let parameters = (ins
    ArrayRefParameter<"int64_t">:$shape,
    "Type":$storageType,   // int8, int4
    "Type":$expressedType, // float32
    "float":$scale,
    "int64_t":$zeroPoint
  );
}
```

**验收标准**:
- 成功定义至少 10 个 AI 算子（Conv2D, MatMul, BatchMatMul, ReLU, GELU, Sigmoid, Add, Softmax, MaxPool2D, BatchNorm, Reshape, Constant）
- 所有算子都有 `verify()` 实现
- 通过 MLIR 的 `--verify-diagnostics` 测试

---

#### 1.3 MLIR Pass 开发（Week 3）

##### 1.3.1 算子融合 Pass

**Conv + BatchNorm + ReLU 融合**:

```cpp
struct ConvBNReLUFusionPattern : public OpRewritePattern<achigh::ReLUOp> {
  using OpRewritePattern::OpRewritePattern;

  LogicalResult matchAndRewrite(achigh::ReLUOp reluOp,
                                PatternRewriter &rewriter) const override {
    auto bnOp = reluOp.getInput().getDefiningOp<achigh::BatchNormOp>();
    if (!bnOp || !bnOp->hasOneUse()) return failure();

    auto convOp = bnOp.getInput().getDefiningOp<achigh::Conv2DOp>();
    if (!convOp || !convOp->hasOneUse()) return failure();

    // 折叠 BN 参数到 Conv 权重
    auto fusedWeight = foldBatchNormIntoConv(convOp, bnOp);

    rewriter.replaceOpWithNewOp<achigh::ConvReLUOp>(
        reluOp, reluOp.getType(),
        convOp.getInput(), fusedWeight.first, fusedWeight.second,
        convOp.getStridesAttr(), convOp.getPaddingAttr());
    return success();
  }
};
```

##### 1.3.2 常量折叠 Pass

```cpp
struct ConstantFoldingPass
    : public PassWrapper<ConstantFoldingPass, OperationPass<func::FuncOp>> {
  void runOnOperation() override {
    auto func = getOperation();
    func.walk([&](Operation *op) {
      SmallVector<OpFoldResult> results;
      if (succeeded(op->fold(results))) {
        for (auto [result, foldResult] : llvm::zip(op->getResults(), results)) {
          if (auto attr = foldResult.dyn_cast<Attribute>()) {
            OpBuilder builder(op);
            auto constOp = builder.create<arith::ConstantOp>(op->getLoc(), attr);
            result.replaceAllUsesWith(constOp);
          }
        }
      }
    });
  }
};
```

##### 1.3.3 形状推导 Pass

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
          for (auto [result, inferredType] :
               llvm::zip(op->getResults(), inferredTypes)) {
            result.setType(inferredType);
          }
        }
      }
    });
  }
};
```

**需实现的 Pass 列表**:

| Pass 名称 | 命令行参数 | 说明 |
|-----------|-----------|------|
| ConstantFolding | `--ac-constant-folding` | 编译期常量计算 |
| OpFusion | `--ac-op-fusion` | Conv+BN+ReLU 等模式融合 |
| ShapeInference | `--ac-shape-inference` | Tensor 形状自动推导 |
| LayoutTransform | `--ac-layout-transform` | NCHW/NHWC 布局优化 |
| DeadCodeElimination | `--ac-dce` | 消除未使用的算子 |

**验收标准**:
- 实现至少 5 个优化 Pass
- 每个 Pass 都有对应的 FileCheck 测试用例
- 融合 Pass 能正确识别并优化 Conv+BN+ReLU 模式

---

#### 1.4 前端支持（Week 4）

##### 1.4.1 ONNX 导入器

```cpp
class ONNXImporter {
public:
  LogicalResult importModel(const std::string &modelPath,
                            mlir::ModuleOp module) {
    onnx::ModelProto model;
    std::ifstream input(modelPath, std::ios::binary);
    if (!model.ParseFromIstream(&input)) return failure();

    for (const auto &node : model.graph().node()) {
      if (failed(convertNode(node))) return failure();
    }
    return success();
  }

private:
  LogicalResult convertNode(const onnx::NodeProto &node) {
    StringRef opType = node.op_type();
    if (opType == "Conv")    return convertConvOp(node);
    if (opType == "MatMul")  return convertMatMulOp(node);
    if (opType == "Relu")    return convertReLUOp(node);
    if (opType == "Gemm")    return convertGemmOp(node);
    // ... 20+ ONNX 算子映射
    return failure();
  }
};
```

##### 1.4.2 AI DSL 前端

```
// ACompiler DSL 示例
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

**使用示例**:

```bash
# 导入 ONNX 模型
ac-translate --import-onnx resnet18.onnx -o resnet18.mlir

# 编译 DSL 源码
acompiler hello.ac -o hello
```

**验收标准**:
- ONNX 导入器支持至少 20 个常见算子
- 能成功导入 ResNet-18 / MobileNet 模型
- DSL 前端能解析基本模型定义
- 导入后的 MLIR 通过验证

---

### 第二阶段: LLVM IR 优化与 Lowering（Week 5-7）

**目标**: 实现从 MLIR 到 LLVM IR 的完整降级链路，集成 LLVM 优化 Pass

---

#### 2.1 多层降级实现（Week 5）

**降级流程**:

```
ACHigh Dialect
  ↓ (ConvertACHighToACMid)      # 图层 → Tile/Loop 层
ACMid Dialect
  ↓ (ConvertACMidToLinalg)      # Tile 层 → Linalg 通用操作
Linalg Dialect  [通用张量操作]
  ↓ (Bufferization)             # Tensor → MemRef
  ↓ (ConvertLinalgToLoops)
SCF Dialect  [结构化控制流]
  ↓ (ConvertSCFToCF)
CF Dialect  [控制流图]
  ↓ (ConvertToLLVM)
LLVM Dialect
```

**Conv2D 降级示例 (ACHigh → Linalg)**:

```cpp
struct ConvOpLowering : public OpConversionPattern<achigh::Conv2DOp> {
  using OpConversionPattern::OpConversionPattern;

  LogicalResult matchAndRewrite(
      achigh::Conv2DOp op, OpAdaptor adaptor,
      ConversionPatternRewriter &rewriter) const override {
    auto loc = op.getLoc();
    auto outputType = op.getType().cast<RankedTensorType>();

    // 创建零初始化的输出 tensor
    Value init = rewriter.create<tensor::EmptyOp>(
        loc, outputType.getShape(), outputType.getElementType());
    Value zero = rewriter.create<arith::ConstantOp>(
        loc, rewriter.getZeroAttr(outputType.getElementType()));
    Value initFilled = rewriter.create<linalg::FillOp>(
        loc, zero, init).getResult(0);

    // 创建 linalg.conv_2d_nchw_fchw
    auto convOp = rewriter.create<linalg::Conv2DNchwFchwOp>(
        loc, outputType,
        ValueRange{adaptor.getInput(), adaptor.getWeight()},
        ValueRange{initFilled},
        op.getStrides(), op.getDilation());

    // 如有 bias，添加 add
    Value result = convOp.getResult(0);
    if (op.getBias()) {
      result = rewriter.create<linalg::AddOp>(loc, result, adaptor.getBias());
    }

    rewriter.replaceOp(op, result);
    return success();
  }
};
```

**Lowering 后的 MLIR (SCF 循环)**:

```mlir
// Linalg → SCF 循环展开后
scf.for %n = 0 to 1 {
  scf.for %k = 0 to 64 {
    scf.for %h = 0 to 224 {
      scf.for %w = 0 to 224 {
        scf.for %c = 0 to 3 {
          scf.for %r = 0 to 3 {
            scf.for %s = 0 to 3 {
              %val = memref.load %input[%n, %c, %h+%r, %w+%s]
              %wgt = memref.load %weight[%k, %c, %r, %s]
              %prod = arith.mulf %val, %wgt
              %acc = memref.load %output[%n, %k, %h, %w]
              %sum = arith.addf %acc, %prod
              memref.store %sum, %output[%n, %k, %h, %w]
            }}}}}}}
```

**验收标准**:
- 完成 ACHigh → ACMid → Linalg → SCF → CF → LLVM Dialect 的完整降级
- 生成的 LLVM IR 能通过 `llc` 编译
- 每层降级都有 FileCheck 测试

---

#### 2.2 LLVM 优化 Pass（Week 6）

##### 2.2.1 自定义 LLVM IR Pass

```cpp
// 自定义循环优化 — 针对 AI 计算模式
struct CustomLoopOptPass : public PassInfoMixin<CustomLoopOptPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    auto &LI = AM.getResult<LoopAnalysis>(F);
    for (auto *L : LI) {
      // 识别计算密集型循环
      // 应用激进展开、预取指令插入
    }
    return PreservedAnalyses::all();
  }
};

// 自定义向量化 — 利用 AVX2/AVX-512
struct CustomVectorizePass : public PassInfoMixin<CustomVectorizePass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    // 识别可向量化模式，映射到 SIMD 指令
    return PreservedAnalyses::all();
  }
};
```

##### 2.2.2 LLVM 优化 Pipeline

```cpp
void buildACOptimizationPipeline(ModulePassManager &MPM,
                                  OptimizationLevel OptLevel) {
  PassBuilder PB;
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);

  // 标准 LLVM 优化 + 自定义 AI Pass
  if (OptLevel == OptimizationLevel::O3) {
    MPM = PB.buildPerModuleDefaultPipeline(OptimizationLevel::O3);
  }
}
```

**向量化前后对比**:

```llvm
; 向量化前
for.body:
  %val = load float, float* %ptr
  %result = fadd float %val, %const
  store float %result, float* %ptr

; 向量化后 (AVX2, 8-wide)
for.body.vec:
  %vec_val = load <8 x float>, <8 x float>* %ptr_vec
  %vec_result = fadd <8 x float> %vec_val, %vec_const
  store <8 x float> %vec_result, <8 x float>* %ptr_vec
```

**验收标准**:
- 集成至少 10 个 LLVM 优化 Pass
- 循环向量化成功应用到卷积/矩阵乘法
- 优化后性能提升 30%+

---

#### 2.3 指令选择与 SIMD 映射（Week 7）

##### 2.3.1 SIMD 指令映射

```cpp
// 使用 Vector Dialect 生成 AVX2 指令
void generateAVX2MatMul(OpBuilder &builder, Value A, Value B, Value C) {
  auto f32x8Type = VectorType::get({8}, builder.getF32Type());

  auto aVec = builder.create<vector::LoadOp>(loc, f32x8Type, A, indices);
  auto bVec = builder.create<vector::BroadcastOp>(loc, f32x8Type, bScalar);
  auto result = builder.create<vector::FMAOp>(loc, aVec, bVec, accum);
  builder.create<vector::StoreOp>(loc, result, C, indices);
}
```

##### 2.3.2 指令代价模型

```cpp
struct InstructionCost {
  int latency;
  float throughput;
};

const std::map<StringRef, InstructionCost> x86CostModel = {
  {"fadd",   {4, 2.0}},
  {"fmul",   {4, 2.0}},
  {"vfmadd", {4, 2.0}},  // AVX2 FMA
  {"load",   {5, 2.0}},
  {"store",  {1, 1.0}},
};
```

**验收标准**:
- 卷积/矩阵乘法使用 SIMD 指令
- 生成的汇编包含 AVX/AVX2 指令
- SIMD 优化带来 2-4x 性能提升

---

### 第三阶段: 代码生成与端到端流水线（Week 8-11）

**目标**: 实现完整的 CPU 后端，开发高性能 Runtime 库，打通端到端编译流程

---

#### 3.1 端到端编译驱动（Week 8-9）

```cpp
class CompilerDriver {
public:
  LogicalResult compile(const std::string &inputFile,
                        const std::string &outputFile,
                        const CompilerOptions &options) {
    // 1. 加载模型 (ONNX / DSL / MLIR)
    auto module = loadModel(inputFile);

    // 2. MLIR 优化 Pipeline
    PassManager pm(module.getContext());
    buildMLIRPipeline(pm, options);
    if (failed(pm.run(module))) return failure();

    // 3. 多层降级到 LLVM Dialect
    if (failed(lowerToLLVMDialect(module))) return failure();

    // 4. 转换为 LLVM IR
    auto llvmModule = translateToLLVMIR(module);

    // 5. LLVM 优化
    optimizeLLVMModule(llvmModule, options.optLevel);

    // 6. 代码生成 → Object File
    return emitObjectFile(llvmModule, outputFile);
  }

private:
  void buildMLIRPipeline(PassManager &pm, const CompilerOptions &opts) {
    pm.addPass(createShapeInferencePass());
    pm.addPass(createConstantFoldingPass());
    pm.addPass(createOpFusionPass());
    pm.addPass(createLayoutTransformPass());
    pm.addPass(createDeadCodeEliminationPass());
    // Lowering
    pm.addPass(createACHighToACMidPass());
    pm.addPass(createACMidToLinalgPass());
    pm.addPass(createLinalgTilingPass({32, 32}));
    pm.addPass(createBufferizePass());
    pm.addPass(createConvertLinalgToLoopsPass());
    pm.addPass(createConvertSCFToCFPass());
    pm.addPass(createConvertToLLVMPass());
  }
};
```

#### 3.2 Runtime 库实现（Week 9-10）

##### 3.2.1 Runtime C API

```c
// 句柄类型
typedef struct ACompilerRuntime* ac_runtime_t;
typedef struct ACompilerTensor*  ac_tensor_t;
typedef struct ACompilerModel*   ac_model_t;

typedef enum {
  AC_FLOAT32 = 0, AC_FLOAT16 = 1,
  AC_INT32 = 2, AC_INT8 = 3,
} ac_dtype_t;

// Runtime 管理
ac_runtime_t ac_create_runtime(const char *backend);
void ac_destroy_runtime(ac_runtime_t runtime);

// Tensor 操作
ac_tensor_t ac_alloc_tensor(ac_runtime_t rt, const int64_t *shape,
                            int ndim, ac_dtype_t dtype);
void ac_free_tensor(ac_tensor_t tensor);
void ac_tensor_from_host(ac_tensor_t tensor, const void *data, size_t size);
void ac_tensor_to_host(ac_tensor_t tensor, void *data, size_t size);

// 模型执行
ac_model_t ac_load_model(ac_runtime_t rt, const char *model_path);
void ac_execute(ac_model_t model,
                ac_tensor_t *inputs, int num_inputs,
                ac_tensor_t *outputs, int num_outputs);
void ac_unload_model(ac_model_t model);
```

##### 3.2.2 高性能 CPU Kernel（im2col + GEMM）

```cpp
void conv2d_im2col(const float *input, const float *weight, float *output,
                   int N, int C, int H, int W, int K, int R, int S,
                   int stride, int padding) {
  int H_out = (H + 2*padding - R) / stride + 1;
  int W_out = (W + 2*padding - S) / stride + 1;

  // 1. im2col: 展开输入为矩阵 [N*H'*W', C*R*S]
  std::vector<float> col(N*H_out*W_out * C*R*S);
  im2col_cpu(input, col.data(), N, C, H, W, R, S, stride, padding);

  // 2. GEMM: output = col @ weight^T
  //    [N*H'*W', K] = [N*H'*W', C*R*S] @ [K, C*R*S]^T
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
              N*H_out*W_out, K, C*R*S,
              1.0f, col.data(), C*R*S,
              weight, C*R*S,
              0.0f, output, K);
}
```

##### 3.2.3 多线程 Runtime

```cpp
class CPURuntime {
public:
  CPURuntime(int numThreads = -1) {
    numThreads_ = (numThreads < 0)
        ? std::thread::hardware_concurrency() : numThreads;
#ifdef USE_OPENMP
    omp_set_num_threads(numThreads_);
#endif
    memoryPool_ = std::make_unique<MemoryPool>(1024 * 1024 * 1024); // 1GB
  }

  Tensor *allocTensor(const std::vector<int64_t> &shape, DataType dtype) {
    size_t size = computeSize(shape, dtype);
    void *data = memoryPool_->allocate(size, 64); // 64字节对齐 (AVX-512)
    return new Tensor(shape, dtype, data);
  }

  void execute(const CompiledModel &model,
               const std::vector<Tensor*> &inputs,
               std::vector<Tensor*> &outputs) {
    using ModelFunc = void(*)(void**, void**);
    auto func = reinterpret_cast<ModelFunc>(model.entryPoint);
    // 准备指针数组并执行
    std::vector<void*> inPtrs, outPtrs;
    for (auto *t : inputs) inPtrs.push_back(t->data());
    for (auto *t : outputs) outPtrs.push_back(t->data());
    func(inPtrs.data(), outPtrs.data());
  }

private:
  int numThreads_;
  std::unique_ptr<MemoryPool> memoryPool_;
};
```

**验收标准**:
- 端到端编译流程正常工作
- Runtime 支持多线程执行
- 内存管理使用内存池，64 字节对齐
- im2col+GEMM 卷积实现

---

#### 3.3 性能优化与基准测试（Week 10-11）

##### 3.3.1 性能基准框架

```cpp
class OpBenchmark {
public:
  struct Result {
    double avgTimeMs;
    double stdDev;
    double gflops;
    double bandwidthGBs;
  };

  Result benchmarkOp(std::function<void()> fn, int warmup = 10,
                     int iterations = 100) {
    for (int i = 0; i < warmup; ++i) fn();
    std::vector<double> times;
    for (int i = 0; i < iterations; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      fn();
      auto end = std::chrono::high_resolution_clock::now();
      times.push_back(
          std::chrono::duration<double, std::milli>(end - start).count());
    }
    return computeStats(times);
  }
};
```

##### 3.3.2 性能目标

| 模型 | 输入规模 | ACompiler 目标 | ONNX Runtime | 相对性能 |
|------|---------|---------------|-------------|---------|
| Conv2D (单算子) | 1x64x56x56, 3x3 | 2.5 ms | 1.5 ms | 60%+ |
| MatMul (单算子) | 1024x1024x1024 | 15 ms | 10 ms | 65%+ |
| ResNet-18 (E2E) | 1x3x224x224 | 30 ms | 20 ms | 65%+ |
| MobileNetV2 | 1x3x224x224 | 20 ms | 15 ms | 75%+ |

##### 3.3.3 Profiling

```bash
# Linux perf 分析
perf record -g -e cycles,cache-misses ./benchmark
perf report

# 缓存行为分析
perf stat -e cache-references,cache-misses,L1-dcache-loads ./benchmark
```

**验收标准**:
- Conv2D 性能达标（60%+ ONNX Runtime）
- ResNet-18 端到端推理成功
- 性能分析报告完整
- 识别并优化至少 3 个性能瓶颈

---

### 第四阶段: 多后端扩展（Week 12+，TODO）

#### 4.1 [TODO] NVIDIA GPU CUDA 支持

**技术路线**:

```
ACHigh Dialect
  ↓ (ACHighToACMid)
ACMid Dialect
  ↓ (ACMidToGPU)
ACGPU Dialect (自定义)
  ↓
MLIR GPU Dialect + NVVM Dialect
  ↓ (LLVM NVPTX Backend)
PTX Assembly → CUDA Binary
```

**CUDA Kernel 生成示例**:

```cpp
void generateCUDAConv(OpBuilder &builder, achigh::Conv2DOp op) {
  auto launchOp = builder.create<gpu::LaunchOp>(loc, gridSize, blockSize);
  OpBuilder::InsertionGuard guard(builder);
  builder.setInsertionPointToStart(&launchOp.body().front());

  Value tidx = builder.create<gpu::ThreadIdOp>(loc, gpu::Dimension::x);
  Value bidx = builder.create<gpu::BlockIdOp>(loc, gpu::Dimension::x);
  // ... Kernel 计算逻辑
}
```

**实施计划** (8 周):
- Week 1-2: GPU Dialect 集成和 Kernel 生成框架
- Week 3-4: 卷积/矩阵乘法 Kernel 优化
- Week 5-6: Shared Memory 和 Tensor Core 优化
- Week 7-8: cuDNN 集成和性能调优

**优化方向**:
- Shared Memory 优化
- Warp-level 原语（Shuffle、Reduce）
- Tensor Core 映射（WMMA/MMA 指令）
- Bank Conflict 消除
- Occupancy 优化

#### 4.2 [TODO] 华为 Ascend NPU 支持

**技术路线**:

```
ACHigh Dialect
  ↓
ACAscend Dialect (自定义)
  ↓ (Ascend Codegen)
Ascend IR (AIR)
  ↓ (ATC 工具)
OM 模型 (Offline Model)
  ↓ (ACL Runtime)
NPU 执行
```

**CANN 集成示例**:

```cpp
class AscendRuntime {
public:
  void execute(const std::string &omPath,
               std::vector<Tensor*> &inputs,
               std::vector<Tensor*> &outputs) {
    uint32_t modelId;
    aclmdlLoadFromFile(omPath.c_str(), &modelId);
    auto inputDataset = createDataset(inputs);
    auto outputDataset = createDataset(outputs);
    aclmdlExecute(modelId, inputDataset, outputDataset);
    aclrtSynchronizeStream(stream_);
  }
};
```

**实施计划** (8 周):
- Week 1-2: CANN SDK 集成和 ACAscend Dialect 定义
- Week 3-4: 算子映射到 TBE 算子库
- Week 5-6: Ascend IR 生成和 OM 模型转换
- Week 7-8: 性能优化和多核调度

---

## 三、性能评估标准

### 编译时性能

| 指标 | 目标值 | 测量方法 |
|-----|-------|---------|
| 小模型编译时间 | <5s | ResNet-18 |
| 大模型编译时间 | <30s | ResNet-50 |
| 单 Pass 执行时间 | <1s | 任意单个 Pass |
| 编译内存峰值 | <2GB | 编译过程监控 |

### 运行时性能

| 模型 | 输入规模 | ACompiler | ONNX Runtime | 相对性能 |
|------|---------|-----------|--------------|---------|
| ResNet-18 | 1x3x224x224 | 30ms | 20ms | 65% |
| ResNet-50 | 1x3x224x224 | 95ms | 60ms | 63% |
| MobileNetV2 | 1x3x224x224 | 20ms | 15ms | 75% |
| BERT-Base | seq=128 | 150ms | 100ms | 65% |

### 代码质量

| 指标 | 目标值 |
|-----|-------|
| 测试覆盖率 | >80% |
| 文档覆盖率 | 100% (所有公共 API) |
| 编译警告 | 0 |
| Clang-tidy 警告 | <10 |

---

## 四、测试策略

### 4.1 单元测试 (GoogleTest)

```cpp
class Conv2DOpTest : public ::testing::Test {
protected:
  void SetUp() override {
    context = std::make_unique<MLIRContext>();
    context->loadDialect<achigh::ACHighDialect>();
    builder = std::make_unique<OpBuilder>(context.get());
  }
  std::unique_ptr<MLIRContext> context;
  std::unique_ptr<OpBuilder> builder;
};

TEST_F(Conv2DOpTest, BasicConv) {
  auto inputType = RankedTensorType::get({1, 3, 224, 224}, builder->getF32Type());
  auto weightType = RankedTensorType::get({64, 3, 3, 3}, builder->getF32Type());
  // ... 创建 Conv2D 并验证
  ASSERT_TRUE(succeeded(convOp.verify()));
  EXPECT_EQ(convOp.getType().cast<RankedTensorType>().getShape()[1], 64);
}
```

### 4.2 集成测试 (FileCheck)

```mlir
// RUN: ac-opt %s --ac-op-fusion | FileCheck %s

// CHECK-LABEL: func @test_fusion
func.func @test_fusion(%input: tensor<1x3x224x224xf32>,
                        %weight: tensor<64x3x3x3xf32>)
                        -> tensor<1x64x224x224xf32> {
  // CHECK-NOT: achigh.conv2d
  // CHECK-NOT: achigh.relu
  // CHECK: achigh.conv_relu
  %conv = achigh.conv2d %input, %weight { stride = [1, 1], padding = [1, 1] }
    : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) -> tensor<1x64x224x224xf32>
  %relu = achigh.relu(%conv) : tensor<1x64x224x224xf32>
  return %relu : tensor<1x64x224x224xf32>
}
```

### 4.3 端到端测试

```python
# test/E2E/test_resnet18.py
import numpy as np
from acompiler import Compiler, Runtime

def test_resnet18():
    compiler = Compiler()
    compiled = compiler.compile("resnet18.onnx", target="cpu", opt_level=3)
    runtime = Runtime("cpu")
    model = runtime.load_model(compiled)

    input_data = np.random.randn(1, 3, 224, 224).astype(np.float32)
    output = runtime.execute(model, [input_data])

    assert output[0].shape == (1, 1000)
    # 与 ONNX Runtime 对比精度
    np.testing.assert_allclose(output[0], ort_output[0], rtol=1e-3, atol=1e-5)
```

---

## 五、关键技术栈

| 技术领域 | 技术/工具 | 用途 |
|----------|-----------|------|
| 编译基础设施 | LLVM 17+ | IR 优化、代码生成 |
| 多层级 IR | MLIR | 自定义方言、图优化、Lowering |
| 构建系统 | CMake + Ninja | 项目构建 |
| 代码生成 | TableGen | 方言定义、指令描述 |
| 测试框架 | lit + FileCheck | MLIR/LLVM IR 测试 |
| 单元测试 | GoogleTest | C++ 单元测试 |
| 模型格式 | ONNX (protobuf) | AI 模型导入 |
| 编程语言 | C++17 | 主要开发语言 |
| 文档 | Doxygen + Markdown | 代码文档和学习笔记 |
| 版本管理 | Git + GitHub | 代码版本管理 |

---

## 六、里程碑与项目检查清单

### 里程碑定义

| 里程碑 | 时间 | 目标 | 验收标准 |
|--------|------|------|----------|
| **M1** | Week 1 末 | 环境就绪 | LLVM/MLIR 编译成功，项目骨架搭建完成 |
| **M2** | Week 2 末 | Dialect 完成 | ACHigh/ACMid Dialect 定义完成，10+ 算子可用 |
| **M3** | Week 3 末 | Pass 完成 | 5 个优化 Pass 实现并通过测试 |
| **M4** | Week 4 末 | 前端完成 | ONNX 导入 + DSL 前端基础可用 |
| **M5** | Week 7 末 | Lowering 完成 | MLIR → LLVM IR 完整降级链路打通 |
| **M6** | Week 9 末 | 端到端可运行 | 从模型到可执行代码的完整流程 |
| **M7** | Week 11 末 | 性能达标 | ResNet-18 推理成功，性能 60%+ ONNX Runtime |

### 检查清单

#### Phase 1: MLIR 基础

- [ ] AI Dialect 完整定义（10+ 算子）
- [ ] 所有算子都有验证逻辑
- [ ] 类型系统实现（含量化类型）
- [ ] 至少 5 个优化 Pass
- [ ] 融合 Pass 工作正常（Conv+BN+ReLU）
- [ ] ONNX 导入器支持 20+ 算子
- [ ] 单元测试覆盖率 >70%
- [ ] 文档完整

#### Phase 2: LLVM IR

- [ ] 完整的降级流程 (ACHigh → ACMid → Linalg → SCF → CF → LLVM)
- [ ] Runtime API 设计完成
- [ ] 集成 LLVM 优化 Pass（10+）
- [ ] 循环向量化生效
- [ ] SIMD 指令生成
- [ ] 测试覆盖率 >75%

#### Phase 3: CodeGen

- [ ] 端到端编译成功
- [ ] Runtime 完整实现（多线程 + 内存池）
- [ ] 核心算子优化（im2col+GEMM）
- [ ] Conv2D 性能达标（60%+ ONNX Runtime）
- [ ] ResNet-18 推理成功
- [ ] 性能基准建立
- [ ] 测试覆盖率 >80%

#### Phase 4: 多后端 (TODO)

- [ ] CUDA 后端规划完成
- [ ] Ascend 后端规划完成
- [ ] 设计文档齐全

---

## 七、学习资源

### 必读论文

1. **MLIR**: "MLIR: A Compiler Infrastructure for the End of Moore's Law" (CGO 2020)
2. **Polyhedral**: "Polyhedral Compilation as a Design Pattern for Compilers" (PLDI 2018)
3. **TVM**: "TVM: An Automated End-to-End Optimizing Compiler for Deep Learning" (OSDI 2018)
4. **XLA**: "XLA - TensorFlow, Compiled" (TensorFlow Dev Summit 2017)

### 推荐书籍

1. **编译原理**: "Engineering a Compiler" (2nd Edition) — Cooper & Torczon
2. **LLVM**: "Getting Started with LLVM Core Libraries" — Bruno Cardoso Lopes
3. **优化技术**: "Optimizing Compilers for Modern Architectures" — Allen & Kennedy
4. **体系结构**: "Computer Architecture: A Quantitative Approach" — Hennessy & Patterson

### 在线课程

1. **Stanford CS143**: Compilers
2. **MIT 6.172**: Performance Engineering
3. **Cornell CS6120**: Advanced Compilers

### 开源项目参考

1. [MLIR Toy Tutorial](https://mlir.llvm.org/docs/Tutorials/Toy/)
2. [IREE](https://github.com/iree-org/iree) — Google 的 MLIR 编译器
3. [Torch-MLIR](https://github.com/llvm/torch-mlir) — PyTorch → MLIR
4. [ONNX-MLIR](https://github.com/onnx/onnx-mlir) — ONNX → MLIR
5. [TVM](https://github.com/apache/tvm) — Apache AI 编译器

---

## 八、贡献指南

### 代码规范

- 遵循 [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- 使用 `clang-format` 格式化代码
- 所有公共 API 都需要文档注释
- 每个 PR 都需要测试

### 提交流程

1. Fork 仓库
2. 创建功能分支
3. 编写代码和测试
4. 运行 `ninja check-acompiler`
5. 提交 PR

---

## 九、变更日志

### v0.2.0 (2026-02-16)

- 重构项目规划为 4 阶段紧凑结构
- 增加详细代码示例和验证逻辑示范
- 增加量化验收标准和性能基准目标
- 增加 Runtime C API 句柄式设计
- 新增 BatchMatMul、GELU、BatchNorm、Sigmoid、ConvReLU 算子规划
- 新增量化类型（INT8/INT4）支持规划
- 新增 im2col+GEMM 高性能卷积实现规划
- 增加测试策略（GoogleTest + FileCheck + E2E）
- 增加贡献指南和代码规范

### v0.1.0 (2026-02-16)

- 初始项目结构
- ACHigh/ACMid Dialect 基础定义
- 基本优化 Pass 框架
- 项目规划文档和教程

---

**最后更新**: 2026-02-16
**文档版本**: 2.0（综合增强版）
**维护者**: ACompiler Team
