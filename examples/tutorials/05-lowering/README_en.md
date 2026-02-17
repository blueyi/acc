# Tutorial 5: Implementing MLIR Lowering

## Goal

Learn MLIR's multi-level lowering mechanism and implement the complete lowering pipeline from ACHigh to LLVM Dialect.

## Core Concepts

### DialectConversion
- `ConversionTarget`: Define legal/illegal Dialect/Op
- `TypeConverter`: Type conversion rules
- `ConversionPattern`: Rewrite patterns with type conversion

### Bufferization
- Tensor → MemRef conversion
- One-shot bufferization framework

### Lowering Pipeline
```
ACHigh → ACMid → Linalg → Affine/SCF → CF → LLVM Dialect
```

## Exercises

1. Implement `achigh.matmul → acmid.tiled_matmul` lowering
2. Implement `acmid.tiled_matmul → linalg.matmul` lowering
3. Use MLIR built-in passes to complete `Linalg → LLVM Dialect` lowering
