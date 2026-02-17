# ACompiler 实施指南

本文档提供ACompiler项目的详细实施步骤，每个阶段都包含具体的任务分解、代码示例和验收标准。

---

## 📌 使用说明

本指南按照PROJECT_PLAN.md的四个阶段组织，每个阶段都有：
- ✅ **任务清单**: 具体的待办事项
- 📝 **实施步骤**: 详细的操作指导
- 💻 **代码模板**: 可直接使用的代码示例
- ✔️ **验收标准**: 完成的评判标准
- 🐛 **常见问题**: 可能遇到的问题及解决方案

---

## 第一阶段：基础架构与MLIR入门

### Week 1: 项目工程搭建

#### Task 1.1: 安装LLVM/MLIR

**步骤**:

```bash
# 1. 克隆LLVM项目
cd /workspace/third_party
git clone --depth 1 --branch llvmorg-17.0.6 https://github.com/llvm/llvm-project.git

# 2. 创建构建目录
cd llvm-project
mkdir build && cd build

# 3. 配置CMake
cmake -G Ninja ../llvm \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;ARM;AArch64" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_INSTALL_UTILS=ON \
  -DCMAKE_INSTALL_PREFIX=/workspace/third_party/llvm-install

# 4. 编译（需要30-60分钟）
ninja -j $(nproc)

# 5. 安装
ninja install

# 6. 验证安装
/workspace/third_party/llvm-install/bin/mlir-opt --version
```

**预期输出**:
```
MLIR (https://mlir.llvm.org):
  MLIR version 17.0.6
  Optimized build.
```

#### Task 1.2: 配置ACompiler构建系统

**更新CMakeLists.txt**:

```cmake
# 确保CMakeLists.txt包含正确的LLVM路径查找
find_package(MLIR REQUIRED CONFIG)

if(NOT MLIR_FOUND)
  message(FATAL_ERROR "MLIR not found. Please set MLIR_DIR")
endif()

message(STATUS "MLIR found at: ${MLIR_DIR}")
message(STATUS "LLVM found at: ${LLVM_DIR}")
```

**构建ACompiler**:

```bash
cd /workspace
mkdir build && cd build

# 配置
cmake -G Ninja .. \
  -DLLVM_DIR=/workspace/third_party/llvm-install/lib/cmake/llvm \
  -DMLIR_DIR=/workspace/third_party/llvm-install/lib/cmake/mlir \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 编译
ninja

# 验证
./bin/acompiler-opt --help
```

#### Task 1.3: 集成测试框架

**安装GoogleTest**:

```cmake
# CMakeLists.txt中添加
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
```

**创建测试示例**:

```cpp
// test/Dialect/AI/BasicTest.cpp
#include "acompiler/Dialect/AI/AIDialect.h"
#include <gtest/gtest.h>

using namespace mlir;
using namespace mlir::ai;

class AIDialectTest : public ::testing::Test {
protected:
  void SetUp() override {
    context.loadDialect<AIDialect>();
  }
  
  MLIRContext context;
};

TEST_F(AIDialectTest, DialectRegistration) {
  auto *dialect = context.getLoadedDialect<AIDialect>();
  ASSERT_NE(dialect, nullptr);
  EXPECT_EQ(dialect->getNamespace(), "ai");
}
```

**运行测试**:

```bash
ninja check-acompiler
```

✅ **验收标准**:
- [ ] LLVM/MLIR成功安装
- [ ] ACompiler项目可编译
- [ ] `ninja check-acompiler`所有测试通过
- [ ] 生成`compile_commands.json`（用于IDE）

---

### Week 2: MLIR Dialect开发

#### Task 2.1: 扩展AI Dialect算子

**添加更多算子到AIOps.td**:

```tablegen
//===----------------------------------------------------------------------===//
// Pooling Operations
//===----------------------------------------------------------------------===//

def AI_AvgPool2DOp : AI_Op<"avgpool2d", [Pure]> {
  let summary = "2D average pooling operation";
  
  let arguments = (ins
    AnyRankedTensor:$input,
    I64ArrayAttr:$kernel_size,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$stride,
    DefaultValuedAttr<I64ArrayAttr, "{0, 0}">:$padding
  );
  
  let results = (outs AnyRankedTensor:$output);
  
  let assemblyFormat = [{
    $input attr-dict `:` type($input) `->` type($output)
  }];
}

def AI_GlobalAvgPoolOp : AI_Op<"global_avgpool", [Pure]> {
  let summary = "Global average pooling";
  let description = [{
    对空间维度进行全局平均池化: output[n,c] = mean(input[n,c,h,w])
  }];
  
  let arguments = (ins AnyRankedTensor:$input);
  let results = (outs AnyRankedTensor:$output);
  
  let assemblyFormat = "$input attr-dict `:` type($input) `->` type($output)";
}

//===----------------------------------------------------------------------===//
// Element-wise Operations
//===----------------------------------------------------------------------===//

def AI_AddOp : AI_Op<"add", [Pure, Commutative, SameOperandsAndResultType]> {
  let summary = "Element-wise addition";
  
  let arguments = (ins AnyTensor:$lhs, AnyTensor:$rhs);
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $lhs `,` $rhs attr-dict `:` type($lhs)
  }];
  
  let hasFolder = 1;
}

def AI_MulOp : AI_Op<"mul", [Pure, Commutative, SameOperandsAndResultType]> {
  let summary = "Element-wise multiplication";
  
  let arguments = (ins AnyTensor:$lhs, AnyTensor:$rhs);
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $lhs `,` $rhs attr-dict `:` type($lhs)
  }];
  
  let hasFolder = 1;
}

//===----------------------------------------------------------------------===//
// Normalization Operations
//===----------------------------------------------------------------------===//

def AI_LayerNormOp : AI_Op<"layer_norm", [Pure]> {
  let summary = "Layer normalization";
  
  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$scale,
    AnyRankedTensor:$bias,
    DefaultValuedAttr<F32Attr, "1e-5">:$epsilon,
    I64ArrayAttr:$normalized_shape
  );
  
  let results = (outs AnyRankedTensor:$output);
}

//===----------------------------------------------------------------------===//
// Reshape Operations
//===----------------------------------------------------------------------===//

def AI_ReshapeOp : AI_Op<"reshape", [Pure]> {
  let summary = "Reshape tensor";
  
  let arguments = (ins
    AnyTensor:$input,
    I64ArrayAttr:$shape
  );
  
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $input attr-dict `:` type($input) `->` type($output)
  }];
}

def AI_TransposeOp : AI_Op<"transpose", [Pure]> {
  let summary = "Transpose tensor dimensions";
  
  let arguments = (ins
    AnyRankedTensor:$input,
    I64ArrayAttr:$perm
  );
  
  let results = (outs AnyRankedTensor:$output);
  
  let assemblyFormat = [{
    $input attr-dict `:` type($input) `->` type($output)
  }];
}
```

**实现验证逻辑**:

```cpp
// lib/Dialect/AI/AIOps.cpp

LogicalResult AvgPool2DOp::verify() {
  auto inputType = getInput().getType().cast<RankedTensorType>();
  
  if (inputType.getRank() != 4) {
    return emitOpError("input must be 4D tensor (NCHW)");
  }
  
  auto kernelSize = getKernelSize();
  if (kernelSize.size() != 2) {
    return emitOpError("kernel_size must have 2 elements");
  }
  
  return success();
}

LogicalResult TransposeOp::verify() {
  auto inputType = getInput().getType().cast<RankedTensorType>();
  auto perm = getPerm();
  
  if (perm.size() != inputType.getRank()) {
    return emitOpError("perm size must match input rank");
  }
  
  // 检查perm是0到rank-1的排列
  SmallVector<bool> seen(perm.size(), false);
  for (auto p : perm) {
    int64_t idx = p.cast<IntegerAttr>().getInt();
    if (idx < 0 || idx >= perm.size() || seen[idx]) {
      return emitOpError("invalid permutation");
    }
    seen[idx] = true;
  }
  
  return success();
}

// 常量折叠示例
OpFoldResult AddOp::fold(ArrayRef<Attribute> operands) {
  // 如果两个操作数都是常量，在编译时计算
  if (operands[0] && operands[1]) {
    auto lhs = operands[0].dyn_cast<DenseElementsAttr>();
    auto rhs = operands[1].dyn_cast<DenseElementsAttr>();
    
    if (lhs && rhs && lhs.isSplat() && rhs.isSplat()) {
      // 常量加法
      auto lhsVal = lhs.getSplatValue<FloatAttr>().getValueAsDouble();
      auto rhsVal = rhs.getSplatValue<FloatAttr>().getValueAsDouble();
      auto result = lhsVal + rhsVal;
      
      return DenseElementsAttr::get(
          getType().cast<TensorType>(),
          builder.getF32FloatAttr(result));
    }
  }
  
  // 0 + x = x
  if (operands[0] && isZero(operands[0])) {
    return getRhs();
  }
  
  // x + 0 = x
  if (operands[1] && isZero(operands[1])) {
    return getLhs();
  }
  
  return {};
}
```

**编写测试**:

```mlir
// test/Dialect/AI/ops.mlir

// RUN: acompiler-opt %s | FileCheck %s

module {
  // CHECK-LABEL: @test_avgpool
  func.func @test_avgpool(%arg0: tensor<1x64x56x56xf32>) -> tensor<1x64x28x28xf32> {
    // CHECK: ai.avgpool2d
    %0 = ai.avgpool2d %arg0 {
      kernel_size = [2, 2],
      stride = [2, 2]
    } : tensor<1x64x56x56xf32> -> tensor<1x64x28x28xf32>
    return %0 : tensor<1x64x28x28xf32>
  }
  
  // CHECK-LABEL: @test_add_fold
  func.func @test_add_fold() -> tensor<f32> {
    %c1 = arith.constant dense<1.0> : tensor<f32>
    %c2 = arith.constant dense<2.0> : tensor<f32>
    // CHECK: arith.constant dense<3.0>
    // CHECK-NOT: ai.add
    %sum = ai.add %c1, %c2 : tensor<f32>
    return %sum : tensor<f32>
  }
}
```

✅ **验收标准**:
- [ ] 添加至少15个AI算子
- [ ] 所有算子有完整的verify()实现
- [ ] AddOp/MulOp的常量折叠工作
- [ ] 测试覆盖率>70%

#### Task 2.2: 实现形状推导

**为Conv2D实现InferTypeOpInterface**:

```cpp
// lib/Dialect/AI/AIOps.cpp

LogicalResult Conv2DOp::inferReturnTypes(
    MLIRContext *context,
    Optional<Location> location,
    ValueRange operands,
    DictionaryAttr attributes,
    RegionRange regions,
    SmallVectorImpl<Type> &inferredReturnTypes) {
  
  // 获取输入类型
  auto inputType = operands[0].getType().dyn_cast<RankedTensorType>();
  auto weightType = operands[1].getType().dyn_cast<RankedTensorType>();
  
  if (!inputType || !weightType) {
    return failure();
  }
  
  // 提取形状
  auto inputShape = inputType.getShape();   // [N, C, H, W]
  auto weightShape = weightType.getShape(); // [K, C, R, S]
  
  // 提取属性
  auto stride = attributes.getAs<ArrayAttr>("stride");
  auto padding = attributes.getAs<ArrayAttr>("padding");
  auto dilation = attributes.getAs<ArrayAttr>("dilation");
  
  // 默认值
  int64_t strideH = 1, strideW = 1;
  int64_t paddingH = 0, paddingW = 0;
  int64_t dilationH = 1, dilationW = 1;
  
  if (stride && stride.size() == 2) {
    strideH = stride[0].cast<IntegerAttr>().getInt();
    strideW = stride[1].cast<IntegerAttr>().getInt();
  }
  
  if (padding && padding.size() == 2) {
    paddingH = padding[0].cast<IntegerAttr>().getInt();
    paddingW = padding[1].cast<IntegerAttr>().getInt();
  }
  
  if (dilation && dilation.size() == 2) {
    dilationH = dilation[0].cast<IntegerAttr>().getInt();
    dilationW = dilation[1].cast<IntegerAttr>().getInt();
  }
  
  // 计算输出形状
  int64_t N = inputShape[0];
  int64_t K = weightShape[0];
  
  // H' = (H + 2*padding - dilation*(R-1) - 1) / stride + 1
  int64_t H_out = (inputShape[2] + 2*paddingH - 
                   dilationH*(weightShape[2]-1) - 1) / strideH + 1;
  int64_t W_out = (inputShape[3] + 2*paddingW - 
                   dilationW*(weightShape[3]-1) - 1) / strideW + 1;
  
  // 构造输出类型
  auto outputShape = llvm::SmallVector<int64_t>{N, K, H_out, W_out};
  auto outputType = RankedTensorType::get(
      outputShape, inputType.getElementType());
  
  inferredReturnTypes.push_back(outputType);
  return success();
}
```

**测试形状推导**:

```cpp
// test/Dialect/AI/ShapeInferenceTest.cpp

TEST_F(AIDialectTest, Conv2DShapeInference) {
  auto loc = builder.getUnknownLoc();
  
  // 输入: [1, 3, 224, 224]
  auto inputType = RankedTensorType::get(
      {1, 3, 224, 224}, builder.getF32Type());
  
  // 权重: [64, 3, 7, 7]
  auto weightType = RankedTensorType::get(
      {64, 3, 7, 7}, builder.getF32Type());
  
  // 创建操作（不指定输出类型）
  SmallVector<Type> inferredTypes;
  ASSERT_TRUE(succeeded(
      Conv2DOp::inferReturnTypes(
          &context, loc, {input, weight},
          builder.getDictionaryAttr({
              builder.getNamedAttr("stride", builder.getI64ArrayAttr({2, 2})),
              builder.getNamedAttr("padding", builder.getI64ArrayAttr({3, 3}))
          }),
          {}, inferredTypes)));
  
  // 验证推导的形状: [1, 64, 112, 112]
  auto outputType = inferredTypes[0].cast<RankedTensorType>();
  EXPECT_EQ(outputType.getShape(), ArrayRef<int64_t>({1, 64, 112, 112}));
}
```

✅ **验收标准**:
- [ ] Conv2D/MatMul/Pool等算子支持形状推导
- [ ] 形状推导测试通过
- [ ] 能处理动态形状（-1维度）

---

### Week 3: Pass开发

#### Task 3.1: 实现MatMul+Add融合（GEMM）

```cpp
// lib/Transforms/FusionPasses/MatMulAddFusion.cpp

struct MatMulAddFusionPattern : public OpRewritePattern<ai::AddOp> {
  using OpRewritePattern<ai::AddOp>::OpRewritePattern;

  LogicalResult matchAndRewrite(ai::AddOp addOp,
                                PatternRewriter &rewriter) const override {
    // 匹配模式: MatMul(%A, %B) + %bias
    auto matmulOp = addOp.getLhs().getDefiningOp<ai::MatMulOp>();
    if (!matmulOp || !matmulOp->hasOneUse())
      return failure();
    
    Value bias = addOp.getRhs();
    
    // 验证bias形状（应该可广播到matmul输出）
    if (!isBroadcastable(bias.getType(), matmulOp.getType()))
      return failure();
    
    // 创建GEMM操作 (General Matrix Multiply: C = α*AB + β*C)
    rewriter.replaceOpWithNewOp<ai::GEMMOp>(
        addOp, addOp.getType(),
        matmulOp.getLhs(), matmulOp.getRhs(), bias,
        /*alpha=*/rewriter.getF32FloatAttr(1.0),
        /*beta=*/rewriter.getF32FloatAttr(1.0));
    
    return success();
  }

private:
  bool isBroadcastable(Type biasType, Type matmulType) const {
    // 实现广播规则检查
    // ...
  }
};

// 注册Pass
std::unique_ptr<Pass> createMatMulAddFusionPass() {
  return std::make_unique<MatMulAddFusionPass>();
}
```

**在Passes.td中定义**:

```tablegen
def MatMulAddFusion : Pass<"ai-matmul-add-fusion", "::mlir::func::FuncOp"> {
  let summary = "Fuse MatMul and Add into GEMM";
  let description = [{
    将MatMul + Add模式融合为单个GEMM操作，减少内存访问和kernel启动开销。
    
    示例:
    ```
    %matmul = ai.matmul %A, %B : ...
    %result = ai.add %matmul, %bias : ...
    ```
    
    转换为:
    ```
    %result = ai.gemm %A, %B, %bias : ...
    ```
  }];
  
  let constructor = "mlir::createMatMulAddFusionPass()";
  let dependentDialects = ["ai::AIDialect"];
}
```

**测试**:

```mlir
// test/Transforms/matmul_add_fusion.mlir

// RUN: acompiler-opt %s -ai-matmul-add-fusion | FileCheck %s

module {
  // CHECK-LABEL: @gemm_fusion
  func.func @gemm_fusion(%A: tensor<128x256xf32>,
                          %B: tensor<256x512xf32>,
                          %bias: tensor<512xf32>) -> tensor<128x512xf32> {
    // CHECK-NOT: ai.matmul
    // CHECK-NOT: ai.add
    // CHECK: ai.gemm
    %matmul = ai.matmul %A, %B : (tensor<128x256xf32>, tensor<256x512xf32>) 
                              -> tensor<128x512xf32>
    %result = ai.add %matmul, %bias : tensor<128x512xf32>
    return %result : tensor<128x512xf32>
  }
}
```

#### Task 3.2: 实现通用模式匹配框架

```cpp
// include/acompiler/Transforms/PatternUtils.h

/// 通用的序列融合模板
template<typename Op1, typename Op2, typename FusedOp>
struct BinaryFusionPattern : public OpRewritePattern<Op2> {
  using OpRewritePattern<Op2>::OpRewritePattern;

  LogicalResult matchAndRewrite(Op2 op2,
                                PatternRewriter &rewriter) const override {
    auto op1 = op2.getOperand(0).template getDefiningOp<Op1>();
    if (!op1 || !op1->hasOneUse())
      return failure();
    
    return fuseOps(op1, op2, rewriter);
  }

protected:
  virtual LogicalResult fuseOps(Op1 op1, Op2 op2,
                               PatternRewriter &rewriter) const = 0;
};

// 使用示例
struct ConvBNFusionPattern : public BinaryFusionPattern<ai::Conv2DOp,
                                                        ai::BatchNormOp,
                                                        ai::Conv2DOp> {
  LogicalResult fuseOps(ai::Conv2DOp convOp,
                       ai::BatchNormOp bnOp,
                       PatternRewriter &rewriter) const override {
    // 融合逻辑：将BN参数折叠到Conv权重中
    auto fusedWeight = foldBatchNormWeights(
        convOp.getWeight(), bnOp.getMean(), bnOp.getVariance(),
        bnOp.getScale(), bnOp.getEpsilon());
    
    auto fusedBias = foldBatchNormBias(
        convOp.getBias(), bnOp.getMean(), bnOp.getVariance(),
        bnOp.getScale(), bnOp.getBias(), bnOp.getEpsilon());
    
    rewriter.replaceOpWithNewOp<ai::Conv2DOp>(
        bnOp, bnOp.getType(),
        convOp.getInput(), fusedWeight, fusedBias,
        convOp.getStrideAttr(), convOp.getPaddingAttr());
    
    return success();
  }
};
```

✅ **验收标准**:
- [ ] 实现至少5个融合Pass
- [ ] 所有Pass有对应的FileCheck测试
- [ ] Pass能正确处理不可融合的情况

---

### Week 4: ONNX前端

#### Task 4.1: 实现ONNX导入器核心框架

**完整的导入器实现**:

```cpp
// tools/acompiler-translate/ONNXImporter.cpp

class ONNXImporter {
public:
  ONNXImporter(MLIRContext *context) : context(context), builder(context) {
    context->loadDialect<ai::AIDialect>();
    context->loadDialect<func::FuncDialect>();
  }
  
  LogicalResult importModel(const std::string &path, ModuleOp module) {
    // 1. 加载ONNX模型
    ONNX_NAMESPACE::ModelProto model;
    std::ifstream input(path, std::ios::binary);
    if (!model.ParseFromIstream(&input)) {
      llvm::errs() << "Failed to parse ONNX model\n";
      return failure();
    }
    
    const auto &graph = model.graph();
    
    // 2. 创建符号表（name -> Value映射）
    for (const auto &initializer : graph.initializer()) {
      if (failed(importInitializer(initializer))) {
        return failure();
      }
    }
    
    // 3. 创建主函数
    auto funcType = getFunctionType(graph);
    auto func = builder.create<func::FuncOp>(
        module.getLoc(), "main", funcType);
    
    auto *entryBlock = func.addEntryBlock();
    builder.setInsertionPointToStart(entryBlock);
    
    // 4. 注册函数参数
    for (auto [idx, input] : llvm::enumerate(graph.input())) {
      valueMap[input.name()] = func.getArgument(idx);
    }
    
    // 5. 转换所有节点
    for (const auto &node : graph.node()) {
      if (failed(importNode(node))) {
        return failure();
      }
    }
    
    // 6. 创建return
    SmallVector<Value> results;
    for (const auto &output : graph.output()) {
      results.push_back(valueMap[output.name()]);
    }
    builder.create<func::ReturnOp>(builder.getUnknownLoc(), results);
    
    module.push_back(func);
    return success();
  }

private:
  LogicalResult importNode(const ONNX_NAMESPACE::NodeProto &node) {
    StringRef opType = node.op_type();
    
    // 查找转换器
    auto it = converters.find(opType);
    if (it == converters.end()) {
      llvm::errs() << "Unsupported ONNX op: " << opType << "\n";
      return failure();
    }
    
    return it->second(node, *this);
  }
  
  LogicalResult convertConv(const ONNX_NAMESPACE::NodeProto &node) {
    // 获取输入
    Value input = valueMap[node.input(0)];
    Value weight = valueMap[node.input(1)];
    Value bias = node.input_size() > 2 ? valueMap[node.input(2)] : nullptr;
    
    // 提取属性
    auto stride = getIntArrayAttr(node, "strides", {1, 1});
    auto padding = getIntArrayAttr(node, "pads", {0, 0});
    auto dilation = getIntArrayAttr(node, "dilations", {1, 1});
    auto groups = getIntAttr(node, "group", 1);
    
    // 推导输出类型
    auto outputType = inferConvOutputType(
        input.getType(), weight.getType(), stride, padding, dilation);
    
    // 创建Conv2D操作
    auto convOp = builder.create<ai::Conv2DOp>(
        builder.getUnknownLoc(), outputType,
        input, weight, bias, stride, padding, dilation, groups);
    
    // 注册输出
    valueMap[node.output(0)] = convOp.getResult();
    return success();
  }
  
  // 注册所有支持的算子转换器
  void registerConverters() {
    converters["Conv"] = [](const auto &node, auto &importer) {
      return importer.convertConv(node);
    };
    converters["Relu"] = [](const auto &node, auto &importer) {
      return importer.convertRelu(node);
    };
    converters["MatMul"] = [](const auto &node, auto &importer) {
      return importer.convertMatMul(node);
    };
    // 添加更多...
  }
  
  MLIRContext *context;
  OpBuilder builder;
  DenseMap<std::string, Value> valueMap;
  StringMap<std::function<LogicalResult(const ONNX_NAMESPACE::NodeProto&, 
                                        ONNXImporter&)>> converters;
};
```

**命令行工具**:

```cpp
// tools/acompiler-translate/acompiler-translate.cpp

int main(int argc, char **argv) {
  cl::opt<std::string> inputFile(cl::Positional,
                                  cl::desc("<input file>"),
                                  cl::Required);
  
  cl::opt<std::string> outputFile("o",
                                  cl::desc("Output file"),
                                  cl::value_desc("filename"));
  
  cl::opt<bool> importONNX("import-onnx",
                          cl::desc("Import ONNX model"));
  
  cl::ParseCommandLineOptions(argc, argv);
  
  MLIRContext context;
  context.loadAllAvailableDialects();
  
  if (importONNX) {
    auto module = ModuleOp::create(UnknownLoc::get(&context));
    ONNXImporter importer(&context);
    
    if (failed(importer.importModel(inputFile, module))) {
      llvm::errs() << "Failed to import ONNX model\n";
      return 1;
    }
    
    // 输出MLIR
    if (!outputFile.empty()) {
      std::error_code ec;
      llvm::raw_fd_ostream out(outputFile, ec);
      module.print(out);
    } else {
      module.print(llvm::outs());
    }
  }
  
  return 0;
}
```

**使用示例**:

```bash
# 导入ResNet-18
./build/bin/acompiler-translate \
  --import-onnx models/resnet18.onnx \
  -o resnet18.mlir

# 查看生成的MLIR
cat resnet18.mlir
```

✅ **验收标准**:
- [ ] 支持20+个ONNX算子
- [ ] 成功导入ResNet-18, MobileNetV2
- [ ] 生成的MLIR通过`acompiler-opt --verify-diagnostics`

---

## 🐛 常见问题

### Q1: CMake找不到MLIR

**问题**: `CMake Error: Could not find MLIRConfig.cmake`

**解决**:
```bash
# 确保MLIR_DIR指向正确的CMake配置目录
export MLIR_DIR=/workspace/third_party/llvm-install/lib/cmake/mlir
export LLVM_DIR=/workspace/third_party/llvm-install/lib/cmake/llvm

cmake .. -DMLIR_DIR=$MLIR_DIR -DLLVM_DIR=$LLVM_DIR
```

### Q2: TableGen生成失败

**问题**: `mlir-tblgen: error: Could not find include file 'mlir/IR/OpBase.td'`

**解决**:
确保CMakeLists.txt中正确设置了包含路径：
```cmake
include_directories(${MLIR_INCLUDE_DIRS})
```

### Q3: 链接错误

**问题**: `undefined reference to mlir::Operation::create(...)`

**解决**:
确保链接了所有必需的MLIR库：
```cmake
target_link_libraries(your_target
  MLIRIR
  MLIRSupport
  MLIRParser
  # 添加其他必需的库
)
```

### Q4: ONNX导入失败

**问题**: `Failed to parse ONNX model`

**解决**:
1. 检查ONNX版本兼容性
2. 验证模型文件完整性：`python -m onnx.checker model.onnx`
3. 查看详细错误信息

---

## 📝 检查清单模板

复制以下清单追踪您的进度：

```markdown
## Week 1: 工程搭建
- [ ] LLVM/MLIR 17.0+安装完成
- [ ] ACompiler项目可编译
- [ ] 测试框架工作
- [ ] compile_commands.json生成

## Week 2: Dialect开发
- [ ] AI Dialect包含15+算子
- [ ] 所有算子有verify()
- [ ] 形状推导实现
- [ ] 测试覆盖>70%

## Week 3: Pass开发
- [ ] 5+个融合Pass实现
- [ ] FileCheck测试完整
- [ ] Pass pipeline配置

## Week 4: ONNX导入
- [ ] 支持20+个算子
- [ ] ResNet-18导入成功
- [ ] MLIR验证通过
```

---

**下一部分**: [第二阶段实施指南](./IMPLEMENTATION_GUIDE_PHASE2.md)

