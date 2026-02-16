# 教程 5: 实现 MLIR Lowering

## 目标

学习 MLIR 的多层 Lowering 机制，实现从 ACHigh 到 LLVM Dialect 的完整 Lowering 链路。

## 核心概念

### DialectConversion
- `ConversionTarget`: 定义合法/非法的 Dialect/Op
- `TypeConverter`: 类型转换规则
- `ConversionPattern`: 带类型转换的重写模式

### Bufferization
- Tensor → MemRef 的转换
- One-shot bufferization 框架

### Lowering 链路
```
ACHigh → ACMid → Linalg → Affine/SCF → CF → LLVM Dialect
```

## 练习

1. 实现 `achigh.matmul → acmid.tiled_matmul` Lowering
2. 实现 `acmid.tiled_matmul → linalg.matmul` Lowering
3. 使用 MLIR 内置 Pass 完成 `Linalg → LLVM Dialect` Lowering
