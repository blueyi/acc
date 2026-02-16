# MLIR基础教程

## 第1章：MLIR简介

### 什么是MLIR？

MLIR（Multi-Level Intermediate Representation）是一个可扩展的编译器基础设施，由LLVM项目开发。它提供了一个灵活的框架来构建领域特定的中间表示（IR）。

### MLIR的核心概念

#### 1. Operation（操作）

Operation是MLIR的基本单位，表示一个计算或控制流操作。

```mlir
%result = ai.conv2d %input, %weight : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) -> tensor<1x64x224x224xf32>
```

**组成部分**:
- `%result`: 结果值（SSA形式）
- `ai.conv2d`: 操作名称（dialect.operation）
- `%input, %weight`: 操作数
- 类型签名

#### 2. Block（基本块）

Block包含一系列顺序执行的操作，类似于LLVM的基本块。

```mlir
^bb0(%arg0: i32):
  %0 = arith.addi %arg0, %arg0 : i32
  cf.br ^bb1(%0 : i32)

^bb1(%arg1: i32):
  func.return %arg1 : i32
```

#### 3. Region（区域）

Region包含一个或多个Block，用于表示作用域。

```mlir
scf.if %cond {
  // Region 1 (then branch)
  %0 = ...
} else {
  // Region 2 (else branch)
  %1 = ...
}
```

#### 4. Dialect（方言）

Dialect是一组相关操作、类型和属性的命名空间。

**标准Dialect**:
- `arith`: 算术操作
- `func`: 函数操作
- `scf`: 结构化控制流
- `tensor`: 张量操作

**ACompiler自定义Dialect**:
- `ai`: AI算子

### SSA形式

MLIR使用SSA（Static Single Assignment）形式：
- 每个值只被定义一次
- 值使用前必须先定义
- 使用`%`前缀标识值

```mlir
%0 = arith.constant 1 : i32
%1 = arith.constant 2 : i32
%2 = arith.addi %0, %1 : i32  // %2 = %0 + %1
```

### 类型系统

MLIR有丰富的类型系统：

**内置类型**:
- `i32`, `i64`: 整数类型
- `f32`, `f64`: 浮点类型
- `tensor<1x3x224x224xf32>`: 张量类型
- `memref<10xf32>`: 内存引用类型

**自定义类型**: Dialect可以定义自己的类型

### 属性（Attributes）

属性是编译时常量，用于配置操作：

```mlir
%output = ai.conv2d %input, %weight {
  stride = array<i64: 1, 1>,
  padding = array<i64: 1, 1>
} : ...
```

## 第2章：MLIR工具链

### mlir-opt

优化和转换IR的工具：

```bash
# 打印IR
mlir-opt input.mlir

# 运行Pass
mlir-opt input.mlir -pass-name

# 降级
mlir-opt input.mlir --convert-scf-to-cf
```

### mlir-translate

转换不同IR格式：

```bash
# MLIR -> LLVM IR
mlir-translate input.mlir --mlir-to-llvmir -o output.ll
```

## 第3章：编写第一个MLIR程序

### 示例：简单的加法函数

```mlir
module {
  func.func @add(%arg0: i32, %arg1: i32) -> i32 {
    %result = arith.addi %arg0, %arg1 : i32
    return %result : i32
  }
}
```

### 示例：使用AI Dialect

```mlir
module {
  func.func @simple_conv(%input: tensor<1x3x224x224xf32>,
                          %weight: tensor<64x3x3x3xf32>) 
                          -> tensor<1x64x224x224xf32> {
    %output = ai.conv2d %input, %weight {
      stride = array<i64: 1, 1>,
      padding = array<i64: 1, 1>
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) 
     -> tensor<1x64x224x224xf32>
    
    return %output : tensor<1x64x224x224xf32>
  }
}
```

## 第4章：理解IR结构

### 层次结构

```
ModuleOp
  └─ FuncOp
      └─ Block
          ├─ Operation 1
          ├─ Operation 2
          └─ ReturnOp
```

### 遍历IR

在C++中遍历IR：

```cpp
void walkOperations(ModuleOp module) {
  module.walk([](Operation *op) {
    llvm::outs() << "Operation: " << op->getName() << "\n";
  });
}
```

## 实践练习

### 练习1：编写简单的MLIR函数

编写一个计算 `(a + b) * c` 的函数。

<details>
<summary>答案</summary>

```mlir
module {
  func.func @compute(%a: i32, %b: i32, %c: i32) -> i32 {
    %sum = arith.addi %a, %b : i32
    %result = arith.muli %sum, %c : i32
    return %result : i32
  }
}
```
</details>

### 练习2：使用AI Dialect

编写一个包含Conv2D和ReLU的函数。

<details>
<summary>答案</summary>

```mlir
module {
  func.func @conv_relu(%input: tensor<1x3x224x224xf32>,
                        %weight: tensor<64x3x3x3xf32>) 
                        -> tensor<1x64x224x224xf32> {
    %conv = ai.conv2d %input, %weight {
      stride = array<i64: 1, 1>,
      padding = array<i64: 1, 1>
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) 
     -> tensor<1x64x224x224xf32>
    
    %relu = ai.relu %conv : tensor<1x64x224x224xf32>
    
    return %relu : tensor<1x64x224x224xf32>
  }
}
```
</details>

## 下一步

- [自定义Dialect](02_custom_dialect.md)
- [Pass开发](03_pass_development.md)

## 参考资源

- [MLIR Language Reference](https://mlir.llvm.org/docs/LangRef/)
- [MLIR Dialects](https://mlir.llvm.org/docs/Dialects/)
- [MLIR Tutorials](https://mlir.llvm.org/docs/Tutorials/)
