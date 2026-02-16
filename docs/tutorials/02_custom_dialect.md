# 自定义Dialect开发

## 概述

本教程介绍如何在ACompiler中创建自定义MLIR Dialect。我们将以AI Dialect为例。

## Dialect结构

一个完整的Dialect包括：
1. **Dialect定义**: 命名空间和基本信息
2. **Operations**: 操作定义
3. **Types**: 类型定义
4. **Attributes**: 属性定义

## 步骤1：定义Dialect

### 使用TableGen定义

创建`AI.td`文件：

```tablegen
#ifndef AI_DIALECT
#define AI_DIALECT

include "mlir/IR/OpBase.td"

def AI_Dialect : Dialect {
  let name = "ai";
  let summary = "AI operations dialect";
  let description = [{
    Dialect for AI/ML operations.
  }];
  let cppNamespace = "::mlir::ai";
}

#endif
```

### C++头文件

`AIDialect.h`:

```cpp
#ifndef ACOMPILER_DIALECT_AI_AIDIALECT_H
#define ACOMPILER_DIALECT_AI_AIDIALECT_H

#include "mlir/IR/Dialect.h"

namespace mlir {
namespace ai {

#include "acompiler/Dialect/AI/AIDialect.h.inc"

} // namespace ai
} // namespace mlir

#endif
```

### C++实现

`AIDialect.cpp`:

```cpp
#include "acompiler/Dialect/AI/AIDialect.h"
#include "acompiler/Dialect/AI/AIOps.h"

using namespace mlir;
using namespace mlir::ai;

#include "acompiler/Dialect/AI/AIDialect.cpp.inc"

void AIDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "acompiler/Dialect/AI/AIOps.cpp.inc"
      >();
}
```

## 步骤2：定义Operations

### TableGen定义

`AIOps.td`:

```tablegen
def AI_Conv2DOp : AI_Op<"conv2d", [Pure]> {
  let summary = "2D convolution operation";
  
  let description = [{
    Performs 2D convolution.
    Example:
    ```mlir
    %output = ai.conv2d %input, %weight {stride = [1, 1]} 
      : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) 
      -> tensor<1x64x224x224xf32>
    ```
  }];
  
  let arguments = (ins
    AnyTensor:$input,
    AnyTensor:$weight,
    OptionalAttr<I64ArrayAttr>:$stride,
    OptionalAttr<I64ArrayAttr>:$padding
  );
  
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $input `,` $weight attr-dict `:` 
    `(` type($input) `,` type($weight) `)` `->` type($output)
  }];
}
```

### Operation特性（Traits）

常用Traits：
- `Pure`: 无副作用
- `SameOperandsAndResultType`: 操作数和结果类型相同
- `Commutative`: 可交换
- `Idempotent`: 幂等

### 自定义验证

在`AIOps.cpp`中添加验证逻辑：

```cpp
LogicalResult Conv2DOp::verify() {
  // 验证stride属性
  if (auto stride = getStride()) {
    if (stride->size() != 2) {
      return emitOpError("stride must have 2 elements");
    }
  }
  
  // 验证输入输出形状
  auto inputType = getInput().getType().cast<RankedTensorType>();
  auto outputType = getOutput().getType().cast<RankedTensorType>();
  
  if (inputType.getRank() != 4 || outputType.getRank() != 4) {
    return emitOpError("input and output must be 4D tensors");
  }
  
  return success();
}
```

## 步骤3：定义自定义类型

### TableGen定义

```tablegen
def AI_TensorType : AI_Type<"Tensor", "tensor"> {
  let summary = "AI tensor type";
  
  let description = [{
    Represents a multi-dimensional tensor with shape and element type.
  }];
  
  let parameters = (ins
    ArrayRefParameter<"int64_t">:$shape,
    "Type":$elementType
  );
  
  let assemblyFormat = [{
    `<` $shape `,` $elementType `>`
  }];
}
```

### 使用自定义类型

```mlir
%tensor = ... : !ai.tensor<[1, 3, 224, 224], f32>
```

## 步骤4：定义自定义属性

### TableGen定义

```tablegen
def AI_ConvParamsAttr : AI_Attr<"ConvParams", "conv_params"> {
  let summary = "Convolution parameters";
  
  let parameters = (ins
    "ArrayRef<int64_t>":$stride,
    "ArrayRef<int64_t>":$padding,
    "ArrayRef<int64_t>":$dilation
  );
  
  let assemblyFormat = [{
    `<` $stride `,` $padding `,` $dilation `>`
  }];
}
```

### 使用自定义属性

```mlir
%output = ai.conv2d %input, %weight {
  params = #ai.conv_params<[1, 1], [1, 1], [1, 1]>
} : ...
```

## 步骤5：CMake配置

### include/acompiler/Dialect/AI/CMakeLists.txt

```cmake
set(LLVM_TARGET_DEFINITIONS AI.td)

mlir_tablegen(AIDialect.h.inc -gen-dialect-decls -dialect=ai)
mlir_tablegen(AIDialect.cpp.inc -gen-dialect-defs -dialect=ai)
mlir_tablegen(AIOps.h.inc -gen-op-decls)
mlir_tablegen(AIOps.cpp.inc -gen-op-defs)

add_public_tablegen_target(MLIRAIIncGen)
```

### lib/Dialect/AI/CMakeLists.txt

```cmake
add_mlir_dialect_library(ACompilerAIDialect
  AIDialect.cpp
  AIOps.cpp
  
  DEPENDS
  MLIRAIIncGen
  
  LINK_LIBS PUBLIC
  MLIRIR
  )
```

## 步骤6：注册Dialect

在工具中注册Dialect（如`acompiler-opt`）：

```cpp
#include "acompiler/Dialect/AI/AIDialect.h"

int main(int argc, char **argv) {
  DialectRegistry registry;
  registry.insert<ai::AIDialect>();
  
  // ...
}
```

## Operation接口

### InferTypeOpInterface

自动推导输出类型：

```cpp
LogicalResult Conv2DOp::inferReturnTypes(
    MLIRContext *context,
    Optional<Location> location,
    ValueRange operands,
    DictionaryAttr attributes,
    RegionRange regions,
    SmallVectorImpl<Type> &inferredReturnTypes) {
  // 根据输入推导输出类型
  auto inputType = operands[0].getType().cast<RankedTensorType>();
  // ... 计算输出形状 ...
  inferredReturnTypes.push_back(outputType);
  return success();
}
```

### FoldOpInterface

常量折叠：

```cpp
OpFoldResult ReLUOp::fold(ArrayRef<Attribute> operands) {
  // 如果输入是常量，可以在编译时计算
  if (auto inputAttr = operands[0].dyn_cast_or_null<DenseElementsAttr>()) {
    // 执行ReLU计算
    return computeReLU(inputAttr);
  }
  return {};
}
```

## 测试Dialect

### 编写测试用例

`test/Dialect/AI/ops.mlir`:

```mlir
// RUN: acompiler-opt %s | FileCheck %s

// CHECK-LABEL: func @test_conv2d
func.func @test_conv2d(%input: tensor<1x3x224x224xf32>,
                        %weight: tensor<64x3x3x3xf32>) 
                        -> tensor<1x64x224x224xf32> {
  // CHECK: ai.conv2d
  %output = ai.conv2d %input, %weight {
    stride = array<i64: 1, 1>,
    padding = array<i64: 1, 1>
  } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) 
   -> tensor<1x64x224x224xf32>
  
  return %output : tensor<1x64x224x224xf32>
}
```

### 运行测试

```bash
acompiler-opt test/Dialect/AI/ops.mlir | FileCheck test/Dialect/AI/ops.mlir
```

## 最佳实践

1. **命名规范**: 使用清晰的操作名称
2. **文档完善**: 在TableGen中添加详细的description
3. **验证充分**: 实现verify()方法检查操作合法性
4. **测试覆盖**: 为每个操作编写测试用例
5. **接口实现**: 实现常用接口（InferType, Fold等）

## 实践练习

### 练习：添加新操作

为AI Dialect添加一个Sigmoid激活函数操作。

要求：
1. 在`AIOps.td`中定义`AI_SigmoidOp`
2. 添加适当的Traits
3. 实现验证逻辑
4. 编写测试用例

## 下一步

- [Pass开发](03_pass_development.md)
- [Dialect降级](04_dialect_lowering.md)

## 参考资源

- [Defining Dialects](https://mlir.llvm.org/docs/DefiningDialects/)
- [Operation Definition Specification](https://mlir.llvm.org/docs/DefiningDialects/Operations/)
- [TableGen Programmer's Reference](https://llvm.org/docs/TableGen/ProgRef.html)
