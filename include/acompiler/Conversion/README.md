# Conversion 目录

本目录包含Dialect之间转换的Pass定义和实现。

## 转换路径

```
AI Dialect
    ↓ (AIToLinalg)
Linalg Dialect
    ↓ (LinalgToAffine)
Affine Dialect
    ↓ (AffineToSCF)
SCF Dialect
    ↓ (SCFToStd)
Std Dialect
    ↓ (StdToLLVM)
LLVM Dialect
```

## 转换Pass说明

### AIToLinalg

将高层AI算子降级为Linalg通用操作。

**示例转换**:
```mlir
// 输入 (AI Dialect)
%0 = ai.matmul %A, %B : tensor<MxK>, tensor<KxN> -> tensor<MxN>

// 输出 (Linalg Dialect)
%0 = linalg.matmul ins(%A, %B : tensor<MxK>, tensor<KxN>)
                   outs(%C : tensor<MxN>) -> tensor<MxN>
```

### LinalgToLLVM

将Linalg操作降级为LLVM Dialect。

**功能**:
- 循环展开
- 内存访问优化
- 类型转换（tensor -> memref -> llvm.ptr）

## 使用方法

```bash
# 单步转换
acompiler-opt --convert-ai-to-linalg input.mlir

# 完整降级
acompiler-opt --convert-ai-to-llvm input.mlir
```
