# ACompiler 项目规划（增强版）

## 📋 项目概述

ACompiler是一个基于LLVM/MLIR的AI编译器教育项目，通过实例驱动的方式系统学习现代编译器技术。项目采用**自上而下、由浅入深**的渐进式学习路线，从MLIR高层抽象开始，逐步深入到LLVM IR优化和机器码生成。

### 🎯 项目目标

| 目标类型 | 具体目标 | 验收标准 |
|---------|---------|---------|
| **学习目标** | 掌握LLVM/MLIR编译器框架 | 能独立实现自定义Dialect和Pass |
| **技术目标** | 实现多后端AI编译器 | 成功编译并运行ResNet-18模型 |
| **工程目标** | 建立完整工程体系 | 代码覆盖率>80%，文档完整 |
| **性能目标** | CPU后端性能优化 | 达到ONNX Runtime性能的60%+ |

### 💡 核心特性

- ✅ **渐进式学习**: 从简单到复杂，逐步掌握编译技术
- ✅ **实例驱动**: 每个概念都有完整的代码示例
- ✅ **多后端支持**: CPU(实现) + CUDA(规划) + Ascend(规划)
- ✅ **完整文档**: 详细的教程、API文档和实践指南
- ✅ **测试完备**: 单元测试、集成测试、性能测试

---

## 🗓️ 项目分期规划

### 第一阶段：基础架构与MLIR入门（Week 1-4）

**目标**: 建立项目基础，掌握MLIR核心概念，实现自定义AI Dialect

#### 1.1 项目工程搭建（Week 1）

**任务清单**:
- [x] 创建项目目录结构
- [ ] 配置CMake构建系统
- [ ] 集成LLVM/MLIR依赖（17.0+）
- [ ] 建立单元测试框架（GoogleTest）
- [ ] 配置CI/CD流程（GitHub Actions）

**实施步骤**:

```bash
# 1. 安装LLVM/MLIR
cd third_party
git clone --depth 1 --branch llvmorg-17.0.0 https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build && cd build
cmake -G Ninja ../llvm \
  -DLLVM_ENABLE_PROJECTS="mlir" \
  -DLLVM_TARGETS_TO_BUILD="X86;ARM" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON
ninja

# 2. 配置ACompiler构建
cd /workspace
mkdir build && cd build
cmake -G Ninja .. \
  -DLLVM_DIR=/workspace/third_party/llvm-project/build/lib/cmake/llvm \
  -DMLIR_DIR=/workspace/third_party/llvm-project/build/lib/cmake/mlir
ninja
```

**验收标准**:
- ✓ 项目能成功编译
- ✓ `acompiler-opt --version` 正常输出
- ✓ 单元测试框架可运行

#### 1.2 MLIR基础概念实践（Week 2）

##### 1.2.1 Dialect定义

**创建AI Dialect完整流程**:

**步骤1**: 定义Dialect基础（`include/acompiler/Dialect/AI/AI.td`）

```tablegen
// AI Dialect定义
def AI_Dialect : Dialect {
  let name = "ai";
  let cppNamespace = "::mlir::ai";
  let summary = "AI operations dialect for deep learning";
  let description = [{
    提供常见深度学习算子的高层抽象，包括卷积、矩阵运算、
    激活函数、池化、归一化等操作。
  }];
  
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
  
  let extraClassDeclaration = [{
    // 注册所有Operations
    void registerOperations();
    // 注册所有Types
    void registerTypes();
    // 注册所有Attributes
    void registerAttributes();
  }];
}
```

**步骤2**: 定义核心算子（扩展`AIOps.td`）

```tablegen
//===----------------------------------------------------------------------===//
// 卷积操作
//===----------------------------------------------------------------------===//

def AI_Conv2DOp : AI_Op<"conv2d", [Pure, DeclareOpInterfaceMethods<InferTypeOpInterface>]> {
  let summary = "2D convolution operation";
  let description = [{
    执行2D卷积操作：output[n,k,h',w'] = sum(input[n,c,h,w] * weight[k,c,r,s])
    
    示例:
    ```mlir
    %output = ai.conv2d %input, %weight {
      stride = [1, 1],
      padding = [1, 1],
      dilation = [1, 1],
      groups = 1
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) -> tensor<1x64x224x224xf32>
    ```
  }];
  
  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$weight,
    OptionalAttr<AnyRankedTensor>:$bias,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$stride,
    DefaultValuedAttr<I64ArrayAttr, "{0, 0}">:$padding,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$dilation,
    DefaultValuedAttr<I64Attr, "1">:$groups
  );
  
  let results = (outs AnyRankedTensor:$output);
  
  let assemblyFormat = [{
    $input `,` $weight (`,` $bias^)? attr-dict `:` 
    `(` type($input) `,` type($weight) (`,` type($bias)^)? `)` `->` type($output)
  }];
  
  let hasVerifier = 1;
  let hasFolder = 1;
}

//===----------------------------------------------------------------------===//
// BatchMatMul操作（支持批量矩阵乘法）
//===----------------------------------------------------------------------===//

def AI_BatchMatMulOp : AI_Op<"batch_matmul", [Pure]> {
  let summary = "Batch matrix multiplication";
  let description = [{
    批量矩阵乘法：C[b,m,n] = A[b,m,k] @ B[b,k,n]
    支持广播机制。
  }];
  
  let arguments = (ins
    AnyRankedTensor:$lhs,
    AnyRankedTensor:$rhs,
    DefaultValuedAttr<BoolAttr, "false">:$transpose_lhs,
    DefaultValuedAttr<BoolAttr, "false">:$transpose_rhs
  );
  
  let results = (outs AnyRankedTensor:$output);
  
  let assemblyFormat = [{
    $lhs `,` $rhs attr-dict `:` 
    `(` type($lhs) `,` type($rhs) `)` `->` type($output)
  }];
}

//===----------------------------------------------------------------------===//
// 激活函数
//===----------------------------------------------------------------------===//

def AI_GELUOp : AI_Op<"gelu", [Pure, SameOperandsAndResultType]> {
  let summary = "GELU activation function";
  let description = [{
    Gaussian Error Linear Unit: GELU(x) = x * Φ(x)
    其中Φ(x)是标准正态分布的累积分布函数
  }];
  
  let arguments = (ins AnyTensor:$input);
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = "$input attr-dict `:` type($input)";
}
```

**步骤3**: 实现验证逻辑（`lib/Dialect/AI/AIOps.cpp`）

```cpp
LogicalResult Conv2DOp::verify() {
  auto inputType = getInput().getType().cast<RankedTensorType>();
  auto weightType = getWeight().getType().cast<RankedTensorType>();
  auto outputType = getOutput().getType().cast<RankedTensorType>();
  
  // 验证输入维度
  if (inputType.getRank() != 4) {
    return emitOpError("input must be 4D tensor (NCHW format)");
  }
  
  if (weightType.getRank() != 4) {
    return emitOpError("weight must be 4D tensor (KCHW format)");
  }
  
  // 验证通道数匹配
  int64_t inputChannels = inputType.getShape()[1];
  int64_t weightChannels = weightType.getShape()[1];
  int64_t groups = getGroups();
  
  if (inputChannels != weightChannels * groups) {
    return emitOpError("input channels must equal weight channels * groups");
  }
  
  // 验证stride/padding属性
  auto stride = getStride();
  if (stride && stride->size() != 2) {
    return emitOpError("stride must have 2 elements");
  }
  
  return success();
}

// 形状推导
LogicalResult Conv2DOp::inferReturnTypes(
    MLIRContext *context, Optional<Location> location,
    ValueRange operands, DictionaryAttr attributes,
    RegionRange regions, SmallVectorImpl<Type> &inferredReturnTypes) {
  
  // 从操作数中提取类型
  auto inputType = operands[0].getType().cast<RankedTensorType>();
  auto weightType = operands[1].getType().cast<RankedTensorType>();
  
  // 提取属性
  auto strideAttr = attributes.get("stride").dyn_cast_or_null<ArrayAttr>();
  auto paddingAttr = attributes.get("padding").dyn_cast_or_null<ArrayAttr>();
  
  // 计算输出形状
  auto inputShape = inputType.getShape();
  auto weightShape = weightType.getShape();
  
  int64_t N = inputShape[0];
  int64_t K = weightShape[0];
  
  // 计算输出H'和W'
  int64_t H_out = computeOutputSize(inputShape[2], weightShape[2], 
                                     stride[0], padding[0]);
  int64_t W_out = computeOutputSize(inputShape[3], weightShape[3],
                                     stride[1], padding[1]);
  
  // 构造输出类型
  auto outputShape = SmallVector<int64_t>{N, K, H_out, W_out};
  auto outputType = RankedTensorType::get(outputShape, inputType.getElementType());
  inferredReturnTypes.push_back(outputType);
  
  return success();
}
```

##### 1.2.2 Type System实现

**定义自定义张量类型**（`AITypes.td`）:

```tablegen
def AI_TensorType : AI_Type<"Tensor", "tensor"> {
  let summary = "AI tensor type with shape and dtype";
  
  let parameters = (ins
    ArrayRefParameter<"int64_t">:$shape,
    "Type":$elementType,
    OptionalParameter<"Attribute">:$encoding
  );
  
  let builders = [
    TypeBuilderWithInferredContext<(ins
      "ArrayRef<int64_t>":$shape,
      "Type":$elementType
    ), [{
      return $_get(elementType.getContext(), shape, elementType, nullptr);
    }]>
  ];
  
  let assemblyFormat = "`<` $shape `,` $elementType (`,` $encoding^)? `>`";
}

// 量化类型支持
def AI_QuantizedType : AI_Type<"Quantized", "quant"> {
  let summary = "Quantized tensor type for INT8/INT4";
  
  let parameters = (ins
    ArrayRefParameter<"int64_t">:$shape,
    "Type":$storageType,  // int8, int4
    "Type":$expressedType, // float32
    "float":$scale,
    "int64_t":$zeroPoint
  );
}
```

##### 1.2.3 Attribute系统

```tablegen
def AI_ActivationAttr : AI_Attr<"Activation", "activation"> {
  let summary = "Activation function type";
  
  let parameters = (ins
    "ActivationType":$type
  );
  
  let assemblyFormat = "$type";
  
  let extraClassDeclaration = [{
    enum class ActivationType {
      None,
      ReLU,
      ReLU6,
      Sigmoid,
      Tanh,
      GELU
    };
  }];
}
```

**验收标准**:
- ✓ 成功定义至少10个AI算子
- ✓ 所有算子都有verify()实现
- ✓ 通过MLIR的`--verify-diagnostics`测试

#### 1.3 MLIR Pass开发（Week 3）

##### 1.3.1 算子融合Pass

**Conv+BatchNorm+ReLU融合实现**:

```cpp
// lib/Transforms/FusionPasses/ConvBNReLUFusion.cpp

struct ConvBNReLUFusionPattern : public OpRewritePattern<ai::ReLUOp> {
  using OpRewritePattern<ai::ReLUOp>::OpRewritePattern;

  LogicalResult matchAndRewrite(ai::ReLUOp reluOp,
                                PatternRewriter &rewriter) const override {
    // 匹配模式: Conv -> BatchNorm -> ReLU
    auto bnOp = reluOp.getInput().getDefiningOp<ai::BatchNormOp>();
    if (!bnOp || !bnOp->hasOneUse())
      return failure();
    
    auto convOp = bnOp.getInput().getDefiningOp<ai::Conv2DOp>();
    if (!convOp || !convOp->hasOneUse())
      return failure();
    
    // 折叠BatchNorm参数到Conv权重中
    auto fusedWeight = foldBatchNormIntoConv(
        convOp.getWeight(), convOp.getBias(),
        bnOp.getMean(), bnOp.getVariance(), 
        bnOp.getScale(), bnOp.getBias(), bnOp.getEpsilon());
    
    // 创建融合的ConvReLU操作
    rewriter.replaceOpWithNewOp<ai::ConvReLUOp>(
        reluOp, reluOp.getType(),
        convOp.getInput(), fusedWeight.first, fusedWeight.second,
        convOp.getStrideAttr(), convOp.getPaddingAttr());
    
    return success();
  }

private:
  // 将BatchNorm参数融合到Conv权重中
  std::pair<Value, Value> foldBatchNormIntoConv(
      Value weight, Value bias,
      Value mean, Value variance,
      Value scale, Value bnBias, FloatAttr epsilon) const {
    // weight_new = weight * scale / sqrt(variance + epsilon)
    // bias_new = (bias - mean) * scale / sqrt(variance + epsilon) + bnBias
    // 这里需要在编译时完成常量计算
    // ...
  }
};
```

##### 1.3.2 常量折叠Pass

```cpp
struct AIConstantFoldingPass : public PassWrapper<AIConstantFoldingPass, 
                                                    OperationPass<func::FuncOp>> {
  void runOnOperation() override {
    auto func = getOperation();
    
    func.walk([&](Operation *op) {
      // 尝试折叠每个操作
      SmallVector<OpFoldResult> results;
      if (succeeded(op->fold(results))) {
        // 替换为常量
        for (auto [result, foldResult] : llvm::zip(op->getResults(), results)) {
          if (auto attr = foldResult.dyn_cast<Attribute>()) {
            auto constOp = rewriter.create<arith::ConstantOp>(
                op->getLoc(), attr);
            result.replaceAllUsesWith(constOp);
          }
        }
      }
    });
  }
};
```

##### 1.3.3 形状推导Pass

```cpp
struct AIShapeInferencePass : public PassWrapper<AIShapeInferencePass,
                                                  OperationPass<ModuleOp>> {
  void runOnOperation() override {
    auto module = getOperation();
    
    // 遍历所有函数
    module.walk([&](func::FuncOp funcOp) {
      // 从函数参数开始推导形状
      SmallVector<Type> argTypes(funcOp.getArgumentTypes().begin(),
                                  funcOp.getArgumentTypes().end());
      
      // 前向传播形状信息
      funcOp.walk([&](Operation *op) {
        if (auto shapeOp = dyn_cast<InferTypeOpInterface>(op)) {
          SmallVector<Type> inferredTypes;
          if (succeeded(shapeOp.inferReturnTypes(
                op->getContext(), op->getLoc(), op->getOperands(),
                op->getAttrDictionary(), op->getRegions(), inferredTypes))) {
            
            // 更新结果类型
            for (auto [result, inferredType] : 
                 llvm::zip(op->getResults(), inferredTypes)) {
              result.setType(inferredType);
            }
          }
        }
      });
    });
  }
};
```

**验收标准**:
- ✓ 实现至少5个优化Pass
- ✓ 每个Pass都有对应的测试用例
- ✓ 融合Pass能正确识别并优化模式

#### 1.4 前端支持（Week 4）

##### 1.4.1 ONNX导入器实现

```cpp
// tools/acompiler-translate/ONNXImporter.cpp

class ONNXImporter {
public:
  LogicalResult importModel(const std::string &modelPath,
                            mlir::ModuleOp module) {
    // 1. 加载ONNX模型
    onnx::ModelProto model;
    std::ifstream input(modelPath, std::ios::binary);
    if (!model.ParseFromIstream(&input)) {
      return failure();
    }
    
    // 2. 创建MLIR函数
    auto funcType = builder.getFunctionType(
        getInputTypes(model), getOutputTypes(model));
    auto func = builder.create<func::FuncOp>(
        module.getLoc(), "main", funcType);
    
    // 3. 转换ONNX节点到MLIR操作
    for (const auto &node : model.graph().node()) {
      if (failed(convertNode(node, func))) {
        return failure();
      }
    }
    
    return success();
  }

private:
  LogicalResult convertNode(const onnx::NodeProto &node,
                           func::FuncOp func) {
    StringRef opType = node.op_type();
    
    if (opType == "Conv") {
      return convertConvOp(node, func);
    } else if (opType == "MatMul") {
      return convertMatMulOp(node, func);
    } else if (opType == "Relu") {
      return convertReLUOp(node, func);
    }
    // ... 更多算子转换
    
    return failure();
  }
  
  LogicalResult convertConvOp(const onnx::NodeProto &node,
                              func::FuncOp func) {
    // 提取属性
    auto stride = getAttributeAsArray(node, "strides");
    auto padding = getAttributeAsArray(node, "pads");
    auto dilation = getAttributeAsArray(node, "dilations");
    
    // 创建Conv2D操作
    auto convOp = builder.create<ai::Conv2DOp>(
        loc, outputType, input, weight, bias,
        builder.getI64ArrayAttr(stride),
        builder.getI64ArrayAttr(padding),
        builder.getI64ArrayAttr(dilation));
    
    return success();
  }
};
```

**使用示例**:

```bash
# 导入ONNX模型
./build/bin/acompiler-translate --import-onnx resnet18.onnx -o resnet18.mlir

# 查看生成的MLIR
cat resnet18.mlir
```

**验收标准**:
- ✓ 支持至少20个常见ONNX算子
- ✓ 能成功导入ResNet-18/MobileNet等模型
- ✓ 导入后的MLIR通过验证

---

### 第二阶段：LLVM IR优化（Week 5-7）

**目标**: 实现从MLIR到LLVM IR的完整降级，集成LLVM优化Pass

#### 2.1 MLIR到LLVM IR降级（Week 5）

##### 2.1.1 多层降级实现

**降级流程**:

```
AI Dialect
  ↓ (ConvertAIToLinalg)
Linalg Dialect  [通用张量操作]
  ↓ (ConvertLinalgToLoops)
SCF Dialect  [结构化控制流]
  ↓ (ConvertSCFToCF)
CF Dialect  [控制流图]
  ↓ (ConvertToLLVM)
LLVM Dialect
```

**Conv2D降级示例**:

```cpp
// lib/Conversion/AIToLinalg/ConvLowering.cpp

struct ConvOpLowering : public OpConversionPattern<ai::Conv2DOp> {
  using OpConversionPattern<ai::Conv2DOp>::OpConversionPattern;

  LogicalResult matchAndRewrite(
      ai::Conv2DOp op, OpAdaptor adaptor,
      ConversionPatternRewriter &rewriter) const override {
    
    auto loc = op.getLoc();
    auto outputType = op.getType().cast<RankedTensorType>();
    
    // 创建输出tensor
    Value init = rewriter.create<tensor::EmptyOp>(
        loc, outputType.getShape(), outputType.getElementType());
    
    // 创建零常量用于初始化
    Value zero = rewriter.create<arith::ConstantOp>(
        loc, rewriter.getZeroAttr(outputType.getElementType()));
    
    // 填充零值
    Value initFilled = rewriter.create<linalg::FillOp>(
        loc, zero, init).getResult(0);
    
    // 创建linalg.conv_2d_nchw_fchw操作
    auto convOp = rewriter.create<linalg::Conv2DNchwFchwOp>(
        loc, outputType,
        ValueRange{adaptor.getInput(), adaptor.getWeight()},
        ValueRange{initFilled},
        op.getStride(), op.getDilation());
    
    // 如果有bias，添加add操作
    Value result = convOp.getResult(0);
    if (op.getBias()) {
      result = rewriter.create<linalg::AddOp>(
          loc, result, adaptor.getBias());
    }
    
    rewriter.replaceOp(op, result);
    return success();
  }
};
```

**Linalg到循环的降级**:

```mlir
// 输入: Linalg抽象
%result = linalg.conv_2d_nchw_fchw
  ins(%input, %weight : tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>)
  outs(%init : tensor<1x64x224x224xf32>) -> tensor<1x64x224x224xf32>

// 输出: 嵌套循环 (SCF)
scf.for %n = 0 to 1 {
  scf.for %k = 0 to 64 {
    scf.for %h = 0 to 224 {
      scf.for %w = 0 to 224 {
        scf.for %c = 0 to 3 {
          scf.for %r = 0 to 3 {
            scf.for %s = 0 to 3 {
              %val = load %input[%n, %c, %h+%r, %w+%s]
              %wgt = load %weight[%k, %c, %r, %s]
              %prod = arith.mulf %val, %wgt
              %acc = load %output[%n, %k, %h, %w]
              %sum = arith.addf %acc, %prod
              store %sum, %output[%n, %k, %h, %w]
            }
          }
        }
      }
    }
  }
}
```

##### 2.1.2 Runtime库接口设计

**C API设计**:

```c
// include/acompiler/Runtime/API.h

// Runtime句柄
typedef struct ACompilerRuntime* acompiler_runtime_t;

// Tensor句柄
typedef struct ACompilerTensor* acompiler_tensor_t;

// 数据类型枚举
typedef enum {
  ACOMPILER_FLOAT32 = 0,
  ACOMPILER_FLOAT16 = 1,
  ACOMPILER_INT32 = 2,
  ACOMPILER_INT8 = 3,
} acompiler_dtype_t;

// ===== Runtime管理 =====

// 创建Runtime实例
acompiler_runtime_t acompiler_create_runtime(const char* backend);

// 销毁Runtime
void acompiler_destroy_runtime(acompiler_runtime_t runtime);

// ===== Tensor操作 =====

// 分配Tensor
acompiler_tensor_t acompiler_alloc_tensor(
    acompiler_runtime_t runtime,
    const int64_t* shape, int ndim,
    acompiler_dtype_t dtype);

// 释放Tensor
void acompiler_free_tensor(acompiler_tensor_t tensor);

// 从主机内存拷贝数据到Tensor
void acompiler_tensor_from_host(
    acompiler_tensor_t tensor,
    const void* data, size_t size);

// 从Tensor拷贝数据到主机内存
void acompiler_tensor_to_host(
    acompiler_tensor_t tensor,
    void* data, size_t size);

// ===== 模型执行 =====

// 加载编译后的模型
typedef struct ACompilerModel* acompiler_model_t;

acompiler_model_t acompiler_load_model(
    acompiler_runtime_t runtime,
    const char* model_path);

// 执行模型推理
void acompiler_execute(
    acompiler_model_t model,
    acompiler_tensor_t* inputs, int num_inputs,
    acompiler_tensor_t* outputs, int num_outputs);

// 卸载模型
void acompiler_unload_model(acompiler_model_t model);
```

**LLVM IR生成示例**:

```cpp
// 生成Runtime调用的LLVM IR
void generateRuntimeCall(OpBuilder &builder, Location loc,
                         StringRef functionName,
                         ArrayRef<Value> args) {
  auto module = builder.getInsertionBlock()->getParentOp()
                    ->getParentOfType<ModuleOp>();
  
  // 声明Runtime函数
  auto funcType = LLVM::LLVMFunctionType::get(
      LLVM::LLVMVoidType::get(context),
      convertToLLVMTypes(args));
  
  auto func = module.lookupSymbol<LLVM::LLVMFuncOp>(functionName);
  if (!func) {
    OpBuilder::InsertionGuard guard(builder);
    builder.setInsertionPointToStart(module.getBody());
    func = builder.create<LLVM::LLVMFuncOp>(loc, functionName, funcType);
  }
  
  // 生成函数调用
  builder.create<LLVM::CallOp>(loc, func, args);
}
```

**验收标准**:
- ✓ 完成AI Dialect到LLVM Dialect的完整降级
- ✓ 生成的LLVM IR能通过`llc`编译
- ✓ Runtime API完整且文档齐全

#### 2.2 LLVM优化Pass（Week 6）

##### 2.2.1 循环优化

**循环展开示例**:

```cpp
// 应用LLVM的循环优化Pass
void applyLoopOptimizations(ModuleOp module) {
  PassManager pm(module.getContext());
  
  // 循环展开
  pm.addPass(createLoopUnrollPass(4)); // 展开因子4
  
  // 循环向量化
  pm.addPass(createLoopVectorizePass());
  
  // 循环融合
  pm.addPass(createLoopFusionPass());
  
  // 循环分块(Tiling)
  pm.addPass(createLoopTilingPass({32, 32}));
  
  if (failed(pm.run(module))) {
    llvm::errs() << "Loop optimization failed\n";
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
  %ptr_next = getelementptr float, float* %ptr, i64 1
  br label %for.body

; 向量化后 (AVX2, 8-wide)
for.body.vec:
  %vec_val = load <8 x float>, <8 x float>* %ptr_vec
  %vec_result = fadd <8 x float> %vec_val, %vec_const
  store <8 x float> %vec_result, <8 x float>* %ptr_vec
  %ptr_vec_next = getelementptr <8 x float>, <8 x float>* %ptr_vec, i64 1
  br label %for.body.vec
```

##### 2.2.2 内存优化

```cpp
// Linalg Tiling用于提高缓存局部性
void applyTiling(linalg::Conv2DNchwFchwOp convOp) {
  OpBuilder builder(convOp);
  
  // Tile sizes: [batch=1, output_channel=32, height=32, width=32]
  SmallVector<int64_t> tileSizes = {1, 32, 32, 32};
  
  auto tiledOp = linalg::tileLinalgOp(builder, convOp, tileSizes);
  
  // 提升局部性：将Tile后的循环重新排序
  // 优先遍历内存连续的维度
}
```

**验收标准**:
- ✓ 集成至少10个LLVM优化Pass
- ✓ 循环向量化成功应用到卷积/矩阵乘法
- ✓ 优化后性能提升30%+

#### 2.3 指令选择与映射（Week 7）

##### 2.3.1 SIMD指令映射

**AVX2指令示例**:

```cpp
// 使用LLVM内建函数生成AVX2指令
void generateAVX2Conv(OpBuilder &builder, Value input, Value weight) {
  auto f32x8Type = VectorType::get({8}, builder.getF32Type());
  
  // 加载8个float值（使用AVX2的vmovups）
  auto inputVec = builder.create<vector::LoadOp>(
      loc, f32x8Type, input, indices);
  
  auto weightVec = builder.create<vector::LoadOp>(
      loc, f32x8Type, weight, indices);
  
  // 乘法（vfmaddps - fused multiply-add）
  auto result = builder.create<vector::FMAOp>(
      loc, inputVec, weightVec, accumulator);
  
  // 存储结果
  builder.create<vector::StoreOp>(loc, result, output, indices);
}
```

**代价模型**:

```cpp
// 指令延迟和吞吐量模型
struct InstructionCost {
  int latency;      // 延迟（周期）
  float throughput; // 吞吐量（指令/周期）
};

const std::map<StringRef, InstructionCost> x86CostModel = {
  {"fadd", {4, 2.0}},
  {"fmul", {4, 2.0}},
  {"vfmadd", {4, 2.0}},  // AVX2 FMA
  {"load", {5, 2.0}},
  {"store", {1, 1.0}},
};
```

**验收标准**:
- ✓ 卷积/矩阵乘法使用SIMD指令
- ✓ 生成的汇编代码包含AVX/AVX2指令
- ✓ SIMD优化带来2-4x性能提升

---

### 第三阶段：代码生成（Week 8-11）

**目标**: 实现完整的x86_64后端，开发高性能Runtime库

#### 3.1 CPU后端实现（Week 8-9）

##### 3.1.1 完整的端到端流程

**编译流程**:

```cpp
// tools/acompiler/CompilerDriver.cpp

class CompilerDriver {
public:
  LogicalResult compile(const std::string &inputFile,
                       const std::string &outputFile,
                       const CompilerOptions &options) {
    // 1. 加载ONNX模型
    auto module = loadModel(inputFile);
    
    // 2. 应用优化Pass Pipeline
    PassManager pm(module.getContext());
    buildOptimizationPipeline(pm, options);
    if (failed(pm.run(module))) {
      return failure();
    }
    
    // 3. 降级到LLVM IR
    if (failed(lowerToLLVMDialect(module))) {
      return failure();
    }
    
    // 4. 转换为LLVM IR
    auto llvmModule = translateToLLVMIR(module);
    
    // 5. LLVM优化
    optimizeLLVMModule(llvmModule, options.optimizationLevel);
    
    // 6. 代码生成
    if (failed(emitObjectFile(llvmModule, outputFile))) {
      return failure();
    }
    
    return success();
  }

private:
  void buildOptimizationPipeline(PassManager &pm, 
                                 const CompilerOptions &opts) {
    // MLIR优化
    pm.addPass(createShapeInferencePass());
    pm.addPass(createConvReLUFusionPass());
    pm.addPass(createConstantFoldingPass());
    
    // 降级
    pm.addPass(createConvertAIToLinalgPass());
    pm.addPass(createLinalgTilingPass({32, 32}));
    pm.addPass(createConvertLinalgToLoopsPass());
    pm.addPass(createConvertSCFToCFPass());
    pm.addPass(createConvertToLLVMPass());
  }
  
  void optimizeLLVMModule(llvm::Module *module, int optLevel) {
    llvm::PassBuilder PB;
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;
    
    // 注册分析Pass
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    
    // 创建优化Pass Pipeline
    llvm::ModulePassManager MPM;
    if (optLevel == 0) {
      MPM = PB.buildO0DefaultPipeline(llvm::OptimizationLevel::O0);
    } else if (optLevel == 3) {
      MPM = PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O3);
    }
    
    // 运行优化
    MPM.run(*module, MAM);
  }
};
```

##### 3.1.2 Runtime库实现

**CPU Runtime完整实现**:

```cpp
// runtime/cpu/src/cpu_runtime.cpp

class CPURuntime {
public:
  CPURuntime(int numThreads = -1) {
    if (numThreads < 0) {
      numThreads_ = std::thread::hardware_concurrency();
    } else {
      numThreads_ = numThreads;
    }
    
    // 初始化线程池
#ifdef USE_OPENMP
    omp_set_num_threads(numThreads_);
#else
    threadPool_ = std::make_unique<ThreadPool>(numThreads_);
#endif
    
    // 初始化内存池
    memoryPool_ = std::make_unique<MemoryPool>(1024 * 1024 * 1024); // 1GB
  }
  
  // 分配张量
  Tensor* allocTensor(const std::vector<int64_t> &shape,
                      DataType dtype) {
    size_t size = computeSize(shape, dtype);
    void* data = memoryPool_->allocate(size, 64); // 64字节对齐(AVX512)
    
    return new Tensor(shape, dtype, data);
  }
  
  // 执行推理
  void execute(const CompiledModel& model,
               const std::vector<Tensor*>& inputs,
               std::vector<Tensor*>& outputs) {
    // 调用JIT编译的函数
    using ModelFunc = void(*)(void**, void**);
    auto func = reinterpret_cast<ModelFunc>(model.entryPoint);
    
    // 准备输入输出指针数组
    std::vector<void*> inputPtrs, outputPtrs;
    for (auto* t : inputs) inputPtrs.push_back(t->data());
    for (auto* t : outputs) outputPtrs.push_back(t->data());
    
    // 执行
    func(inputPtrs.data(), outputPtrs.data());
  }

private:
  int numThreads_;
  std::unique_ptr<ThreadPool> threadPool_;
  std::unique_ptr<MemoryPool> memoryPool_;
};
```

**高性能卷积实现**（使用im2col + GEMM）:

```cpp
// runtime/cpu/src/ops/conv2d.cpp

void conv2d_im2col(
    const float* input,  // [N, C, H, W]
    const float* weight, // [K, C, R, S]
    float* output,       // [N, K, H', W']
    int N, int C, int H, int W,
    int K, int R, int S,
    int stride, int padding) {
  
  int H_out = (H + 2*padding - R) / stride + 1;
  int W_out = (W + 2*padding - S) / stride + 1;
  
  // 1. im2col: 将输入展开为矩阵
  //    输出: [N*H'*W', C*R*S]
  std::vector<float> im2col_matrix(N*H_out*W_out * C*R*S);
  im2col_cpu(input, im2col_matrix.data(),
             N, C, H, W, R, S, stride, padding, H_out, W_out);
  
  // 2. 重排权重为矩阵
  //    [K, C*R*S]
  
  // 3. GEMM: output = im2col_matrix @ weight^T
  //    [N*H'*W', K] = [N*H'*W', C*R*S] @ [K, C*R*S]^T
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
              N*H_out*W_out, K, C*R*S,
              1.0f, im2col_matrix.data(), C*R*S,
              weight, C*R*S,
              0.0f, output, K);
}

// im2col实现（带SIMD优化）
void im2col_cpu(const float* data_im, float* data_col,
                int N, int C, int H, int W,
                int R, int S, int stride, int padding,
                int H_out, int W_out) {
  
  #pragma omp parallel for collapse(2)
  for (int n = 0; n < N; ++n) {
    for (int h_out = 0; h_out < H_out; ++h_out) {
      for (int w_out = 0; w_out < W_out; ++w_out) {
        int h_in = h_out * stride - padding;
        int w_in = w_out * stride - padding;
        
        float* col_ptr = data_col + 
            ((n * H_out + h_out) * W_out + w_out) * (C*R*S);
        
        for (int c = 0; c < C; ++c) {
          for (int r = 0; r < R; ++r) {
            for (int s = 0; s < S; ++s) {
              int h = h_in + r;
              int w = w_in + s;
              
              *col_ptr++ = (h >= 0 && h < H && w >= 0 && w < W) ?
                  data_im[((n*C + c)*H + h)*W + w] : 0.0f;
            }
          }
        }
      }
    }
  }
}
```

**验收标准**:
- ✓ 端到端编译流程正常工作
- ✓ Runtime库支持多线程执行
- ✓ 内存管理高效（使用内存池）
- ✓ 核心算子有优化实现

#### 3.2 性能优化与基准测试（Week 10-11）

##### 3.2.1 性能基准

**创建性能测试框架**:

```cpp
// test/performance/benchmark.cpp

class OpBenchmark {
public:
  struct BenchmarkResult {
    double avgTime;      // 平均时间(ms)
    double stdDev;       // 标准差
    double throughput;   // 吞吐量(GFLOPS)
    double bandwidth;    // 内存带宽(GB/s)
  };
  
  BenchmarkResult benchmarkConv2D(
      const std::vector<int64_t>& inputShape,
      const std::vector<int64_t>& weightShape,
      int iterations = 100) {
    
    // 预热
    for (int i = 0; i < 10; ++i) {
      runConv2D(inputShape, weightShape);
    }
    
    // 正式测试
    std::vector<double> times;
    for (int i = 0; i < iterations; ++i) {
      auto start = std::chrono::high_resolution_clock::now();
      runConv2D(inputShape, weightShape);
      auto end = std::chrono::high_resolution_clock::now();
      
      double elapsed = std::chrono::duration<double, std::milli>(
          end - start).count();
      times.push_back(elapsed);
    }
    
    // 计算统计信息
    return computeStats(times, inputShape, weightShape);
  }
  
  void printComparison(const std::string& opName,
                      const BenchmarkResult& acompiler,
                      const BenchmarkResult& reference) {
    printf("=== %s Performance ===\n", opName.c_str());
    printf("ACompiler:  %.3f ms, %.2f GFLOPS\n",
           acompiler.avgTime, acompiler.throughput);
    printf("Reference:  %.3f ms, %.2f GFLOPS\n",
           reference.avgTime, reference.throughput);
    printf("Speedup:    %.2fx\n",
           reference.avgTime / acompiler.avgTime);
  }
};
```

**性能目标**:

| 操作 | 输入规模 | ACompiler目标 | ONNX Runtime | 目标比例 |
|-----|---------|--------------|-------------|---------|
| Conv2D | 1x64x56x56, 3x3 | 2.5 ms | 1.5 ms | 60%+ |
| MatMul | 1024x1024x1024 | 15 ms | 10 ms | 65%+ |
| ResNet-18 | 1x3x224x224 | 30 ms | 20 ms | 65%+ |

##### 3.2.2 Profiling和调优

**使用Linux perf进行分析**:

```bash
# 记录性能数据
perf record -g -e cycles,cache-misses ./benchmark

# 分析报告
perf report

# 查看热点函数
perf top

# 分析缓存行为
perf stat -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./benchmark
```

**使用VTune进行深度分析**:

```bash
# Intel VTune Profiler
vtune -collect hotspots -result-dir vtune_results ./benchmark

# 查看结果
vtune-gui vtune_results
```

**验收标准**:
- ✓ Conv2D达到目标性能（60%+ ONNX Runtime）
- ✓ ResNet-18端到端推理成功
- ✓ 性能分析报告完整
- ✓ 识别并优化至少3个性能瓶颈

---

### 第四阶段：多后端扩展（Week 12+，TODO）

#### 4.1 NVIDIA GPU CUDA支持（未实现）

**技术路线**:

```
MLIR (AI Dialect)
  ↓
GPU Dialect (MLIR)
  ↓ (GPU to NVVM)
NVVM Dialect
  ↓ (NVVM to PTX)
PTX Assembly
  ↓ (CUDA Driver)
CUDA Binary
```

**Kernel生成示例**:

```cpp
// 生成CUDA Kernel
void generateCUDAConv(OpBuilder &builder, ai::Conv2DOp op) {
  // 1. 创建gpu.launch操作
  auto launchOp = builder.create<gpu::LaunchOp>(
      loc, gridSize, blockSize);
  
  // 2. 在Kernel内部生成代码
  OpBuilder::InsertionGuard guard(builder);
  builder.setInsertionPointToStart(&launchOp.body().front());
  
  // 线程索引
  Value tidx = builder.create<gpu::ThreadIdOp>(loc, gpu::Dimension::x);
  Value bidx = builder.create<gpu::BlockIdOp>(loc, gpu::Dimension::x);
  
  // 计算全局索引
  Value idx = builder.create<arith::AddIOp>(loc,
      builder.create<arith::MulIOp>(loc, bidx, blockSize), tidx);
  
  // 卷积计算...
}
```

**实施计划**:
- Week 1-2: GPU Dialect集成和Kernel生成框架
- Week 3-4: 卷积/矩阵乘法Kernel优化
- Week 5-6: Shared Memory和Tensor Core优化
- Week 7-8: cuDNN集成和性能调优

#### 4.2 华为Ascend NPU支持（未实现）

**技术路线**:

```
MLIR (AI Dialect)
  ↓
Ascend Dialect (自定义)
  ↓ (Ascend Codegen)
Ascend IR (AIR)
  ↓ (ATC工具)
OM模型 (Offline Model)
  ↓ (ACL Runtime)
NPU执行
```

**CANN集成示例**:

```cpp
// ACL Runtime调用
class AscendRuntime {
public:
  void execute(const std::string& omPath,
               std::vector<Tensor*>& inputs,
               std::vector<Tensor*>& outputs) {
    // 1. 加载OM模型
    uint32_t modelId;
    aclmdlLoadFromFile(omPath.c_str(), &modelId);
    
    // 2. 创建输入Dataset
    auto inputDataset = createDataset(inputs);
    auto outputDataset = createDataset(outputs);
    
    // 3. 执行推理
    aclmdlExecute(modelId, inputDataset, outputDataset);
    
    // 4. 同步
    aclrtSynchronizeStream(stream_);
  }
};
```

**实施计划**:
- Week 1-2: CANN SDK集成和Ascend Dialect定义
- Week 3-4: 算子映射到TBE算子库
- Week 5-6: Ascend IR生成和OM模型转换
- Week 7-8: 性能优化和多核调度

---

## 📊 性能评估标准

### 编译时性能

| 指标 | 目标值 | 测量方法 |
|-----|-------|---------|
| 小模型编译时间 | <5s | ResNet-18 |
| 大模型编译时间 | <30s | ResNet-50 |
| Pass执行时间 | <1s | 单个Pass |
| 内存占用 | <2GB | 编译过程峰值 |

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
| 文档覆盖率 | 100% (所有公共API) |
| 编译警告 | 0 |
| Clang-tidy警告 | <10 |

---

## 🧪 测试策略（增强版）

### 单元测试框架

**使用GoogleTest**:

```cpp
// test/Dialect/AI/ConvOpTest.cpp

class Conv2DOpTest : public ::testing::Test {
protected:
  void SetUp() override {
    context = std::make_unique<MLIRContext>();
    context->loadDialect<ai::AIDialect>();
    builder = std::make_unique<OpBuilder>(context.get());
  }
  
  std::unique_ptr<MLIRContext> context;
  std::unique_ptr<OpBuilder> builder;
};

TEST_F(Conv2DOpTest, BasicConv) {
  // 创建输入类型
  auto inputType = RankedTensorType::get(
      {1, 3, 224, 224}, builder->getF32Type());
  auto weightType = RankedTensorType::get(
      {64, 3, 3, 3}, builder->getF32Type());
  
  // 创建Conv2D操作
  Value input = /* ... */;
  Value weight = /* ... */;
  
  auto convOp = builder->create<ai::Conv2DOp>(
      builder->getUnknownLoc(), input, weight,
      builder->getI64ArrayAttr({1, 1}),  // stride
      builder->getI64ArrayAttr({1, 1})); // padding
  
  // 验证
  ASSERT_TRUE(succeeded(convOp.verify()));
  EXPECT_EQ(convOp.getType().cast<RankedTensorType>().getShape()[1], 64);
}

TEST_F(Conv2DOpTest, InvalidStride) {
  // 测试错误情况
  auto convOp = createConvWithStride({1, 2, 3}); // 错误：3个元素
  EXPECT_TRUE(failed(convOp.verify()));
}
```

### 集成测试（FileCheck）

```mlir
// test/Integration/conv_fusion.mlir

// RUN: acompiler-opt %s -ai-conv-relu-fusion | FileCheck %s

module {
  // CHECK-LABEL: func @test_fusion
  func.func @test_fusion(%input: tensor<1x3x224x224xf32>,
                          %weight: tensor<64x3x3x3xf32>) 
                          -> tensor<1x64x224x224xf32> {
    // CHECK-NOT: ai.conv2d
    // CHECK-NOT: ai.relu
    // CHECK: ai.conv_relu
    %conv = ai.conv2d %input, %weight {
      stride = [1, 1], padding = [1, 1]
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) 
     -> tensor<1x64x224x224xf32>
    
    %relu = ai.relu %conv : tensor<1x64x224x224xf32>
    return %relu : tensor<1x64x224x224xf32>
  }
}
```

### 端到端测试

```python
# test/e2e/test_resnet18.py

import numpy as np
import onnx
from acompiler import Compiler, Runtime

def test_resnet18_inference():
    # 1. 加载ONNX模型
    model = onnx.load("models/resnet18.onnx")
    
    # 2. 编译
    compiler = Compiler()
    compiled = compiler.compile(model, target="cpu", opt_level=3)
    compiled.save("resnet18.so")
    
    # 3. 加载Runtime
    runtime = Runtime("cpu")
    model_handle = runtime.load_model("resnet18.so")
    
    # 4. 准备输入
    input_data = np.random.randn(1, 3, 224, 224).astype(np.float32)
    
    # 5. 执行推理
    output = runtime.execute(model_handle, [input_data])
    
    # 6. 验证结果
    assert output[0].shape == (1, 1000)
    assert np.all(np.isfinite(output[0]))
    
    # 7. 与ONNX Runtime对比
    import onnxruntime as ort
    sess = ort.InferenceSession("models/resnet18.onnx")
    ort_output = sess.run(None, {"input": input_data})
    
    # 允许小的数值误差
    np.testing.assert_allclose(output[0], ort_output[0], rtol=1e-3, atol=1e-5)
```

---

## 📚 学习资源（扩展版）

### 必读论文

1. **MLIR**: "MLIR: A Compiler Infrastructure for the End of Moore's Law" (CGO 2020)
2. **Polyhedral**: "Polyhedral Compilation as a Design Pattern for Compilers" (PLDI 2018)
3. **TVM**: "TVM: An Automated End-to-End Optimizing Compiler for Deep Learning" (OSDI 2018)
4. **XLA**: "XLA - TensorFlow, Compiled" (TensorFlow Dev Summit 2017)

### 推荐书籍

1. **编译原理**:
   - "Engineering a Compiler" (2nd Edition) - Cooper & Torczon
   - "Modern Compiler Implementation in C" - Andrew Appel
   
2. **LLVM/MLIR**:
   - "Getting Started with LLVM Core Libraries" - Bruno Cardoso Lopes
   - "LLVM Cookbook" - Mayur Pandey
   
3. **优化技术**:
   - "Optimizing Compilers for Modern Architectures" - Allen & Kennedy
   - "Computer Architecture: A Quantitative Approach" - Hennessy & Patterson

### 在线课程

1. **Stanford CS143**: Compilers
2. **MIT 6.172**: Performance Engineering
3. **Cornell CS6120**: Advanced Compilers

### 开源项目学习

1. **MLIR Examples**: https://github.com/llvm/llvm-project/tree/main/mlir/examples
2. **IREE**: https://github.com/iree-org/iree
3. **Torch-MLIR**: https://github.com/llvm/torch-mlir
4. **TVM**: https://github.com/apache/tvm
5. **ONNX-MLIR**: https://github.com/onnx/onnx-mlir

---

## 🎯 项目检查清单

### Phase 1: MLIR基础 ✓

- [ ] AI Dialect完整定义（10+算子）
- [ ] 所有算子都有验证逻辑
- [ ] 类型系统实现
- [ ] 属性系统实现
- [ ] 至少5个优化Pass
- [ ] 融合Pass工作正常
- [ ] ONNX导入器支持20+算子
- [ ] 单元测试覆盖率>70%
- [ ] 文档完整

### Phase 2: LLVM IR ✓

- [ ] 完整的降级流程
- [ ] AI → Linalg → SCF → CF → LLVM
- [ ] Runtime API设计完成
- [ ] 集成LLVM优化Pass
- [ ] 循环向量化工作
- [ ] SIMD指令生成
- [ ] 测试覆盖率>75%

### Phase 3: CodeGen ✓

- [ ] 端到端编译成功
- [ ] Runtime库完整实现
- [ ] 多线程支持
- [ ] 内存池管理
- [ ] 核心算子优化
- [ ] Conv2D性能达标
- [ ] ResNet-18推理成功
- [ ] 性能基准建立
- [ ] 测试覆盖率>80%

### Phase 4: 多后端 (TODO)

- [ ] CUDA后端规划完成
- [ ] Ascend后端规划完成
- [ ] 设计文档齐全

---

## 📈 项目进度跟踪

### 当前状态（2026-02-16）

| 模块 | 完成度 | 状态 |
|-----|-------|------|
| 项目结构 | 100% | ✅ 完成 |
| CMake配置 | 80% | 🚧 进行中 |
| AI Dialect | 60% | 🚧 进行中 |
| 优化Pass | 40% | 🚧 进行中 |
| 降级流程 | 20% | 📅 计划中 |
| Runtime | 10% | 📅 计划中 |
| 测试框架 | 30% | 🚧 进行中 |
| 文档 | 70% | 🚧 进行中 |

### 下一步行动

1. **立即**: 完成CMake配置，确保项目可编译
2. **本周**: 完善AI Dialect，添加更多算子
3. **下周**: 实现ConvReLU融合Pass并测试
4. **月内**: 完成ONNX导入器基础版本

---

## 🤝 贡献指南

### 代码规范

- 遵循[LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- 使用`clang-format`格式化代码
- 所有公共API都需要文档
- 每个PR都需要测试

### 提交流程

1. Fork仓库
2. 创建功能分支
3. 编写代码和测试
4. 运行`ninja check-acompiler`
5. 提交PR

### 问题报告

- 使用GitHub Issues
- 提供复现步骤
- 附上错误日志
- 说明环境信息

---

## 📝 变更日志

### v0.1.0 (2026-02-16)

- 🎉 初始项目结构
- ✨ AI Dialect基础定义
- ✨ ConvReLU融合Pass框架
- 📝 完整的项目规划文档
- 📝 学习教程和路线图

---

**最后更新**: 2026-02-16  
**文档版本**: 2.0（增强版）  
**维护者**: ACompiler Team

