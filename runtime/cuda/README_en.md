# CUDA Runtime — TODO

## Overview

This module will implement NVIDIA GPU CUDA Runtime support for executing ACC-compiled code on GPUs.

## Planned Features

- CUDA memory management (Device Memory Allocation/Free/Copy)
- CUDA Kernel launch wrapper
- CUDA Stream management
- cuBLAS/cuDNN integration (optional, for performance comparison)
- Tensor Core WMMA operation support

## Dependencies

- CUDA Toolkit >= 11.0
- NVIDIA GPU (Compute Capability >= 7.0)

## Status

**Not yet implemented** — Development will start after CPU backend is stable.
