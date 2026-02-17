# Tutorial 7: Target Code Generation

## Goal

Learn LLVM's code generation framework, including instruction selection, register allocation, and instruction scheduling.

## Core Concepts

### Code Generation Pipeline
```
LLVM IR → SelectionDAG → MachineInstr → MCInst → Target Code
```

### Key Steps
1. **Instruction Selection (ISel)**: LLVM IR → Target instructions
2. **Register Allocation (RegAlloc)**: Virtual registers → Physical registers
3. **Instruction Scheduling**: Optimize instruction execution order
4. **MC Layer**: Generate final machine code

### Multi-Backend Support
- x86-64: SSE/AVX/AVX-512
- AArch64: NEON/SVE

## Exercises

1. Use `llc` to view assembly code generated from LLVM IR
2. Compare code generation across optimization levels (-O0, -O2, -O3)
3. Analyze SIMD vectorization instruction generation
