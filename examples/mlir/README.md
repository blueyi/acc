# MLIR示例

本目录包含ACompiler的MLIR示例代码。

## 示例文件

### simple_ops.mlir
基础AI算子的使用示例，包括：
- Conv2D卷积操作
- MatMul矩阵乘法
- ReLU激活函数
- MaxPool2D池化操作

**运行方式**:
```bash
acompiler-opt simple_ops.mlir
```

### fusion.mlir
演示算子融合的示例，展示Conv2D和ReLU如何被融合成ConvReLU操作。

**运行方式**:
```bash
# 运行融合Pass
acompiler-opt fusion.mlir -ai-conv-relu-fusion

# 查看融合前后的差异
acompiler-opt fusion.mlir | diff - <(acompiler-opt fusion.mlir -ai-conv-relu-fusion)
```

### lowering.mlir（TODO）
演示Dialect降级过程的示例，展示AI Dialect如何逐步降级到LLVM Dialect。

**计划展示的降级路径**:
```
AI Dialect → Linalg → Affine → SCF → LLVM Dialect
```

## MLIR语法说明

### 基本结构

```mlir
module {
  func.func @function_name(%arg0: type, %arg1: type) -> result_type {
    %result = operation %arg0, %arg1 : type
    return %result : type
  }
}
```

### AI Dialect操作格式

#### Conv2D
```mlir
%output = ai.conv2d %input, %weight {
  stride = array<i64: s_h, s_w>,
  padding = array<i64: p_h, p_w>
} : (tensor<NxCxHxWxf32>, tensor<KxCxRxSxf32>) -> tensor<NxKxH'xW'xf32>
```

#### MatMul
```mlir
%C = ai.matmul %A, %B : (tensor<MxKxf32>, tensor<KxNxf32>) -> tensor<MxNxf32>
```

#### ReLU
```mlir
%output = ai.relu %input : tensor<*xf32>
```

#### ConvReLU（融合操作）
```mlir
%output = ai.conv_relu %input, %weight {
  stride = array<i64: 1, 1>,
  padding = array<i64: 1, 1>
} : (tensor<NxCxHxWxf32>, tensor<KxCxRxSxf32>) -> tensor<NxKxH'xW'xf32>
```

## 运行测试

```bash
# 使用FileCheck验证输出
acompiler-opt simple_ops.mlir | FileCheck simple_ops.mlir

# 运行所有示例
for file in *.mlir; do
  echo "Testing $file..."
  acompiler-opt "$file"
done
```

## 调试技巧

### 打印IR
```bash
# 打印原始IR
acompiler-opt simple_ops.mlir -mlir-print-ir-before-all

# 打印Pass执行后的IR
acompiler-opt simple_ops.mlir -ai-conv-relu-fusion -mlir-print-ir-after-all
```

### 查看Pass执行顺序
```bash
acompiler-opt simple_ops.mlir -ai-conv-relu-fusion -pass-pipeline='builtin.module(func.func(ai-conv-relu-fusion))' -mlir-print-ir-after-all
```

### 验证IR
```bash
acompiler-opt simple_ops.mlir -verify-diagnostics
```

## 学习资源

- [MLIR Language Reference](https://mlir.llvm.org/docs/LangRef/)
- [MLIR Toy Tutorial](https://mlir.llvm.org/docs/Tutorials/Toy/)
- [Operation Definition Specification](https://mlir.llvm.org/docs/DefiningDialects/Operations/)
