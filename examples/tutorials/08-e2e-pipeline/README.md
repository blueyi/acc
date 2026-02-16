# 教程 8: 端到端编译流水线

## 目标

将所有模块串联起来，实现从模型输入到推理执行的完整流程。

## 完整流程

```
输入 (DSL/ONNX)
    │
    ▼
Frontend (Lexer → Parser → AST → Sema → MLIRGen)
    │
    ▼
MLIR (ACHigh Dialect)
    │
    ▼ Shape Inference → Constant Folding → Op Fusion → Layout Transform
MLIR Optimized (ACHigh)
    │
    ▼ ACHigh → ACMid Lowering
MLIR (ACMid Dialect)
    │
    ▼ ACMid → Linalg → Affine/SCF → LLVM Dialect
MLIR (LLVM Dialect)
    │
    ▼ mlir-translate (LLVM Dialect → LLVM IR)
LLVM IR
    │
    ▼ Custom Passes + Standard LLVM Optimization
Optimized LLVM IR
    │
    ▼ LLVM CodeGen (Instruction Selection → RegAlloc → Scheduling)
Object File / Executable
    │
    ▼ Link with ACRuntime
可执行文件
    │
    ▼ 运行推理
推理结果
```

## 练习

1. 编译一个简单的矩阵乘法模型并运行
2. 对比不同优化级别的推理性能
3. 使用 JIT 模式快速测试模型
