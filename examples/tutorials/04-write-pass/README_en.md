# Tutorial 4: Writing MLIR Optimization Passes

## Goal

Learn the MLIR Pass framework and write constant folding and operator fusion optimization passes.

## Core Concepts

### Pass Management Framework
- `PassManager` / `OpPassManager`
- `FunctionPass` / `OperationPass`
- Pass registration and CLI integration

### Pattern Rewriting
- `RewritePattern` base class
- `matchAndRewrite` method
- `GreedyPatternRewriteDriver`

### DRR (Declarative Rewrite Rules)
- Define pattern replacements declaratively using TableGen
- Auto-generate C++ match/rewrite code

## Exercises

1. Implement a simple constant folding Pass (fold `achigh.add(constant, constant)`)
2. Use DRR to implement `ReLU(ReLU(x)) → ReLU(x)` simplification
3. Implement `Conv2D + ReLU → FusedConvReLU` fusion
