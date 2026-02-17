# ACGPU Dialect — TODO

## 概述

ACGPU Dialect 将用于表达 GPU 计算特有的概念，包括：

- Grid / Block / Thread 层次结构
- Shared Memory 分配和访问
- Warp-level 原语（Shuffle、Reduce）
- Tensor Core 操作映射

## 设计方向

```
ACMid Dialect
    │
    ▼ (ACMidToGPU Lowering)
ACGPU Dialect   ← 定义 GPU 特有的高层操作
    │
    ▼ (ACGPUToNVVM Lowering)
MLIR GPU Dialect + NVVM Dialect
    │
    ▼ (LLVM NVPTX Backend)
PTX Code → cubin
```

## 状态

**尚未实现** — 等待 Phase 2 启动。
