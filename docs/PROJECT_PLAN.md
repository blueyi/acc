# ACC 项目规划

## 一、项目概述

**ACC** 是一个基于 LLVM/MLIR 基础设施构建的 AI 编译器核心项目。项目以"从实践中学习"为出发点，通过逐步构建一个完整的、可扩展的 AI 编译器，既作为系统学习编译器技术的实战平台，也作为未来 AI Compiler Core 的基础框架。

项目采用**自上而下、由浅入深**的路线，以 MLIR 为切入点，从高层图优化逐步深入到 LLVM IR 优化、指令映射和代码生成，覆盖 AI 编译器的全链路技术栈。

### 1.1 项目目标

| 目标类型 | 具体目标 | 验收标准 |
|---------|---------|---------|
| **学习目标** | 系统掌握 LLVM/MLIR 编译器框架 | 能独立实现自定义 Dialect、Pass 和 Lowering |
| **技术目标** | 实现可扩展的多后端 AI 编译器 | 成功编译并运行 ResNet-18 模型 |
| **工程目标** | 建立生产级工程体系 | 代码覆盖率 >80%，API 文档完整 |
| **性能目标** | CPU 后端达到可用性能水平 | 达到 ONNX Runtime 性能的 60%+ |
| **扩展目标** | 支撑未来多后端和多框架接入 | 架构可扩展至 GPU/NPU 后端 |

### 1.2 核心特性

- **渐进式学习**: 从简单到复杂，每个阶段都有可运行的代码和测试
- **实例驱动**: 每个编译器概念都有完整的代码示例和实践练习
- **生产级架构**: 模块化设计，为 AI Compiler Core 奠定基础
- **多后端支持**: CPU（实现）+ CUDA（规划）+ Ascend NPU（规划）
- **完整文档**: 设计文档、API 文档、教程和学习笔记
- **测试完备**: 单元测试 + FileCheck 集成测试 + 性能基准测试

### 1.3 技术路线总览

```
AI Model (ONNX / ACC DSL)
        │
        ▼
┌──────────────────────────────────────────────────────┐
│  Frontend (模型导入 / DSL 解析)                        │
│  Lexer → Parser → AST → Sema → MLIRGen               │
├──────────────────────────────────────────────────────┤
│  ACHigh Dialect  [MLIR 高层优化]                       │
│  图级优化: 算子融合, 常量折叠, 形状推导, 布局变换        │
├──────────────────────────────────────────────────────┤
│  ACMid Dialect  [MLIR 中层优化]                        │
│  Tile/Loop 级: Tiling, 循环交换, 向量化                 │
├──────────────────────────────────────────────────────┤
│  MLIR 内置方言 (Linalg / Affine / SCF / Arith)         │
│  Bufferization, Loop Lowering                         │
├──────────────────────────────────────────────────────┤
│  LLVM Dialect → LLVM IR                               │
│  LLVM 优化: 循环展开, SLP/Loop 向量化, LTO              │
├──────────────────────────────────────────────────────┤
│  CodeGen (指令选择 / 寄存器分配 / 指令调度)              │
│  x86-64 (SSE/AVX/AVX-512) / AArch64 (NEON/SVE)       │
├──────────────────────────────────────────────────────┤
│  Runtime + Target Binary                              │
└──────────────────────────────────────────────────────┘
        │
   ┌────┴──────────────┬──────────────────┐
   ▼                   ▼                  ▼
 CPU (x86/ARM)    NVIDIA GPU (CUDA)   Ascend NPU
 [Phase 1-3]      [TODO Phase 4]      [TODO Phase 4]
```

---

## 二、项目分期规划

---

### Phase 1: 基础架构与 MLIR 入门（Week 1-4）

**阶段目标**: 搭建项目工程，掌握 MLIR 核心概念，定义 AI 方言（20+ 算子），实现基础优化 Pass，完成前端导入

---

#### 1.1 项目工程搭建（Week 1）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 1.1.1 | LLVM/MLIR 源码编译 | 从源码编译 LLVM 17+ 和 MLIR，了解构建体系 | 可用的 LLVM/MLIR 开发环境 |
| 1.1.2 | CMake 工程搭建 | 搭建 ACC 的 out-of-tree CMake 构建系统 | CMakeLists.txt 及构建脚本 |
| 1.1.3 | 项目目录结构设计 | 设计清晰的模块化目录，按 Dialect/Conversion/Transforms 等分层 | 完整的项目骨架 |
| 1.1.4 | 单元测试框架搭建 | 集成 GoogleTest，建立 C++ 单元测试基础设施 | 可运行的测试框架 |
| 1.1.5 | lit/FileCheck 测试搭建 | 配置 lit + FileCheck，用于 MLIR/LLVM IR 回归测试 | lit.cfg.py 及示例测试 |
| 1.1.6 | CI/CD 配置 | 配置 GitHub Actions 自动化构建和测试 | .github/workflows 配置 |

**核心知识点**:
- LLVM 项目的 CMake 构建体系（CMake + Ninja）
- LLVM/MLIR out-of-tree 项目的搭建方式
- TableGen 基础 — LLVM 的领域特定代码生成工具
- ODS（Operation Definition Specification）框架概念
- lit + FileCheck 测试方法论

**实施步骤**:

```bash
# 1. 编译 LLVM/MLIR
cd third_party && git clone --depth 1 --branch llvmorg-17.0.0 \
  https://github.com/llvm/llvm-project.git
cd llvm-project && mkdir build && cd build
cmake -G Ninja ../llvm \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON
ninja

# 2. 编译 ACC
cd /workspace && mkdir build && cd build
cmake -G Ninja .. \
  -DLLVM_DIR=<llvm-install>/lib/cmake/llvm \
  -DMLIR_DIR=<llvm-install>/lib/cmake/mlir
ninja
```

**验收标准**:
- ✓ 项目能成功编译，无警告
- ✓ `ac-opt --version` 正常输出
- ✓ GoogleTest 和 lit 测试框架均可运行
- ✓ CI 自动构建成功

---

#### 1.2 MLIR 基础概念实践 — 自定义方言与算子（Week 2）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 1.2.1 | MLIR 核心概念学习 | 学习 Dialect、Operation、Type、Attribute、Region、Block | 学习笔记 |
| 1.2.2 | ACHigh Dialect 定义 | 使用 ODS/TableGen 定义高层 AI 方言 | ACHighDialect.td 及生成的 C++ |
| 1.2.3 | 基本 AI 算子定义 | 定义 Conv2D、MatMul、BatchMatMul、ReLU、Add 等核心算子 | ACHighOps.td（20+ 算子） |
| 1.2.4 | 增强算子定义 | 定义 GELU、Sigmoid、BatchNorm、Softmax、Dense、ConvReLU 等 | 完整算子集 |
| 1.2.5 | Type System 设计 | 设计 Tensor 类型系统，支持不同数据类型和形状 | ACHighTypes.td |
| 1.2.6 | 量化类型支持 | 定义 QuantizedType 支持 INT8/INT4 量化推理 | QuantizedType 类型定义 |
| 1.2.7 | 算子验证逻辑 | 为每个算子实现 verify() 验证规则 | 验证逻辑 C++ 代码 |
| 1.2.8 | 形状推导接口 | 为 Conv2D 等实现 InferTypeOpInterface | 形状自动推导 |
| 1.2.9 | ACMid Dialect 定义 | 定义中层方言，表达 Tile/Loop 级别的计算 | ACMidDialect.td + ACMidOps.td |
| 1.2.10 | 方言测试 | 编写 FileCheck 测试验证方言正确性 | test/Dialect/ 下的 .mlir 测试 |

**核心知识点**:
- MLIR Dialect 的定义与注册机制
- TableGen / ODS 语法及代码生成原理
- MLIR Operation 的 Traits 机制（Pure、Commutative、SameOperandsAndResultType 等）
- MLIR Interfaces 机制（InferTypeOpInterface、SideEffectInterfaces）
- MLIR 的 Region 和 Block 结构
- MLIR 的验证（Verification）机制
- Assembly Format 声明式 IR 打印/解析
- MLIR 自定义 Type 和 Attribute 系统

**示例代码 — Dialect 定义**:

```tablegen
def ACHigh_Dialect : Dialect {
  let name = "achigh";
  let cppNamespace = "::acc::achigh";
  let summary = "High-level AI dialect for graph-level representation";
  let description = [{
    ACHigh Dialect 提供常见深度学习算子的高层抽象，
    包括卷积、矩阵运算、激活函数、池化、归一化等操作。
  }];
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
}
```

**示例代码 — 算子定义（增强版 Conv2D）**:

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

**示例代码 — 验证逻辑**:

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

**示例代码 — MLIR IR**:

```mlir
func.func @simple_model(%input: tensor<1x3x224x224xf32>) -> tensor<1x1000xf32> {
  %w_conv = achigh.constant dense<...> : tensor<64x3x7x7xf32>
  %conv = achigh.conv2d %input, %w_conv {strides = [2, 2], padding = [3, 3]}
    : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
  %relu = achigh.relu(%conv) : tensor<1x64x112x112xf32>
  // ... more operations
  return %result : tensor<1x1000xf32>
}
```

**验收标准**:
- ✓ 成功定义 20+ AI 算子（Conv2D, MatMul, BatchMatMul, ReLU, GELU, Sigmoid, Tanh, Add, Mul, Softmax, MaxPool2D, AvgPool2D, BatchNorm, Dense, Flatten, Reshape, Transpose, ConvReLU, Constant 等）
- ✓ 所有算子都有 verify() 实现
- ✓ Conv2D 实现 InferTypeOpInterface 自动形状推导
- ✓ 通过 MLIR 的 `--verify-diagnostics` 测试
- ✓ ACMid Dialect 基本框架就绪

---

#### 1.3 MLIR Pass 开发（Week 3）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 1.3.1 | Pass 管理框架学习 | 理解 PassManager、OpPassManager、Pass 注册机制 | 学习笔记 |
| 1.3.2 | 常量折叠 Pass | 实现编译期常量计算优化 | ConstantFoldingPass |
| 1.3.3 | 算子融合 Pass | 实现 Conv+BN+ReLU、Conv+ReLU 等经典融合模式 | OpFusionPass |
| 1.3.4 | 形状推导 Pass | 实现 Tensor 形状的前向自动推导 | ShapeInferencePass |
| 1.3.5 | 布局变换 Pass | 实现 NCHW/NHWC 等数据布局变换优化 | LayoutTransformPass |
| 1.3.6 | 死代码消除 Pass | 消除未使用的算子和中间变量 | DeadCodeEliminationPass |
| 1.3.7 | DRR 模式匹配 | 使用 Declarative Rewrite Rules 实现模式替换 | .td 模式定义文件 |
| 1.3.8 | Pass Pipeline 管理 | 组合多个 Pass 形成优化流水线 | PassPipeline 配置 |
| 1.3.9 | Pass 测试 | 为每个 Pass 编写 FileCheck 测试 | test/Transforms/ 下的测试 |

**核心知识点**:
- MLIR Pass 管理框架（PassManager / OpPassManager）
- FunctionPass vs. OperationPass 的区别
- MLIR Pattern Rewriting 机制（RewritePattern / matchAndRewrite）
- Greedy Pattern Rewriting Driver
- Declarative Rewrite Rules (DRR) — TableGen 声明式模式替换
- Canonicalization 机制
- MLIR DialectConversion 框架 — Greedy Rewrite vs. Dialect Conversion 的选择
- Pass 的依赖管理和调度策略

**示例代码 — Conv+BN+ReLU 融合**:

```cpp
struct ConvBNReLUFusionPattern : public OpRewritePattern<achigh::ReLUOp> {
  using OpRewritePattern::OpRewritePattern;

  LogicalResult matchAndRewrite(achigh::ReLUOp reluOp,
                                PatternRewriter &rewriter) const override {
    // 匹配模式: Conv -> BatchNorm -> ReLU
    auto bnOp = reluOp.getInput().getDefiningOp<achigh::BatchNormOp>();
    if (!bnOp || !bnOp->hasOneUse()) return failure();
    auto convOp = bnOp.getInput().getDefiningOp<achigh::Conv2DOp>();
    if (!convOp || !convOp->hasOneUse()) return failure();

    // 折叠 BN 参数到 Conv 权重中
    // weight_new = weight * scale / sqrt(variance + epsilon)
    // bias_new = (bias - mean) * scale / sqrt(variance + epsilon) + bnBias
    auto fusedParams = foldBatchNormIntoConv(convOp, bnOp);

    // 替换为融合算子
    rewriter.replaceOpWithNewOp<achigh::ConvReLUOp>(
        reluOp, reluOp.getType(),
        convOp.getInput(), fusedParams.weight, fusedParams.bias,
        convOp.getStridesAttr(), convOp.getPaddingAttr());
    return success();
  }
};
```

**示例代码 — 形状推导**:

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

**验收标准**:
- ✓ 实现至少 5 个优化 Pass
- ✓ 每个 Pass 都有对应的 FileCheck 测试用例
- ✓ 融合 Pass 能正确识别并优化 Conv+BN+ReLU 和 Conv+ReLU 模式
- ✓ 形状推导 Pass 能正确推导 Conv2D、MatMul 等算子的输出形状
- ✓ Pass Pipeline 能按正确顺序组合运行

---

#### 1.4 前端支持（Week 4）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 1.4.1 | ONNX 导入器设计 | 设计 ONNX → ACHigh Dialect 的映射框架 | ONNXImporter 架构 |
| 1.4.2 | ONNX 算子映射 | 实现 20+ ONNX 算子到 ACHigh 算子的映射 | 算子映射实现 |
| 1.4.3 | 权重/常量处理 | 实现 ONNX initializer 到 achigh.constant 的转换 | 常量加载逻辑 |
| 1.4.4 | DSL 词法分析器 | 实现 ACC DSL 的 Lexer | Lexer 模块 |
| 1.4.5 | DSL 语法分析器 | 实现递归下降 Parser | Parser 模块 |
| 1.4.6 | AST 设计与构建 | 设计 DSL 的抽象语法树 | AST 模块 |
| 1.4.7 | 语义分析 | 类型检查、符号表管理 | Sema 模块 |
| 1.4.8 | AST → MLIR 生成 | 从 AST 生成 ACHigh Dialect MLIR | MLIRGen 模块 |
| 1.4.9 | 前端测试 | 编写 ONNX 导入和 DSL 解析测试 | test/Frontend/ 下的测试 |

**核心知识点**:
- ONNX 模型格式（protobuf）和计算图结构
- ONNX 算子规范与 MLIR 算子的语义映射
- 编译器前端经典架构（Lexer → Parser → AST → Sema → CodeGen）
- 递归下降解析算法
- 符号表和作用域管理
- AST 到 MLIR 的翻译策略（SSA 构建）

**示例代码 — ONNX 导入器**:

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
    // ... 20+ ONNX 算子
    return failure();
  }
};
```

**示例代码 — ACC DSL**:

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

**验收标准**:
- ✓ ONNX 导入器支持至少 20 个常见算子
- ✓ 能成功导入 ResNet-18 / MobileNet 模型
- ✓ DSL 前端能解析基本模型定义并生成 MLIR
- ✓ 导入后的 MLIR 通过 verify 验证

---

### Phase 2: MLIR Lowering 与 LLVM IR 优化（Week 5-8）

**阶段目标**: 实现 ACHigh → ACMid → Linalg → LLVM Dialect 的完整降级链路，编写自定义 LLVM IR 优化 Pass，实现 SIMD 指令映射

---

#### 2.1 MLIR 多层 Lowering — 从 ACHigh 到 Linalg（Week 5）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 2.1.1 | ACHigh → ACMid Lowering | 图层算子到 Tile/Loop 层的 Lowering | ACHighToACMidPass |
| 2.1.2 | ACMid → Linalg Lowering | 将 ACMid 操作映射到 Linalg 通用操作 | ACMidToLinalgPass |
| 2.1.3 | Conv2D Lowering 实现 | Conv2D 到 linalg.conv_2d_nchw_fchw 的具体映射 | Conv2D 降级代码 |
| 2.1.4 | MatMul Lowering 实现 | MatMul 到 linalg.matmul 的具体映射 | MatMul 降级代码 |
| 2.1.5 | 激活函数 Lowering | ReLU/GELU/Sigmoid 到 linalg.generic 的映射 | 逐元素算子降级 |
| 2.1.6 | Tiling 策略实现 | 基于 Linalg Tiling 实现多层 Tile（L1/L2/Register 级） | TilingPass |
| 2.1.7 | Lowering 测试 | 为每层 Lowering 编写 FileCheck 测试 | test/Conversion/ 下的测试 |

**核心知识点**:
- MLIR 的多层 Lowering 架构设计
- DialectConversion 框架 — ConversionTarget / TypeConverter / ConversionPattern
- Partial Lowering vs. Full Lowering
- Linalg Dialect 核心概念（Generic Op、Named Op、Indexing Maps）
- Linalg on Tensors vs. Linalg on Buffers
- Linalg Tiling 和 Fusion API
- MLIR 的 tensor → memref 类型映射

**示例代码 — Conv2D Lowering**:

```cpp
struct ConvOpLowering : public OpConversionPattern<achigh::Conv2DOp> {
  using OpConversionPattern::OpConversionPattern;

  LogicalResult matchAndRewrite(achigh::Conv2DOp op, OpAdaptor adaptor,
      ConversionPatternRewriter &rewriter) const override {
    auto loc = op.getLoc();
    auto outputType = op.getType().cast<RankedTensorType>();

    // 创建零初始化的输出 tensor
    Value init = rewriter.create<tensor::EmptyOp>(
        loc, outputType.getShape(), outputType.getElementType());
    Value zero = rewriter.create<arith::ConstantOp>(
        loc, rewriter.getZeroAttr(outputType.getElementType()));
    Value initFilled = rewriter.create<linalg::FillOp>(loc, zero, init)
        .getResult(0);

    // 映射到 linalg.conv_2d_nchw_fchw
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

**降级后 MLIR 示意**:

```mlir
// ACHigh → Linalg Lowering 结果
%result = linalg.conv_2d_nchw_fchw
  ins(%input, %weight : tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>)
  outs(%init : tensor<1x64x224x224xf32>) -> tensor<1x64x224x224xf32>
```

**验收标准**:
- ✓ ACHigh → ACMid → Linalg 的完整降级链路可运行
- ✓ Conv2D、MatMul、激活函数等核心算子均已实现 Lowering
- ✓ Tiling Pass 能对 Linalg 操作进行分块
- ✓ 每层降级都有 FileCheck 测试

---

#### 2.2 MLIR Lowering — 从 Linalg 到 LLVM Dialect（Week 6）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 2.2.1 | Bufferization | 实现 Tensor → MemRef 的缓冲区化 | Bufferization 配置 |
| 2.2.2 | Linalg → Loops | 将 Linalg 操作展开为嵌套循环（SCF/Affine） | LinalgToLoopsPass |
| 2.2.3 | 循环优化 | 循环展开、循环交换、循环融合 | LoopOptimizationPass |
| 2.2.4 | 向量化 Lowering | 利用 Vector Dialect 实现 SIMD 向量化 | VectorizationPass |
| 2.2.5 | Affine → SCF → CF | 逐层 Lowering 到控制流方言 | AffineToSCF, SCFToCF Pass |
| 2.2.6 | CF → LLVM Dialect | 最终降级到 LLVM Dialect | ToLLVMDialectPass |
| 2.2.7 | 端到端降级测试 | 从 ACHigh 到 LLVM Dialect 的完整链路测试 | E2E 降级测试 |

**核心知识点**:
- MLIR Bufferization 框架（One-Shot Bufferization）
- Tensor 语义 vs. MemRef 语义的区别
- Affine Dialect 的分析能力（Dependence Analysis、Affine Scheduling）
- SCF（Structured Control Flow）和 CF（Control Flow）Dialect
- Vector Dialect 和 SIMD 向量化策略
- LLVM Dialect 的类型映射（memref → LLVM struct of pointers）
- MemRef 到 LLVM Dialect 的 descriptor 转换

**降级全链路示意**:

```
achigh.conv2d
  ↓ ConvertACHighToACMid
acmid.tiled_conv2d
  ↓ ConvertACMidToLinalg
linalg.conv_2d_nchw_fchw
  ↓ Bufferization (tensor → memref)
linalg.conv_2d_nchw_fchw (on memrefs)
  ↓ ConvertLinalgToLoops
scf.for / affine.for (嵌套 7 层循环)
  ↓ ConvertAffineToSCF
scf.for (纯 SCF 表示)
  ↓ ConvertSCFToCF
cf.br / cf.cond_br (基本块 + 控制流)
  ↓ ConvertToLLVM
llvm.br / llvm.load / llvm.store / llvm.fadd / llvm.fmul
```

**验收标准**:
- ✓ 完成从 ACHigh 到 LLVM Dialect 的完整降级链路
- ✓ 生成的 LLVM Dialect IR 能通过 mlir-translate 转为 LLVM IR
- ✓ 生成的 LLVM IR 能通过 `llc` 编译为目标代码
- ✓ Bufferization 正确处理 Tensor → MemRef 转换

---

#### 2.3 LLVM IR 优化 Pass（Week 7）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 2.3.1 | LLVM IR 基础学习 | 学习 LLVM IR 语法、SSA 形式、基本块结构 | 学习笔记及示例 |
| 2.3.2 | MLIR → LLVM IR 导出 | 从 LLVM Dialect 导出标准 LLVM IR | IR 导出模块 |
| 2.3.3 | 自定义 Function Pass | 编写函数级别的 LLVM 优化 Pass（AI 计算模式特化） | CustomLoopOptPass |
| 2.3.4 | 自定义 Vectorize Pass | 针对 AI 工作负载的向量化 Pass | CustomVectorizePass |
| 2.3.5 | LLVM 标准 Pass 集成 | 集成 LLVM 的标准优化 Pipeline（O0-O3） | 优化 Pipeline 配置 |
| 2.3.6 | 循环优化 | 基于 LLVM Loop Infrastructure 的循环展开、向量化 | Loop 优化 Pass |
| 2.3.7 | Pass Manager 集成 | 将自定义 Pass 注册到 LLVM New Pass Manager | PassManager 集成代码 |
| 2.3.8 | 优化效果分析 | 使用 opt-viewer / opt-remark 分析优化效果 | 优化分析报告 |

**核心知识点**:
- LLVM IR 的 SSA（Static Single Assignment）形式
- PHI 节点和基本块（Basic Block）结构
- LLVM 新 Pass Manager (NPM) 架构 — PassInfoMixin / PreservedAnalyses
- LLVM Analysis Pass — DominatorTree、LoopInfo、AliasAnalysis、ScalarEvolution
- LLVM Transform Pass — 循环展开、SLP 向量化、Loop 向量化
- LLVM Intrinsics 和 Target-Specific Intrinsics
- LLVM IR Linker 和 LTO（Link Time Optimization）
- Optimization Remarks 机制

**示例代码 — 自定义 LLVM IR Pass**:

```cpp
struct CustomLoopOptPass : public PassInfoMixin<CustomLoopOptPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    auto &LI = AM.getResult<LoopAnalysis>(F);
    for (auto *L : LI) {
      // 识别 AI 计算密集型循环
      // 应用激进展开、预取指令插入
    }
    return PreservedAnalyses::all();
  }
};
```

**示例 — LLVM IR 向量化效果**:

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
- ✓ 成功从 LLVM Dialect 导出标准 LLVM IR
- ✓ 自定义 LLVM Pass 注册到 New Pass Manager 并可运行
- ✓ 集成至少 10 个 LLVM 标准优化 Pass
- ✓ 循环向量化成功应用到卷积/矩阵乘法
- ✓ 优化后性能提升 30%+

---

#### 2.4 指令选择与 SIMD 映射（Week 8）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 2.4.1 | LLVM 后端架构学习 | 学习 SelectionDAG、指令选择、GlobalISel | 学习笔记 |
| 2.4.2 | SIMD 指令映射 | 利用 Vector Dialect/LLVM Intrinsics 映射 AVX/NEON | SIMD 映射代码 |
| 2.4.3 | 指令代价模型 | 实现 x86 指令延迟/吞吐量代价模型 | CostModel 模块 |
| 2.4.4 | FMA 指令利用 | 确保矩阵乘法使用 FMA（Fused Multiply-Add）指令 | FMA 优化 |
| 2.4.5 | 汇编分析 | 验证生成的汇编包含预期 SIMD 指令 | 汇编测试 |

**核心知识点**:
- LLVM SelectionDAG 和 GlobalISel 指令选择框架
- x86 SIMD 指令集层级（SSE → AVX → AVX2 → AVX-512）
- AArch64 SIMD 指令集（NEON → SVE/SVE2）
- Vector Dialect 到目标 SIMD 指令的映射
- FMA（Fused Multiply-Add）指令及其对数值精度的影响
- 代价模型在编译决策中的作用

**示例代码 — SIMD 向量化**:

```cpp
void generateAVX2MatMul(OpBuilder &builder, Value A, Value B, Value C) {
  auto f32x8Type = VectorType::get({8}, builder.getF32Type());
  // 加载 8 个 float (vmovups)
  auto aVec = builder.create<vector::LoadOp>(loc, f32x8Type, A, indices);
  auto bVec = builder.create<vector::BroadcastOp>(loc, f32x8Type, bScalar);
  // FMA: accumulator += a * b (vfmaddps)
  auto result = builder.create<vector::FMAOp>(loc, aVec, bVec, accum);
  builder.create<vector::StoreOp>(loc, result, C, indices);
}
```

**验收标准**:
- ✓ 卷积和矩阵乘法的核心循环使用 SIMD 指令
- ✓ 生成的汇编代码包含 AVX/AVX2 指令（x86）或 NEON 指令（AArch64）
- ✓ SIMD 优化带来 2-4x 性能提升

---

### Phase 3: 代码生成、Runtime 与端到端流水线（Week 9-12）

**阶段目标**: 实现完整的 CPU 后端代码生成，开发高性能 Runtime 库（im2col+GEMM），打通端到端编译-运行流程，建立性能基准

---

#### 3.1 CPU 后端 CodeGen（Week 9）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 3.1.1 | CodeGen 驱动实现 | 实现 LLVM IR → 目标代码的 CodeGen 驱动 | CodeGenDriver |
| 3.1.2 | TargetMachine 配置 | 配置 x86-64 / AArch64 的 TargetMachine | CPU 后端配置 |
| 3.1.3 | Object File 生成 | 生成 .o 目标文件 | emitObjectFile |
| 3.1.4 | Assembly 生成 | 生成 .s 汇编文件（用于分析） | emitAssembly |
| 3.1.5 | JIT 编译引擎 | 基于 LLVM ORC JIT 实现即时编译 | JITEngine |
| 3.1.6 | 端到端编译驱动 | 串联 Frontend→MLIR Opt→Lowering→LLVM Opt→CodeGen | CompilerDriver |

**核心知识点**:
- LLVM TargetMachine 和 DataLayout 配置
- MC Layer（Machine Code Layer）架构
- Object File 格式（ELF / Mach-O）
- LLVM ORC JIT 框架（LLJIT、ThreadSafeModule）
- 编译器 Driver 设计模式

**示例代码 — 编译器驱动**:

```cpp
class CompilerDriver {
public:
  LogicalResult compile(const std::string &inputFile,
                        const std::string &outputFile,
                        const CompilerOptions &options) {
    auto module = loadModel(inputFile);            // 1. Frontend
    PassManager pm(module.getContext());
    buildMLIRPipeline(pm, options);                // 2. MLIR 优化
    if (failed(pm.run(module))) return failure();
    if (failed(lowerToLLVMDialect(module)))        // 3. Lowering
      return failure();
    auto llvmModule = translateToLLVMIR(module);   // 4. LLVM IR
    optimizeLLVMModule(llvmModule, options.optLevel); // 5. LLVM 优化
    return emitObjectFile(llvmModule, outputFile); // 6. CodeGen
  }
};
```

**验收标准**:
- ✓ 能从 MLIR 生成可运行的 .o / .so 文件
- ✓ JIT 引擎能直接执行 MLIR/LLVM IR
- ✓ 编译驱动串联完整流水线

---

#### 3.2 Runtime 库实现（Week 10）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 3.2.1 | Runtime C API 设计 | 设计句柄式 C API（runtime/tensor/model） | ac_runtime.h |
| 3.2.2 | 内存管理 | 实现 64 字节对齐的内存池 | MemoryPool |
| 3.2.3 | Naive Kernels | 实现全部算子的 naive 参考实现 | cpu_kernels.cpp (naive) |
| 3.2.4 | im2col + GEMM 卷积 | 实现高性能 Conv2D（im2col 矩阵展开 + GEMM） | conv2d_im2col 实现 |
| 3.2.5 | Tiled MatMul | 实现 cache 友好的分块矩阵乘法 | matmul_tiled 实现 |
| 3.2.6 | 多线程支持 | 集成 OpenMP 实现并行计算 | 多线程 Runtime |
| 3.2.7 | 模型加载/执行 | 实现编译产物的加载和推理执行 | ac_load_model / ac_execute |
| 3.2.8 | Runtime 测试 | Kernel 正确性和 Runtime API 测试 | 单元测试 |

**核心知识点**:
- C API 设计原则（句柄模式、错误处理、ABI 稳定性）
- 内存对齐策略（64 字节 = AVX-512 cache line 友好）
- im2col 算法原理 — 将卷积转换为矩阵乘法
- GEMM 优化策略（分块、向量化、预取）
- OpenMP 并行编程模型
- 内存池设计与 Buffer 复用

**示例代码 — im2col + GEMM 卷积**:

```cpp
void ac_cpu_conv2d_im2col_f32(
    const float *input, const float *weight, const float *bias,
    float *output,
    int64_t N, int64_t C, int64_t H, int64_t W,
    int64_t K, int64_t R, int64_t S,
    int64_t stride, int64_t padding) {
  int64_t H_out = (H + 2*padding - R) / stride + 1;
  int64_t W_out = (W + 2*padding - S) / stride + 1;

  // 1. im2col: 将输入展开为矩阵 [N*H'*W', C*R*S]
  std::vector<float> col(N * H_out * W_out * C * R * S);
  ac_cpu_im2col_f32(input, col.data(), N, C, H, W, R, S,
                     stride, stride, padding, padding, H_out, W_out);

  // 2. GEMM: output = col @ weight^T → [N*H'*W', K]
  ac_cpu_matmul_f32(col.data(), weight, output,
                     N * H_out * W_out, K, C * R * S);

  // 3. Add bias
  if (bias) { /* ... */ }
}
```

**示例代码 — Runtime C API**:

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

**验收标准**:
- ✓ Runtime API 完整且文档齐全
- ✓ 所有 CPU Kernel 通过正确性测试
- ✓ im2col+GEMM 卷积性能优于 naive 实现 5x+
- ✓ 内存池管理正常，无内存泄漏
- ✓ 多线程执行正常

---

#### 3.3 端到端执行与性能优化（Week 11-12）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 3.3.1 | 端到端 MatMul 测试 | 从 DSL/MLIR 到执行的矩阵乘法完整流程 | E2E MatMul Demo |
| 3.3.2 | 端到端 Conv 测试 | 从 MLIR 到执行的卷积完整流程 | E2E Conv Demo |
| 3.3.3 | ResNet-18 推理 | ONNX 导入 → 编译 → 推理的完整流程 | ResNet-18 Demo |
| 3.3.4 | 性能基准框架 | 搭建 Benchmark 框架（统计分析、GFLOPS 计算） | benchmark.h 框架 |
| 3.3.5 | 单算子 Benchmark | Conv2D、MatMul 的多尺寸性能测试 | 算子性能数据 |
| 3.3.6 | 模型 Benchmark | ResNet-18、MobileNet 的端到端性能测试 | 模型性能数据 |
| 3.3.7 | 性能 Profiling | 使用 perf/VTune 定位性能瓶颈 | 性能分析报告 |
| 3.3.8 | 瓶颈优化 | 针对 Profiling 结果进行至少 3 轮优化 | 优化后的代码 |

**核心知识点**:
- 端到端编译流水线的集成测试策略
- 性能 Benchmark 方法论（warm-up、统计分析、GFLOPS 计算）
- Linux perf / Intel VTune 性能分析工具
- Cache 行为分析（cache miss rate、L1/L2/L3 命中率）
- 性能优化的迭代方法（Profile → Analyze → Optimize → Verify）

**性能目标**:

| 模型 / 算子 | 输入规模 | ACC 目标 | ONNX Runtime 参考 | 相对性能 |
|-------------|---------|---------------|-------------------|---------|
| Conv2D | 1x64x56x56, k=3x3 | ≤ 2.5 ms | ~1.5 ms | 60%+ |
| MatMul | 1024x1024x1024 | ≤ 15 ms | ~10 ms | 65%+ |
| ResNet-18 | 1x3x224x224 | ≤ 30 ms | ~20 ms | 65%+ |
| MobileNetV2 | 1x3x224x224 | ≤ 20 ms | ~15 ms | 75%+ |

**验收标准**:
- ✓ ResNet-18 端到端推理成功，结果与 ONNX Runtime 数值一致（rtol=1e-3）
- ✓ Conv2D 单算子性能达到目标（60%+ ONNX Runtime）
- ✓ 建立完整的性能基准数据
- ✓ 识别并优化至少 3 个性能瓶颈
- ✓ 完成性能分析报告

---

### Phase 4: 高级特性与多后端扩展（Week 13+）

**阶段目标**: 实现高级编译器特性（量化、AutoTuning、并行化），为 GPU/NPU 后端搭建扩展框架

---

#### 4.1 高级编译器特性（Week 13-14）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 4.1.1 | 内存优化 Pass | 内存分配策略、Buffer 复用优化 | MemoryOptPass |
| 4.1.2 | 并行化 Pass | 多线程并行执行 Pass（循环并行化） | ParallelizationPass |
| 4.1.3 | 量化支持 | INT8/FP16 量化编译支持 | QuantizationPass |
| 4.1.4 | AutoTuning 框架 | 自动调优框架（Tiling Size、Unroll Factor 等） | AutoTuner 模块 |
| 4.1.5 | 性能 Profiling 工具 | 编译器内置的性能分析和瓶颈定位 | Profiler 工具 |

**核心知识点**:
- 编译器内存优化策略（Liveness Analysis、Buffer Reuse）
- 量化编译原理（对称/非对称量化、Scale/ZeroPoint 传播）
- AutoTuning 搜索策略（Grid Search、Bayesian Optimization）
- 多线程并行化（Loop Parallelization、Task Parallelism）

---

#### 4.2 [TODO] NVIDIA GPU CUDA 后端（Week 15+）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 4.2.1 | ACGPU Dialect 定义 | 定义 GPU 特有概念（Grid/Block/Thread/SharedMem） | ACGPU Dialect |
| 4.2.2 | ACMid → ACGPU Lowering | 实现 Tile 层到 GPU Kernel 的降级 | ACMidToGPU Pass |
| 4.2.3 | GPU → NVVM Lowering | 通过 MLIR GPU/NVVM Dialect 生成 PTX | GPUToNVVM Pass |
| 4.2.4 | Kernel 优化 | Shared Memory、Tensor Core、Bank Conflict 优化 | 优化后的 Kernel |
| 4.2.5 | CUDA Runtime 集成 | 实现 CUDA Runtime 支持库 | runtime/cuda/ |

**核心知识点**:
- GPU 编程模型（Grid → Block → Thread 层次）
- Shared Memory 优化策略
- Warp-level 原语（Shuffle、Reduce）
- Tensor Core 映射（WMMA/MMA 指令）
- LLVM NVPTX 后端
- CUDA Driver API

**技术路线**:

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

#### 4.3 [TODO] 华为 Ascend NPU 后端（Week 15+）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 4.3.1 | ACAscend Dialect 定义 | 定义 NPU 特有概念（AI Core/Vector Core/Cube Unit） | ACAscend Dialect |
| 4.3.2 | ACMid → ACAscend Lowering | 实现到 Ascend 方言的降级 | MidToAscend Pass |
| 4.3.3 | Ascend IR 生成 | 生成 Ascend IR（CCE-C 或 TBE 算子描述） | AscendIR CodeGen |
| 4.3.4 | CANN 工具链对接 | 对接华为 CANN 编译工具链 | CANN 集成 |
| 4.3.5 | Ascend Runtime 集成 | 实现 ACL Runtime 支持库 | runtime/ascend/ |

**核心知识点**:
- 昇腾 AI 处理器架构（AI Core、Vector Core、Cube Unit）
- Ascend C 编程模型
- TBE（Tensor Boost Engine）算子开发
- CANN（Compute Architecture for Neural Networks）工具链
- 双 Buffer 乒乓优化、AI Core 间并行调度

**技术路线**:

```
ACMid Dialect
  ↓ (ACMidToAscend)
ACAscend Dialect
  ↓ (Ascend Codegen)
Ascend IR (AIR) → OM Model
  ↓ (ACL Runtime)
NPU 执行
```

---

## 三、性能评估标准

### 3.1 编译时性能

| 指标 | 目标值 | 测量方法 |
|-----|-------|---------|
| 小模型编译时间 | < 5s | ResNet-18 |
| 大模型编译时间 | < 30s | ResNet-50 |
| 单 Pass 执行时间 | < 1s | 任意单个 Pass |
| 编译内存峰值 | < 2GB | 编译过程监控 |

### 3.2 运行时性能

| 模型 | 输入规模 | ACC | ONNX Runtime | 相对性能 |
|------|---------|-----------|--------------|---------|
| ResNet-18 | 1x3x224x224 | ≤ 30ms | ~20ms | 65% |
| ResNet-50 | 1x3x224x224 | ≤ 95ms | ~60ms | 63% |
| MobileNetV2 | 1x3x224x224 | ≤ 20ms | ~15ms | 75% |
| BERT-Base | seq=128 | ≤ 150ms | ~100ms | 65% |

### 3.3 代码质量

| 指标 | 目标值 |
|-----|-------|
| 测试覆盖率 | > 80% |
| 文档覆盖率 | 100%（所有公共 API） |
| 编译警告 | 0 |
| Clang-tidy 警告 | < 10 |

---

## 四、关键技术栈

| 技术领域 | 技术/工具 | 用途 |
|----------|-----------|------|
| 编译基础设施 | LLVM 17+ | IR 优化、代码生成 |
| 多层级 IR | MLIR | 自定义方言、图优化、多层 Lowering |
| 构建系统 | CMake + Ninja | 项目构建 |
| 代码生成 | TableGen | 方言定义、算子定义、Pass 注册 |
| 测试框架 | lit + FileCheck | MLIR/LLVM IR 回归测试 |
| 单元测试 | GoogleTest | C++ 单元测试 |
| 性能测试 | 自研 Benchmark 框架 | 算子和模型性能基准 |
| 模型格式 | ONNX (protobuf) | AI 模型导入 |
| 编程语言 | C++17 | 主要开发语言 |
| 文档 | Doxygen + Markdown | 代码文档和学习笔记 |
| 版本管理 | Git + GitHub | 代码版本管理和 CI/CD |

---

## 五、里程碑定义

| 里程碑 | 时间 | 目标 | 验收标准 |
|--------|------|------|----------|
| **M1** | Week 1 末 | 环境就绪 | LLVM/MLIR 编译成功，项目骨架搭建完成，CI 正常 |
| **M2** | Week 2 末 | 方言完成 | ACHigh 20+ 算子定义完成，verify() 全部实现 |
| **M3** | Week 3 末 | Pass 完成 | 5 个优化 Pass 实现，FileCheck 测试全部通过 |
| **M4** | Week 4 末 | 前端完成 | ONNX 20+ 算子导入可用，DSL 基础解析可用 |
| **M5** | Week 6 末 | Lowering 完成 | ACHigh → LLVM Dialect 完整降级链路打通 |
| **M6** | Week 8 末 | LLVM 优化完成 | 自定义 Pass + 标准 Pipeline 可运行，向量化生效 |
| **M7** | Week 10 末 | Runtime 完成 | im2col+GEMM 卷积实现，Runtime API 可用 |
| **M8** | Week 12 末 | 端到端达标 | ResNet-18 推理成功，性能 60%+ ONNX Runtime |

---

## 六、测试策略

### 6.1 三层测试体系

| 层级 | 框架 | 用途 | 位置 |
|------|------|------|------|
| **单元测试** | GoogleTest | C++ 模块正确性 | unittests/ |
| **集成测试** | lit + FileCheck | MLIR/LLVM IR 变换正确性 | test/ |
| **性能测试** | 自研 Benchmark | 算子和模型性能 | test/Performance/ |
| **端到端测试** | Python + ONNX Runtime | 数值精度对比 | test/E2E/ |

### 6.2 测试覆盖要求

- **Phase 1**: 测试覆盖率 > 70%
- **Phase 2**: 测试覆盖率 > 75%
- **Phase 3**: 测试覆盖率 > 80%
- 每个新增 Pass 必须有对应 FileCheck 测试
- 每个新增 Kernel 必须有正确性单元测试

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

### 在线资源

1. [MLIR 官方文档](https://mlir.llvm.org/) / [MLIR Toy Tutorial](https://mlir.llvm.org/docs/Tutorials/Toy/)
2. [LLVM Language Reference](https://llvm.org/docs/LangRef.html) / [Writing an LLVM Pass](https://llvm.org/docs/WritingAnLLVMNewPMPass.html)
3. Stanford CS143 / MIT 6.172 / Cornell CS6120

### 开源项目参考

1. [IREE](https://github.com/iree-org/iree) — Google 的 MLIR AI 编译器
2. [Torch-MLIR](https://github.com/llvm/torch-mlir) — PyTorch → MLIR
3. [ONNX-MLIR](https://github.com/onnx/onnx-mlir) — ONNX → MLIR
4. [TVM](https://github.com/apache/tvm) — Apache AI 编译器

---

---

### Phase 5: AI 框架对接 — Triton / PyTorch 集成（Week 15+）

**阶段目标**: 将 ACC 对接到主流 AI 框架生态，实现 PyTorch → Triton DSL → ACC → 硬件后端的端到端编译和验证流程

---

#### 5.1 OpenAI Triton 前端对接（Week 15-16）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 5.1.1 | Triton IR 分析 | 分析 Triton IR（TTIR / TTGIR）的结构和语义 | 分析文档 |
| 5.1.2 | Triton Dialect 定义 | 在 ACC 中定义 Triton 兼容方言，表达 Triton 核心操作 | ACTriton Dialect (.td) |
| 5.1.3 | Triton IR → ACC MLIR | 实现 Triton IR 到 ACC MLIR（ACHigh/ACMid）的导入转换 | TritonImporter |
| 5.1.4 | Triton 算子映射 | 映射 tl.load/tl.store/tl.dot/tl.reduce 等到 ACHigh 算子 | 算子映射表 |
| 5.1.5 | Triton 语义保持 | 确保 Block Program 语义、掩码（mask）等 Triton 特性正确转换 | 语义验证测试 |
| 5.1.6 | Triton → ACC → CPU | 端到端：Triton kernel → ACC 编译 → CPU 执行 | E2E Demo |

**核心知识点**:
- OpenAI Triton 的编程模型（Block-level Programming）
- Triton IR 层级结构：Triton Dialect (TTIR) → TritonGPU Dialect (TTGIR)
- Triton 的 tl.load / tl.store / tl.dot / tl.reduce 等核心原语
- Triton 的 Block Pointer 和 Mask 机制
- Triton IR 到 Linalg/SCF 的语义映射
- MLIR Dialect 间的 Import/Conversion 策略

**技术路线**:

```
Triton Python DSL (@triton.jit)
    │
    ▼ (Triton Compiler Frontend)
Triton IR (TTIR)        ← Triton 的 MLIR Dialect
    │
    ▼ (TritonImporter)
ACC MLIR (ACHigh/ACMid)  ← ACC 的多层 IR
    │
    ▼ (ACC Optimization + Lowering Pipeline)
LLVM IR → CodeGen
    │
    ├──→ CPU (x86/ARM)       [Phase 1-3 已实现]
    ├──→ NVIDIA GPU (PTX)    [TODO Phase 4]
    └──→ Ascend NPU          [TODO Phase 4]
```

**示例代码 — Triton Kernel 定义**:

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

**示例代码 — Triton IR 到 ACC MLIR 映射**:

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

**验收标准**:
- ✓ 成功解析 Triton matmul kernel 的 IR
- ✓ 将 tl.load/tl.store/tl.dot 映射到 ACC MLIR 算子
- ✓ Triton kernel → ACC → CPU 的端到端执行正确
- ✓ 数值结果与 Triton 原生执行一致（rtol=1e-3）

---

#### 5.2 PyTorch 框架对接（Week 17-18）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 5.2.1 | PyTorch 导出分析 | 分析 torch.export / torch.compile 的 IR 导出格式 | 分析文档 |
| 5.2.2 | Torch-MLIR 对接 | 通过 Torch-MLIR 将 PyTorch 模型转换到 ACC MLIR | TorchImporter |
| 5.2.3 | ONNX 通路增强 | 增强 PyTorch → ONNX → ACC 的转换通路 | ONNX 导入增强 |
| 5.2.4 | 自定义算子注册 | 支持 PyTorch 调用 ACC 编译的自定义算子 | torch.library 注册 |
| 5.2.5 | Triton 算子集成 | PyTorch 中使用 Triton 编写算子，由 ACC 编译 | Triton 算子 ACC 编译 |
| 5.2.6 | Python Binding | 提供 ACC 的 Python 绑定（pybind11） | acc Python 模块 |

**核心知识点**:
- PyTorch 2.0 编译栈（torch.compile / TorchDynamo / TorchInductor）
- torch.export 和 ExportedProgram 格式
- Torch-MLIR 项目架构和使用方式
- PyTorch 自定义算子注册（torch.library）
- pybind11 C++/Python 绑定
- PyTorch 与自定义编译器后端的集成方式

**技术路线**:

```
PyTorch Model (nn.Module)
    │
    ├──→ torch.export → FX Graph → Torch-MLIR → ACC MLIR  [通路1: 整图编译]
    │
    ├──→ torch.onnx.export → ONNX → ACC MLIR              [通路2: ONNX转换]
    │
    └──→ @triton.jit (Triton Kernel)                       [通路3: 算子级]
              │
              ▼ (TritonImporter)
         ACC MLIR → Lowering → CodeGen → Runtime
              │
              ▼
         PyTorch custom op (torch.library)
```

**示例代码 — PyTorch + Triton + ACC 端到端**:

```python
import torch
import triton
import triton.language as tl
import acc  # ACC Python binding

# ============================================================
# Step 1: 定义 PyTorch 模型
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
# Step 2: 定义 Triton 自定义算子
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
# Step 3: 使用 ACC 编译 Triton kernel
# ============================================================
compiler = acc.Compiler()
compiled_kernel = compiler.compile_triton(
    fused_linear_relu_kernel,
    target="cpu",
    opt_level=3,
    block_config={"BLOCK_M": 32, "BLOCK_N": 32, "BLOCK_K": 32}
)

# ============================================================
# Step 4: 在 PyTorch 中注册并使用 ACC 编译的算子
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
# Step 5: 端到端验证
# ============================================================
model = SimpleNet()
x = torch.randn(32, 784)

# 原始 PyTorch 执行
y_ref = model(x)

# ACC 编译执行
y_acc = fused_linear_relu(x, model.linear1.weight.T,
                           model.linear1.bias)

# 数值对比
print(f"Max diff: {(y_ref - y_acc).abs().max().item():.6f}")
assert torch.allclose(y_ref, y_acc, rtol=1e-3, atol=1e-5)
print("✓ End-to-end verification passed!")
```

**验收标准**:
- ✓ PyTorch 模型通过 ONNX 通路成功导入 ACC
- ✓ Triton kernel 成功编译并在 PyTorch 中调用
- ✓ 端到端数值精度验证通过
- ✓ Python 绑定可用

---

#### 5.3 端到端验证流程（Week 19-20）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 5.3.1 | E2E 测试框架 | 搭建 PyTorch↔ACC 的自动化端到端测试框架 | E2E 测试框架 |
| 5.3.2 | Triton→ACC→CPU | Triton matmul/softmax kernel 的 ACC CPU 编译验证 | Triton E2E 测试 |
| 5.3.3 | PyTorch→ACC | PyTorch 小模型（MLP/CNN）的完整编译验证 | PyTorch E2E 测试 |
| 5.3.4 | 精度对比 | 与 PyTorch eager / Triton GPU 的数值精度对比 | 精度报告 |
| 5.3.5 | 性能对比 | 与 PyTorch eager / TorchInductor 的性能对比 | 性能报告 |

**端到端验证流程全景**:

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
│  │  算子融合 / 常量折叠 / 形状推导 / 布局变换              │    │
│  ├──────────────────────────────────────────────────────┤    │
│  │  ACMid Dialect (Tile/Loop Level)                      │    │
│  │  Tiling / 循环优化 / 向量化                            │    │
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
│ x86/ARM  │           CUDA/PTX           │    Ascend IR      │
│ [实现]    │           [TODO]             │    [TODO]          │
└──────────┴────────────────┴──────────────┴────────────────────┘
```

**验收标准**:
- ✓ PyTorch MLP 模型端到端编译和推理成功
- ✓ Triton matmul/softmax kernel 通过 ACC 编译并执行正确
- ✓ 所有 E2E 测试精度验证通过
- ✓ 生成完整的精度和性能对比报告

---

### Phase 6: 高阶特性（Week 20+）

**阶段目标**: 实现面向生产环境的高阶编译器特性，提升 ACC 作为 AI Compiler Core 的核心竞争力

---

#### 6.1 高阶特性规划

| 编号 | 特性 | 说明 | 优先级 |
|------|------|------|--------|
| 6.1.1 | **Auto-Scheduling** | 基于代价模型的自动调度策略搜索（类似 TVM AutoScheduler） | 高 |
| 6.1.2 | **Polyhedral 优化** | 基于多面体模型的循环变换（Affine Scheduling） | 高 |
| 6.1.3 | **动态形状支持** | 支持动态 Batch Size 和可变序列长度的编译 | 高 |
| 6.1.4 | **混合精度编译** | FP16/BF16/INT8 混合精度自动转换和优化 | 高 |
| 6.1.5 | **图分割与子图编译** | 大模型图的智能分割和子图独立编译 | 中 |
| 6.1.6 | **Kernel 融合引擎** | 跨算子边界的激进 Kernel 融合（类似 XLA/TorchInductor） | 中 |
| 6.1.7 | **内存规划优化** | 编译期全局内存规划，最小化峰值内存占用 | 中 |
| 6.1.8 | **异构调度** | CPU + GPU 异构计算的自动任务分配 | 低 |
| 6.1.9 | **分布式编译** | 多设备/多节点的分布式模型编译支持 | 低 |
| 6.1.10 | **Debug/Profile 集成** | 编译器级别的调试和性能分析工具 | 中 |

#### 6.2 Auto-Scheduling 框架

```
搜索空间定义 → 代价模型评估 → 候选方案生成 → 编译+测量 → 最优方案
                    ↑                                      │
                    └──────────── 反馈学习 ─────────────────┘
```

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 6.2.1 | 搜索空间定义 | 定义 Tiling/Unroll/Vectorize 等可调参数空间 | SearchSpace 模块 |
| 6.2.2 | 分析型代价模型 | 基于硬件参数的静态代价估算 | AnalyticalCostModel |
| 6.2.3 | ML 代价模型 | 基于历史数据训练的机器学习代价模型 | MLCostModel |
| 6.2.4 | 搜索算法 | Grid Search / Random Search / Bayesian Optimization | Searcher 模块 |
| 6.2.5 | 自动调优集成 | 将搜索结果集成到 Pass Pipeline 中 | AutoTuner 集成 |

#### 6.3 动态形状编译

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 6.3.1 | 符号形状表示 | 在 IR 中支持符号化的动态维度 | Symbolic Shape IR |
| 6.3.2 | Shape Guard 生成 | 生成运行时形状检查代码 | ShapeGuard Pass |
| 6.3.3 | 特化 + 回退 | 对常见形状特化编译，动态形状回退到通用版本 | Specialization Pass |

---

## 八、贡献指南

- 遵循 [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- 使用 `clang-format` 格式化代码
- 所有公共 API 都需要文档注释
- 每个 PR 都需要对应的测试
- 运行 `ninja check-acc` 确保所有测试通过

---

## 九、变更日志

### v0.4.0 (2026-02-16)

- 项目重命名: ACompiler → ACC (AI Compiler Core)
- 新增 Phase 5: Triton/PyTorch 框架对接，端到端验证流程
- 新增 Phase 6: 高阶特性规划（Auto-Scheduling、动态形状、混合精度等）
- 新增 Triton 前端导入层（TritonImporter）
- 新增 PyTorch 对接层（TorchImporter）
- 新增 PyTorch + Triton + ACC 端到端验证示例

### v0.3.0 (2026-02-16)

- 重构项目规划: 以任务表格+核心知识点+代码示例+验收标准格式重写全部阶段
- 项目定位升级: 从纯教育项目升级为 AI Compiler Core 项目

### v0.2.0 (2026-02-16)

- 综合两版规划，采用 4 阶段结构
- 增加代码示例和量化验收标准

### v0.1.0 (2026-02-16)

- 初始项目结构和 8 阶段规划

---

**最后更新**: 2026-02-16
**文档版本**: 4.0
**维护者**: ACC Team
