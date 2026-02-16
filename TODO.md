# ACompiler 扩展后端 TODO

本文件记录 ACompiler 项目的扩展计划。第一阶段仅实现 CPU 后端，以下后端暂时不实现。

---

## 1. NVIDIA GPU - CUDA 支持

**优先级**：第二阶段  
**状态**：规划中，暂不实现

### 计划内容

- Lowering 到 MLIR GPU Dialect / NVVM Dialect
- Kernel 发射与内存管理（Host/Device 分离）
- 与 CUDA Driver/Runtime API 集成
- 支持典型 AI 算子（MatMul、Conv 等）的 GPU 代码生成

### 技术参考

- MLIR GPU Dialect
- NVVM IR
- cuBLAS / cuDNN 等库集成

---

## 2. 华为 Ascend NPU - Ascend IR 支持

**优先级**：第二阶段  
**状态**：规划中，暂不实现

### 计划内容

- 定义 Ascend 专用 Dialect 或映射到现有表示
- 将 MLIR 图映射到 CANN (Compute Architecture for Neural Networks) 图
- 支持 Ascend 算子库调用
- 与 Ascend 运行时集成

### 技术参考

- 华为 CANN 文档
- Ascend C / TBE 算子开发
- Ascend IR 规范

---

## 目录预留

在 `lib/Target/` 下已预留子目录：

- `lib/Target/GPU/` - 用于 NVIDIA CUDA 后端
- `lib/Target/NPU/` - 用于华为 Ascend NPU 后端

第一阶段这些目录保持为空或仅含占位文件。
