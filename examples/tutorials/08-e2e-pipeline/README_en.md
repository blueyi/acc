# Tutorial 8: End-to-End Compilation Pipeline

## Goal

Connect all modules together to implement the complete flow from model input to inference execution.

## Complete Pipeline

```
Input (DSL/ONNX)
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
Executable
    │
    ▼ Run Inference
Inference Result
```

## Exercises

1. Compile and run a simple matrix multiplication model
2. Compare inference performance across optimization levels
3. Use JIT mode for rapid model testing
