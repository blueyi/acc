# 教程 7: 目标代码生成

## 目标

学习 LLVM 的代码生成框架，理解指令选择、寄存器分配和指令调度。

## 核心概念

### 代码生成流程
```
LLVM IR → SelectionDAG → MachineInstr → MCInst → 目标代码
```

### 关键步骤
1. **指令选择 (Instruction Selection)**: LLVM IR → 目标指令
2. **寄存器分配 (Register Allocation)**: 虚拟寄存器 → 物理寄存器
3. **指令调度 (Instruction Scheduling)**: 优化指令执行顺序
4. **MC Layer**: 生成最终的机器码

### 多后端支持
- x86-64: SSE/AVX/AVX-512
- AArch64: NEON/SVE

## 练习

1. 使用 `llc` 查看 LLVM IR 生成的汇编代码
2. 比较不同优化级别 (-O0, -O2, -O3) 的代码生成差异
3. 分析 SIMD 向量化指令的生成
