# CUDA Runtime（TODO）

**状态**: 规划中，未实现

## 概述

CUDA Runtime为ACompiler提供NVIDIA GPU后端支持，实现高性能的AI模型推理。

## 规划特性

### 1. 基础架构
- [ ] CUDA Runtime API封装
- [ ] GPU内存管理
- [ ] 流管理和异步执行
- [ ] 多GPU支持

### 2. Kernel库
- [ ] 卷积Kernel（im2col + GEMM）
- [ ] 矩阵乘法Kernel（Tensor Core优化）
- [ ] 激活函数Kernel（融合优化）
- [ ] 池化Kernel
- [ ] 归一化Kernel

### 3. 性能优化
- [ ] Kernel融合
- [ ] 共享内存优化
- [ ] Bank Conflict消除
- [ ] Warp级优化
- [ ] Tensor Core支持（FP16/TF32）
- [ ] Grid/Block自动配置

### 4. 库集成
- [ ] cuDNN集成（可选）
- [ ] cuBLAS集成（可选）
- [ ] CUTLASS库支持

## 技术架构

```
应用程序
    ↓
CUDA Runtime API
    ↓
┌─────────────┬──────────────┬─────────────┐
│ 内存管理     │ Kernel调度    │ 流管理       │
└─────────────┴──────────────┴─────────────┘
    ↓             ↓              ↓
┌─────────────┬──────────────┬─────────────┐
│ 内存池       │ Kernel库      │ 异步执行     │
└─────────────┴──────────────┴─────────────┘
    ↓
CUDA Driver
    ↓
NVIDIA GPU
```

## 代码生成流程

```
MLIR (AI Dialect)
    ↓
Linalg Dialect
    ↓
GPU Dialect (MLIR)
    ↓
NVVM Dialect
    ↓
PTX Assembly
    ↓
CUDA Binary (cubin)
```

## 实现计划

### Phase 1: 基础框架（2-3周）
1. CUDA Runtime初始化和销毁
2. GPU内存分配和释放
3. 主机-设备数据传输
4. 基础Kernel调用框架

### Phase 2: Kernel库（3-4周）
1. 实现基础算子Kernel
2. 性能优化（共享内存、Bank Conflict）
3. Kernel融合支持
4. 自动调优框架

### Phase 3: 高级特性（2-3周）
1. Tensor Core支持
2. 多流并发执行
3. 内存池优化
4. Profiling和调试工具

### Phase 4: 库集成（1-2周）
1. cuDNN集成
2. cuBLAS集成
3. 性能对比测试

## Kernel示例

### Conv2D Kernel

```cuda
// conv2d_kernel.cu
__global__ void conv2d_kernel(
    const float* __restrict__ input,   // [N, C, H, W]
    const float* __restrict__ weight,  // [K, C, R, S]
    float* __restrict__ output,        // [N, K, H', W']
    int N, int C, int H, int W,
    int K, int R, int S,
    int stride, int padding) {
  
  // 使用共享内存优化
  __shared__ float s_input[TILE_SIZE];
  __shared__ float s_weight[TILE_SIZE];
  
  // Kernel实现
  // ...
}
```

### MatMul Kernel（Tensor Core）

```cuda
// matmul_tensor_core.cu
#include <mma.h>

__global__ void matmul_tensor_core(
    const half* __restrict__ A,  // [M, K]
    const half* __restrict__ B,  // [K, N]
    float* __restrict__ C,       // [M, N]
    int M, int N, int K) {
  
  using namespace nvcuda::wmma;
  
  // 声明fragment
  fragment<matrix_a, 16, 16, 16, half, row_major> a_frag;
  fragment<matrix_b, 16, 16, 16, half, col_major> b_frag;
  fragment<accumulator, 16, 16, 16, float> c_frag;
  
  // 使用Tensor Core计算
  fill_fragment(c_frag, 0.0f);
  load_matrix_sync(a_frag, A, K);
  load_matrix_sync(b_frag, B, N);
  mma_sync(c_frag, a_frag, b_frag, c_frag);
  store_matrix_sync(C, c_frag, N, mem_row_major);
}
```

## Runtime API设计

```c
// CUDA Runtime API

// 初始化
acompiler_runtime_t* acompiler_create_cuda_runtime(int device_id);
void acompiler_destroy_cuda_runtime(acompiler_runtime_t* runtime);

// 内存管理
acompiler_tensor_t* acompiler_cuda_alloc_tensor(
    acompiler_runtime_t* runtime,
    const int* shape, int ndim, acompiler_dtype_t dtype);
void acompiler_cuda_free_tensor(acompiler_tensor_t* tensor);

// 数据传输
void acompiler_cuda_memcpy_h2d(void* dst, const void* src, size_t size);
void acompiler_cuda_memcpy_d2h(void* dst, const void* src, size_t size);
void acompiler_cuda_memcpy_d2d(void* dst, const void* src, size_t size);

// 执行
void acompiler_cuda_execute(
    acompiler_runtime_t* runtime,
    const char* model_path,
    acompiler_tensor_t** inputs, int num_inputs,
    acompiler_tensor_t** outputs, int num_outputs);

// 异步执行
acompiler_stream_t* acompiler_cuda_create_stream(acompiler_runtime_t* runtime);
void acompiler_cuda_destroy_stream(acompiler_stream_t* stream);
void acompiler_cuda_execute_async(
    acompiler_runtime_t* runtime,
    const char* model_path,
    acompiler_tensor_t** inputs, int num_inputs,
    acompiler_tensor_t** outputs, int num_outputs,
    acompiler_stream_t* stream);
void acompiler_cuda_synchronize_stream(acompiler_stream_t* stream);
```

## 性能目标

- **卷积**: 达到cuDNN性能的80%以上
- **矩阵乘法**: 充分利用Tensor Core，达到理论峰值的70%以上
- **端到端**: 与TensorRT性能差距在20%以内

## 测试计划

### 功能测试
- [ ] 算子正确性测试
- [ ] 内存管理测试
- [ ] 多流并发测试
- [ ] 错误处理测试

### 性能测试
- [ ] 单算子性能基准
- [ ] 端到端模型性能
- [ ] 与cuDNN/TensorRT对比
- [ ] 不同GPU架构测试（V100/A100/H100）

## 依赖要求

- CUDA Toolkit 11.0+
- 计算能力 7.0+（Tensor Core需要7.0+）
- CMake 3.18+（CUDA支持）
- 可选：cuDNN 8.0+, cuBLAS

## 参考资源

- [CUDA C++ Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)
- [CUTLASS - CUDA Templates for Linear Algebra Subroutines](https://github.com/NVIDIA/cutlass)
- [TensorRT](https://developer.nvidia.com/tensorrt)
- [MLIR GPU Dialect](https://mlir.llvm.org/docs/Dialects/GPU/)

---

**注意**: 本目录内容为规划文档，实际实现将在第四阶段进行。
