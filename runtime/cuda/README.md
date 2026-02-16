# CUDA Runtime — TODO

## 概述

此模块将实现 NVIDIA GPU 的 CUDA Runtime 支持，用于在 GPU 上执行 ACompiler 编译后的代码。

## 计划包含

- CUDA 内存管理（Device Memory Allocation/Free/Copy）
- CUDA Kernel 启动封装
- CUDA Stream 管理
- cuBLAS/cuDNN 集成（可选，用于对比性能）
- Tensor Core WMMA 操作支持

## 依赖

- CUDA Toolkit >= 11.0
- NVIDIA GPU (Compute Capability >= 7.0)

## 状态

**尚未实现** — 等待 CPU 后端稳定后启动开发。
