# ACompiler 项目规划

## 一、项目概述

**ACompiler** 是一个基于 LLVM/MLIR 基础设施实现的 AI 编译器学习与实践项目。项目以"从实践中学习"为核心理念，通过逐步构建一个完整的 AI 编译器，帮助开发者快速掌握现代编译器技术栈，涵盖从高层图优化（MLIR）到底层代码生成（LLVM IR / CodeGen）的全链路知识。

### 1.1 项目目标

- **学习目标**: 通过实践掌握 MLIR 多层级中间表示、LLVM IR 优化、指令选择与代码生成等核心编译器技术
- **工程目标**: 构建一个可运行、可扩展的 AI 编译器原型，支持多后端架构
- **开源目标**: 形成高质量的学习资料和代码库，帮助更多人学习 AI 编译器

### 1.2 技术路线

```
AI Model (ONNX/自定义格式)
        │
        ▼
┌─────────────────────────────┐
│     Frontend (模型导入)       │  ← 模型解析、图构建
├─────────────────────────────┤
│     High-Level Dialect      │  ← ACompiler 自定义高层方言 (ACHigh Dialect)
│     (MLIR 高层优化)          │     图级优化：算子融合、常量折叠、布局变换
├─────────────────────────────┤
│     Mid-Level Dialect       │  ← ACompiler 中层方言 (ACMid Dialect)
│     (MLIR 中层优化)          │     Tiling、Loop优化、向量化
├─────────────────────────────┤
│     Low-Level Dialect       │  ← 复用 MLIR 内置方言 (Linalg/Affine/SCF/Arith)
│     (MLIR -> LLVM 方言)     │     Lowering 到 LLVM Dialect
├─────────────────────────────┤
│     LLVM IR                 │  ← LLVM IR 优化 Pass
│     (LLVM IR 优化)          │     标量优化、循环优化、向量化
├─────────────────────────────┤
│     CodeGen                 │  ← 指令选择、寄存器分配、指令调度
│     (目标代码生成)           │     多后端支持
├─────────────────────────────┤
│     Target Binary           │  ← 可执行文件 / 动态链接库
└─────────────────────────────┘
        │
   ┌────┴─────────────┬──────────────────┐
   ▼                  ▼                  ▼
 CPU (x86/ARM)   NVIDIA GPU (CUDA)   Ascend NPU
 [Phase 1]       [TODO Phase 2]      [TODO Phase 3]
```

---

## 二、项目阶段规划

### Phase 1: 项目基础设施与环境搭建（第1-2周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 1.1 | LLVM/MLIR 源码编译 | 从源码编译 LLVM 和 MLIR，了解构建系统 | 可用的 LLVM/MLIR 开发环境 |
| 1.2 | CMake 工程搭建 | 搭建 ACompiler 项目的 CMake 构建系统 | CMakeLists.txt 及构建脚本 |
| 1.3 | 项目目录结构设计 | 设计清晰的模块化目录结构 | 完整的项目骨架 |
| 1.4 | CI/CD 基础配置 | 配置自动化构建和测试流水线 | GitHub Actions 配置 |
| 1.5 | 开发文档框架 | 建立文档体系，包含设计文档和学习笔记 | docs 目录下的文档框架 |

**学习产出**:
- 理解 LLVM 项目的构建体系（CMake + Ninja）
- 掌握 LLVM/MLIR out-of-tree 项目的搭建方式
- 熟悉 TableGen、ODS（Operation Definition Specification）等基础设施

---

### Phase 2: MLIR 基础 — 自定义方言与基本操作（第3-5周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 2.1 | MLIR 核心概念学习 | 学习 Dialect、Operation、Type、Attribute 等核心概念 | 学习笔记 |
| 2.2 | ACHigh Dialect 定义 | 使用 ODS/TableGen 定义 AI 高层方言 | .td 文件及生成的 C++ 代码 |
| 2.3 | 基本 AI 算子定义 | 定义 MatMul、Conv2D、ReLU、Add 等基本算子 | 算子定义及验证逻辑 |
| 2.4 | Type System 设计 | 设计 Tensor 类型系统，支持不同数据类型和形状 | 类型定义及推导逻辑 |
| 2.5 | MLIR 基本 Pass 框架 | 理解 Pass 管理框架，编写第一个 Pass | Pass 注册及运行框架 |
| 2.6 | 测试框架搭建 | 基于 FileCheck + lit 搭建 MLIR 测试体系 | 测试基础设施 |

**核心知识点**:
- MLIR Dialect 的定义与注册机制
- TableGen / ODS 语法及代码生成原理
- MLIR Operation 的 Traits、Interfaces 机制
- MLIR 的 Region 和 Block 结构
- MLIR 的验证（Verification）机制

**示例代码**:
```mlir
// ACHigh Dialect 示例
func.func @simple_model(%input: tensor<1x3x224x224xf32>) -> tensor<1x1000xf32> {
  %weight_conv = achigh.constant dense<...> : tensor<64x3x7x7xf32>
  %conv = achigh.conv2d(%input, %weight_conv) {
    strides = [2, 2], padding = [3, 3, 3, 3]
  } : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
  %relu = achigh.relu(%conv) : tensor<1x64x112x112xf32> -> tensor<1x64x112x112xf32>
  // ... more operations
  return %result : tensor<1x1000xf32>
}
```

---

### Phase 3: MLIR 图层优化（第6-8周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 3.1 | 常量折叠 Pass | 实现编译期常量计算优化 | ConstantFoldingPass |
| 3.2 | 算子融合 Pass | 实现 Conv+BN+ReLU 等经典融合模式 | OpFusionPass |
| 3.3 | 死代码消除 Pass | 消除未使用的算子和变量 | DeadCodeEliminationPass |
| 3.4 | 形状推导 Pass | 实现 Tensor 形状的自动推导 | ShapeInferencePass |
| 3.5 | 布局变换优化 | 实现 NCHW/NHWC 等数据布局变换 | LayoutTransformPass |
| 3.6 | DRR 模式匹配 | 使用 Declarative Rewrite Rules 实现模式替换 | .td 模式定义文件 |
| 3.7 | Pass Pipeline 管理 | 组合多个 Pass 形成优化流水线 | PassPipeline 配置 |

**核心知识点**:
- MLIR Pattern Rewriting 机制（Canonicalization、DRR）
- MLIR DialectConversion 框架
- Greedy Pattern Rewriting vs. Dialect Conversion
- MLIR Pass 的依赖管理和调度
- 图优化的常见策略和启发式规则

---

### Phase 4: MLIR Lowering — 从高层到低层（第9-11周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 4.1 | ACMid Dialect 定义 | 定义中层方言，表达 Tile/Loop 级别的计算 | ACMid Dialect 定义 |
| 4.2 | ACHigh → ACMid Lowering | 实现从图层到循环层的 Lowering | HighToMidLoweringPass |
| 4.3 | Linalg Dialect 对接 | 将 ACMid 映射到 MLIR Linalg 方言 | MidToLinalgLoweringPass |
| 4.4 | Tiling 策略实现 | 实现多层 Tiling（寄存器/L1/L2级别） | TilingPass |
| 4.5 | Loop 优化 | 循环展开、循环交换、循环融合等 | LoopOptimizationPass |
| 4.6 | 向量化 Lowering | 利用 Vector Dialect 实现 SIMD 向量化 | VectorizationPass |
| 4.7 | Affine → SCF → CF Lowering | 逐层 Lowering 到 LLVM Dialect | AffineToSCFPass, SCFToCFPass |
| 4.8 | LLVM Dialect 生成 | 最终生成 LLVM Dialect IR | ToLLVMDialectPass |

**核心知识点**:
- MLIR 的多层 Lowering 架构设计
- Partial Lowering vs. Full Lowering
- Linalg on Tensors → Linalg on Buffers（Bufferization）
- MLIR 的 Bufferization 框架
- Affine 分析与优化（Dependence Analysis、Affine Scheduling）
- MLIR 到 LLVM Dialect 的类型映射和操作映射

---

### Phase 5: LLVM IR 优化（第12-14周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 5.1 | LLVM IR 基础 | 学习 LLVM IR 语法和语义 | 学习笔记及示例 |
| 5.2 | MLIR → LLVM IR 转换 | 从 LLVM Dialect 导出标准 LLVM IR | LLVM IR 导出模块 |
| 5.3 | 自定义 Function Pass | 编写函数级别的 LLVM 优化 Pass | CustomFunctionPass |
| 5.4 | 自定义 Module Pass | 编写模块级别的 LLVM 优化 Pass | CustomModulePass |
| 5.5 | 循环优化 Pass | 基于 LLVM Loop Infrastructure 实现循环优化 | LoopOptPass |
| 5.6 | 向量化 Pass | 利用 LLVM 的 SLP/Loop Vectorizer 接口 | VectorizePass |
| 5.7 | Pass Manager 集成 | 将自定义 Pass 集成到 LLVM Pass Pipeline | PassManager 配置 |
| 5.8 | 优化效果分析 | 使用 opt-viewer 等工具分析优化效果 | 优化报告工具 |

**核心知识点**:
- LLVM IR 的 SSA 形式、PHI 节点、基本块结构
- LLVM 新 Pass Manager (NPM) 架构
- LLVM Analysis Pass（DominatorTree、LoopInfo、AliasAnalysis 等）
- LLVM Intrinsics 和 Target-Specific Intrinsics
- LLVM 的 IR Linker 和 LTO（Link Time Optimization）

---

### Phase 6: LLVM 语言前端实践（第15-17周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 6.1 | 词法分析器（Lexer）| 实现简单 AI DSL 的词法分析 | Lexer 模块 |
| 6.2 | 语法分析器（Parser）| 实现递归下降解析器 | Parser 模块 |
| 6.3 | AST 设计与构建 | 设计抽象语法树数据结构 | AST 模块 |
| 6.4 | 语义分析 | 类型检查、符号表管理 | Sema 模块 |
| 6.5 | AST → MLIR 生成 | 从 AST 生成 MLIR（ACHigh Dialect） | MLIRGen 模块 |
| 6.6 | ONNX 模型导入 | 实现 ONNX 格式模型的导入 | ONNXImporter |
| 6.7 | 端到端流水线 | 串联整个编译流水线 | Driver 模块 |

**AI DSL 示例**:
```
// ACompiler DSL 示例
model SimpleNet {
  input x: tensor<1, 3, 224, 224, f32>

  layer conv1 = Conv2D(x, filters=64, kernel=3, stride=1, padding=same)
  layer relu1 = ReLU(conv1)
  layer pool1 = MaxPool2D(relu1, kernel=2, stride=2)

  layer fc1 = Dense(flatten(pool1), units=1000)
  output = Softmax(fc1)
}

compile SimpleNet -> target("cpu") {
  optimize {
    fuse_ops = true
    vectorize = true
    tiling_size = [32, 32]
  }
}
```

---

### Phase 7: CodeGen — 目标代码生成（第18-20周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 7.1 | LLVM 后端架构学习 | 学习 SelectionDAG、指令选择等 | 学习笔记 |
| 7.2 | 指令选择（ISel）| 理解 LLVM 指令选择机制 | ISel 相关代码分析 |
| 7.3 | 寄存器分配 | 理解寄存器分配算法 | RegAlloc 分析笔记 |
| 7.4 | 指令调度 | 理解指令调度和流水线优化 | InstrScheduling 分析 |
| 7.5 | CPU 后端实现 | 生成 x86-64 / AArch64 目标代码 | CPU 后端模块 |
| 7.6 | Runtime 库实现 | 实现 AI 算子的 Runtime 支持库 | ACRuntime 库 |
| 7.7 | JIT 编译支持 | 基于 LLVM ORC JIT 实现即时编译 | JIT 模块 |
| 7.8 | 端到端执行 | 从模型输入到推理结果的完整流程 | 可运行的 Demo |

**核心知识点**:
- SelectionDAG 和 GlobalISel 两种指令选择框架
- TableGen 目标描述（.td）文件的编写
- 寄存器分配算法（Linear Scan、Graph Coloring）
- MC Layer（Machine Code Layer）架构
- Object File 生成（ELF、Mach-O）

---

### Phase 8: 高级特性与性能调优（第21-24周）

| 编号 | 任务 | 说明 | 产出 |
|------|------|------|------|
| 8.1 | 内存优化 | 内存分配策略、Buffer 复用优化 | MemoryOptPass |
| 8.2 | 并行化 | 多线程并行执行支持 | ParallelizationPass |
| 8.3 | AutoTuning 框架 | 自动调优框架（Tiling Size、Unroll Factor等） | AutoTuner 模块 |
| 8.4 | 性能 Profiling | 编译器性能分析和瓶颈定位工具 | Profiler 工具 |
| 8.5 | Benchmark 套件 | 常见 AI 模型的 Benchmark 测试 | Benchmark 模块 |
| 8.6 | 量化支持 | INT8/FP16 量化编译支持 | QuantizationPass |

---

## 三、多后端支持规划

### 3.1 Phase 1: CPU 后端（当前实现）

- **目标架构**: x86-64 (SSE/AVX/AVX-512), AArch64 (NEON/SVE)
- **实现方式**: 复用 LLVM 现有的 x86/AArch64 后端
- **优化重点**: SIMD 向量化、Cache 友好的 Tiling、多线程并行

### 3.2 [TODO] Phase 2: NVIDIA GPU 后端（CUDA 支持）

> **状态**: 待实现（计划在 CPU 后端稳定后启动）

- **目标**: 支持 NVIDIA GPU 的 CUDA 代码生成
- **技术路线**:
  - 定义 `ACGPU` Dialect，表达 GPU 计算特有的概念（Grid/Block/Thread/SharedMem）
  - 实现 ACMid → ACGPU Dialect 的 Lowering
  - 利用 MLIR 的 GPU Dialect 和 NVVM Dialect
  - 通过 LLVM NVPTX 后端生成 PTX 代码
  - 集成 CUDA Runtime 实现 Kernel 启动
- **优化方向**:
  - Shared Memory 优化
  - Warp-level 优化
  - Tensor Core 映射（WMMA/MMA 指令）
  - Bank Conflict 消除
  - Occupancy 优化
- **预期目录结构**:
  ```
  lib/Dialect/ACGPU/           # GPU 方言定义
  lib/Conversion/MidToGPU/     # Lowering Pass
  lib/Target/NVPTX/            # NVPTX 代码生成
  runtime/cuda/                # CUDA Runtime 支持库
  ```

### 3.3 [TODO] Phase 3: 华为 Ascend NPU 后端（Ascend IR 支持）

> **状态**: 待实现（计划在 GPU 后端基本完成后启动）

- **目标**: 支持华为 Ascend NPU 的代码生成
- **技术路线**:
  - 定义 `ACAscend` Dialect，表达 Ascend NPU 特有概念（AI Core/Vector Core/Cube Unit）
  - 实现 ACMid → ACAscend Dialect 的 Lowering
  - 生成 Ascend IR（CCE-C 或 TBE 算子描述）
  - 对接华为 CANN 工具链
- **优化方向**:
  - Cube Unit 矩阵计算映射
  - Vector Unit 向量化
  - Data Flow 优化
  - 双 Buffer 乒乓优化
  - AI Core 间并行调度
- **预期目录结构**:
  ```
  lib/Dialect/ACAscend/        # Ascend NPU 方言定义
  lib/Conversion/MidToAscend/  # Lowering Pass
  lib/Target/AscendIR/         # Ascend IR 代码生成
  runtime/ascend/              # Ascend Runtime 支持库
  ```

---

## 四、关键技术栈

| 技术领域 | 技术/工具 | 用途 |
|----------|-----------|------|
| 编译基础设施 | LLVM 18+ | IR 优化、代码生成 |
| 多层级 IR | MLIR | 自定义方言、图优化、Lowering |
| 构建系统 | CMake + Ninja | 项目构建 |
| 代码生成 | TableGen | 方言定义、指令描述 |
| 测试框架 | lit + FileCheck | MLIR/LLVM IR 测试 |
| 单元测试 | GoogleTest | C++ 单元测试 |
| 模型格式 | ONNX (protobuf) | AI 模型导入 |
| 编程语言 | C++17 | 主要开发语言 |
| 文档 | Doxygen + Markdown | 代码文档和学习笔记 |
| 版本管理 | Git + GitHub | 代码版本管理 |

---

## 五、里程碑定义

| 里程碑 | 时间 | 目标 | 验收标准 |
|--------|------|------|----------|
| **M1** | 第2周末 | 环境就绪 | LLVM/MLIR 编译成功，项目骨架搭建完成 |
| **M2** | 第5周末 | MLIR 基础 | ACHigh Dialect 定义完成，基本算子可用 |
| **M3** | 第8周末 | 图优化 | 算子融合、常量折叠等核心 Pass 实现 |
| **M4** | 第11周末 | Lowering | MLIR 到 LLVM Dialect 的完整 Lowering 链路打通 |
| **M5** | 第14周末 | LLVM 优化 | 自定义 LLVM Pass 实现，优化 Pipeline 可运行 |
| **M6** | 第17周末 | 前端完成 | AI DSL 前端 + ONNX 导入可用 |
| **M7** | 第20周末 | 端到端 | 从模型输入到 CPU 可执行代码的完整链路 |
| **M8** | 第24周末 | 高级特性 | AutoTuning、Benchmark、性能分析工具就绪 |

---

## 六、学习资源推荐

### MLIR 相关
1. [MLIR 官方文档](https://mlir.llvm.org/)
2. [MLIR Toy Tutorial](https://mlir.llvm.org/docs/Tutorials/Toy/)
3. [MLIR: A Compiler Infrastructure for the End of Moore's Law](https://arxiv.org/abs/2002.11054)

### LLVM 相关
1. [LLVM Language Reference Manual](https://llvm.org/docs/LangRef.html)
2. [Writing an LLVM Pass](https://llvm.org/docs/WritingAnLLVMNewPMPass.html)
3. [LLVM CodeGen Documentation](https://llvm.org/docs/CodeGenerator.html)
4. [Getting Started with LLVM Core Libraries](https://www.packtpub.com/product/getting-started-with-llvm-core-libraries/9781782166924)

### AI 编译器
1. [TVM: An Automated End-to-End Optimizing Compiler for Deep Learning](https://arxiv.org/abs/1802.04799)
2. [The Deep Learning Compiler: A Comprehensive Survey](https://arxiv.org/abs/2002.03794)
3. [IREE Project](https://github.com/openxla/iree) — MLIR 编译器参考实现
