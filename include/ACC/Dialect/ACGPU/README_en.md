# ACGPU Dialect — TODO

## Overview

The ACGPU Dialect will express GPU computation-specific concepts, including:

- Grid / Block / Thread hierarchy
- Shared Memory allocation and access
- Warp-level primitives (Shuffle, Reduce)
- Tensor Core operation mapping

## Design Direction

```
ACMid Dialect
    │
    ▼ (ACMidToGPU Lowering)
ACGPU Dialect   ← Define GPU-specific high-level operations
    │
    ▼ (ACGPUToNVVM Lowering)
MLIR GPU Dialect + NVVM Dialect
    │
    ▼ (LLVM NVPTX Backend)
PTX Code → cubin
```

## Status

**Not yet implemented** — Waiting for Phase 4 to begin.
