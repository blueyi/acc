# ACompiler 架构设计

## 系统架构概览

```
┌─────────────────────────────────────────────────────────────┐
│                      前端层 (Frontend)                        │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │ ONNX导入器   │  │ TF导入器     │  │ PyTorch导入器 │      │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘      │
│         └──────────────────┴─────────────────┘              │
└─────────────────────────┬───────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────────┐
│                    MLIR表示层                                 │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              AI Dialect (高层抽象)                    │   │
│  │  Conv2D, MatMul, ReLU, BatchNorm, etc.              │   │
│  └──────────────────────┬──────────────────────────────┘   │
│                         ↓                                    │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              优化Pass (Transformations)              │   │
│  │  • 算子融合 (Conv+ReLU, MatMul+Add)                  │   │
│  │  • 常量折叠                                           │   │
│  │  • 死代码消除                                         │   │
│  │  • 形状推导                                           │   │
│  └──────────────────────┬──────────────────────────────┘   │
│                         ↓                                    │
│  ┌─────────────────────────────────────────────────────┐   │
│  │           渐进降级 (Progressive Lowering)            │   │
│  │  AI Dialect → Linalg → Affine → SCF → Std           │   │
│  └──────────────────────┬──────────────────────────────┘   │
└─────────────────────────┼───────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────────┐
│                    LLVM IR层                                 │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              LLVM Dialect                            │   │
│  └──────────────────────┬──────────────────────────────┘   │
│                         ↓                                    │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              LLVM优化Pass                            │   │
│  │  • 循环优化 (Unroll, Vectorization)                  │   │
│  │  • 内联优化                                           │   │
│  │  • 内存优化                                           │   │
│  └──────────────────────┬──────────────────────────────┘   │
└─────────────────────────┼───────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────────┐
│                   代码生成层 (CodeGen)                       │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  CPU后端     │  │  CUDA后端    │  │ Ascend后端   │     │
│  │  (x86/ARM)   │  │  (NVIDIA)    │  │  (华为)      │     │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘     │
└─────────┼──────────────────┼──────────────────┼─────────────┘
          ↓                  ↓                  ↓
┌─────────────────────────────────────────────────────────────┐
│                   运行时层 (Runtime)                         │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │ CPU Runtime  │  │ CUDA Runtime │  │Ascend Runtime│     │
│  │ • 内存管理   │  │ • GPU内存    │  │ • NPU内存    │     │
│  │ • 算子库     │  │ • Kernel库   │  │ • TBE算子    │     │
│  │ • 并行执行   │  │ • Stream管理 │  │ • ACL接口    │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
└─────────────────────────────────────────────────────────────┘
```

## 核心组件详解

### 1. 前端导入器 (Frontend Importers)

**职责**: 将不同框架的模型转换为MLIR AI Dialect

**支持的格式**:
- ONNX (当前优先)
- TensorFlow (计划)
- PyTorch (计划)

**关键技术**:
- 图遍历和转换
- 算子映射
- 类型推导

### 2. AI Dialect

**核心算子**:
- 卷积类: Conv2D, Conv3D, DepthwiseConv2D
- 线性代数: MatMul, BatchMatMul, Transpose
- 激活函数: ReLU, Sigmoid, Tanh, GELU
- 池化: MaxPool, AvgPool, GlobalAvgPool
- 归一化: BatchNorm, LayerNorm

**设计原则**:
- 高层抽象，接近AI框架
- 便于图级优化
- 易于降级到标准Dialect

### 3. 优化Pass系统

#### 3.1 融合Pass (Fusion Passes)

**Conv+ReLU融合**:
```mlir
// Before
%conv = ai.conv2d %input, %weight : ...
%relu = ai.relu %conv : ...

// After
%fused = ai.conv_relu %input, %weight : ...
```

**MatMul+Add融合 (GEMM)**:
```mlir
// Before
%matmul = ai.matmul %A, %B : ...
%add = arith.addf %matmul, %bias : ...

// After
%gemm = ai.gemm %A, %B, %bias : ...
```

#### 3.2 优化Pass (Optimization Passes)

- **常量折叠**: 编译时计算常量表达式
- **死代码消除**: 移除未使用的操作
- **CSE**: 消除公共子表达式
- **形状推导**: 推导张量形状

### 4. 渐进降级 (Progressive Lowering)

```
AI Dialect (高层AI算子)
    ↓ AIToLinalg
Linalg Dialect (线性代数抽象)
    ↓ LinalgToAffine
Affine Dialect (仿射循环)
    ↓ AffineToSCF
SCF Dialect (结构化控制流)
    ↓ SCFToStd
Std Dialect (标准操作)
    ↓ StdToLLVM
LLVM Dialect (LLVM IR在MLIR中的表示)
```

**降级示例 - Conv2D**:

```mlir
// 1. AI Dialect
%out = ai.conv2d %in, %weight : tensor<1x3x224x224xf32>

// 2. Linalg Dialect
%out = linalg.conv_2d_nchw_fchw 
  ins(%in, %weight : tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>)
  outs(%init : tensor<1x64x224x224xf32>)

// 3. Affine Dialect (嵌套循环)
affine.for %n = 0 to 1 {
  affine.for %k = 0 to 64 {
    affine.for %h = 0 to 224 {
      affine.for %w = 0 to 224 {
        // 卷积计算
      }
    }
  }
}

// 4. SCF Dialect
scf.for %n = 0 to 1 {
  scf.for %k = 0 to 64 {
    // ...
  }
}

// 5. LLVM Dialect
llvm.br ^bb1
^bb1:
  // 循环体
```

### 5. LLVM优化层

**优化Pass**:
- 循环展开 (Loop Unrolling)
- 循环向量化 (Loop Vectorization)
- 内联 (Inlining)
- 内存访问优化
- 指令调度

### 6. 多后端支持

#### 6.1 CPU后端

**目标架构**:
- x86_64 (SSE, AVX, AVX-512)
- ARM64 (NEON)

**优化技术**:
- SIMD向量化
- 缓存优化
- 多线程并行 (OpenMP/TBB)

#### 6.2 CUDA后端 (TODO)

**代码生成**:
```
MLIR → GPU Dialect → NVVM Dialect → PTX → CUDA Binary
```

**优化**:
- Kernel融合
- Shared Memory优化
- Tensor Core利用

#### 6.3 Ascend后端 (TODO)

**代码生成**:
```
MLIR → Ascend Dialect → Ascend IR → OM模型
```

**集成**:
- CANN框架
- TBE算子
- ACL Runtime

### 7. Runtime系统

#### 7.1 内存管理

```cpp
class MemoryManager {
  void* allocate(size_t size);
  void deallocate(void* ptr);
  void* allocateAligned(size_t size, size_t alignment);
};
```

#### 7.2 算子库

```cpp
// CPU算子接口
void conv2d_cpu(float* input, float* weight, float* output,
                int N, int C, int H, int W, int K,
                int stride, int padding);

void matmul_cpu(float* A, float* B, float* C,
                int M, int N, int K);
```

#### 7.3 执行引擎

```cpp
class ExecutionEngine {
  void execute(CompiledModel* model,
               Tensor** inputs, int num_inputs,
               Tensor** outputs, int num_outputs);
};
```

## 数据流

### 编译时数据流

```
用户模型 (model.onnx)
    ↓
ONNX Parser → 图结构
    ↓
MLIR Builder → AI Dialect IR
    ↓
优化Pass → 优化后的IR
    ↓
降级Pass → LLVM Dialect
    ↓
LLVM优化 → 优化的LLVM IR
    ↓
CodeGen → 目标代码 (model.o)
    ↓
链接Runtime → 可执行文件/共享库
```

### 运行时数据流

```
加载模型 → ExecutionEngine
    ↓
准备输入 → Tensor分配
    ↓
执行推理 → 算子调用
    ↓
获取输出 → 结果返回
    ↓
释放资源 → 内存清理
```

## Pass Pipeline设计

### 标准Pipeline

```cpp
PassManager pm;

// 前端优化
pm.addPass(createShapeInferencePass());
pm.addPass(createConvReLUFusionPass());
pm.addPass(createConstantFoldingPass());

// 降级流程
pm.addPass(createConvertAIToLinalgPass());
pm.addPass(createConvertLinalgToAffinePass());
pm.addPass(createConvertAffineToSCFPass());
pm.addPass(createConvertSCFToStdPass());
pm.addPass(createConvertStdToLLVMPass());

// LLVM优化
pm.addPass(createLoopUnrollPass());
pm.addPass(createVectorizePass());
pm.addPass(createInliningPass());
```

## 扩展性设计

### 添加新算子

1. 在AI Dialect中定义
2. 实现降级到Linalg
3. 添加Runtime实现
4. 测试验证

### 添加新后端

1. 实现Target接口
2. 添加CodeGen逻辑
3. 实现Runtime库
4. 集成到Pipeline

### 添加新优化

1. 定义Pass
2. 实现优化逻辑
3. 注册到Pipeline
4. 性能测试

## 性能考量

### 编译时性能

- TableGen代码生成
- 并行Pass执行
- 增量编译支持

### 运行时性能

- 算子融合减少kernel启动
- SIMD向量化
- 内存对齐和预取
- 多线程并行

## 调试和分析

### IR可视化

```bash
# 打印所有Pass后的IR
acompiler-opt input.mlir -mlir-print-ir-after-all

# 可视化计算图
acompiler-opt input.mlir -view-cfg
```

### 性能分析

```bash
# Pass执行时间
acompiler-opt input.mlir -mlir-timing

# Runtime性能
perf record ./model
perf report
```

## 未来扩展

- 动态Shape支持
- 量化感知编译
- Auto-tuning框架
- 分布式编译
- JIT编译支持
