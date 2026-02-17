# Tutorial 6: LLVM IR Optimization

## Goal

Learn the LLVM IR optimization framework and write custom LLVM IR optimization passes.

## Core Concepts

### LLVM IR Basics
- SSA (Static Single Assignment) form
- Basic Blocks and Control Flow Graph (CFG)
- PHI nodes
- LLVM built-in type system

### New Pass Manager (NPM)
- `PassInfoMixin` base class
- `PreservedAnalyses` return value
- Analysis Pass vs. Transform Pass

### Common Analyses
- `DominatorTree`: Dominator tree
- `LoopInfo`: Loop information
- `AliasAnalysis`: Alias analysis
- `ScalarEvolution`: Scalar evolution

## Exercises

1. Write an Analysis Pass that counts basic blocks in a function
2. Write a simple dead code elimination Pass
3. Write a loop unrolling Pass (using LoopInfo)
