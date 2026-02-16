# Runtime 目录

本目录包含ACompiler的运行时库，为编译后的模型提供执行支持。

## Runtime架构

```
应用程序
    ↓
Runtime API (统一接口)
    ↓
后端Runtime (CPU/CUDA/Ascend)
    ↓
算子库 (Op Kernels)
    ↓
硬件 (CPU/GPU/NPU)
```

## 后端Runtime

### CPU Runtime（已实现）

**功能**:
- 张量内存管理
- 算子执行
- 多线程调度
- SIMD优化

**依赖**:
- OpenMP/TBB（并行计算）
- 可选：MKL/OpenBLAS（矩阵运算）

**API示例**:
```c
// 创建Runtime
acompiler_runtime_t* runtime = acompiler_create_cpu_runtime();

// 分配张量
acompiler_tensor_t* tensor = acompiler_alloc_tensor(runtime, shape, dtype);

// 执行推理
acompiler_execute(runtime, model, inputs, outputs);

// 释放资源
acompiler_free_tensor(tensor);
acompiler_destroy_runtime(runtime);
```

### CUDA Runtime（TODO）

**规划功能**:
- GPU内存管理
- Kernel执行
- 异步执行
- 流管理
- 内存池

**计划依赖**:
- CUDA Toolkit
- cuDNN（可选）
- cuBLAS（可选）

**API示例**:
```c
// 创建CUDA Runtime
acompiler_runtime_t* runtime = acompiler_create_cuda_runtime(device_id);

// 分配GPU内存
acompiler_tensor_t* tensor = acompiler_alloc_tensor(runtime, shape, dtype);

// 异步执行
acompiler_stream_t* stream = acompiler_create_stream(runtime);
acompiler_execute_async(runtime, model, inputs, outputs, stream);
acompiler_synchronize_stream(stream);
```

### Ascend Runtime（TODO）

**规划功能**:
- NPU内存管理
- ACL Runtime集成
- 算子执行
- 多核调度

**计划依赖**:
- CANN Toolkit
- ACL库

**API示例**:
```c
// 创建Ascend Runtime
acompiler_runtime_t* runtime = acompiler_create_ascend_runtime(device_id);

// 分配NPU内存
acompiler_tensor_t* tensor = acompiler_alloc_tensor(runtime, shape, dtype);

// 执行推理
acompiler_execute(runtime, model, inputs, outputs);
```

## 内存管理

### CPU内存
- 对齐分配（SIMD优化）
- 内存池（减少分配开销）
- 引用计数

### GPU内存（TODO）
- 设备内存管理
- 主机-设备传输优化
- 统一内存支持
- 内存池

### NPU内存（TODO）
- DDR/HBM管理
- 数据搬运优化

## 算子库

### CPU算子
```
cpu/src/ops/
├── conv2d.cpp        # 卷积实现
├── matmul.cpp        # 矩阵乘法
├── activation.cpp    # 激活函数
├── pooling.cpp       # 池化操作
└── normalization.cpp # 归一化
```

### CUDA算子（TODO）
```
cuda/src/kernels/
├── conv2d.cu         # CUDA Kernel
├── matmul.cu         # Tensor Core优化
└── ...
```

### Ascend算子（TODO）
```
ascend/src/ops/
├── tbe_ops.cpp       # TBE算子封装
└── ...
```

## 性能优化

### CPU优化
- SIMD向量化（SSE/AVX）
- 缓存优化
- OpenMP并行
- 循环展开

### GPU优化（TODO）
- Kernel融合
- 共享内存
- Warp级优化
- Tensor Core

### NPU优化（TODO）
- AI Core利用率
- 数据流优化
- 多核并行

## 构建Runtime

```bash
# 构建所有Runtime
cd build
ninja runtime

# 只构建CPU Runtime
ninja cpu-runtime

# 构建并安装
ninja install
```

## 测试Runtime

```bash
# 运行Runtime测试
ninja test-runtime

# 性能基准测试
./build/bin/runtime-benchmark
```
