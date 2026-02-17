# ACC 项目进度跟踪

> 本文件用于分层跟踪 ACC (AI Compiler Core) 项目每个阶段的每一项任务。
> 开发者可以 step by step 地完成各阶段 TODO，从而追踪整体项目进度。
>
> **使用方法**: 完成任务后将 `[ ]` 改为 `[x]`，并在备注中记录完成日期或相关 PR。
>
> **进度概览**: [Phase 1](#phase-1-基础架构与-mlir-入门week-1-4) → [Phase 2](#phase-2-mlir-lowering-与-llvm-ir-优化week-5-8) → [Phase 3](#phase-3-代码生成runtime-与端到端流水线week-9-12) → [Phase 4](#phase-4-高级特性与多后端扩展week-13-14) → [Phase 5](#phase-5-ai-框架对接--triton--pytorch-集成week-15-20) → [Phase 6](#phase-6-高阶特性week-20)

---

## 总体进度

| 阶段 | 状态 | 进度 | 周期 |
|------|------|------|------|
| Phase 1: 基础架构与 MLIR 入门 | 🚧 进行中 | ░░░░░░░░░░ 0% | Week 1-4 |
| Phase 2: MLIR Lowering 与 LLVM IR 优化 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 5-8 |
| Phase 3: CodeGen、Runtime 与 E2E | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 9-12 |
| Phase 4: 高级特性与多后端扩展 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 13-14 |
| Phase 5: Triton / PyTorch 框架对接 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 15-20 |
| Phase 6: 高阶特性 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 20+ |

---

## Phase 1: 基础架构与 MLIR 入门（Week 1-4）

### 1.1 项目工程搭建（Week 1）

- [x] **1.1.1** LLVM/MLIR 源码编译 — 从源码编译 LLVM 17+ 和 MLIR
- [x] **1.1.2** CMake 工程搭建 — 搭建 ACC 的 out-of-tree CMake 构建系统
- [x] **1.1.3** 项目目录结构设计 — 按 Dialect/Conversion/Transforms 等分层
- [ ] **1.1.4** 单元测试框架搭建 — 集成 GoogleTest
- [x] **1.1.5** lit/FileCheck 测试搭建 — 配置 lit + FileCheck
- [x] **1.1.6** CI/CD 配置 — 配置 GitHub Actions

**验收检查**:
- [ ] 项目能成功编译，无警告
- [ ] `ac-opt --version` 正常输出
- [ ] GoogleTest 和 lit 测试框架均可运行
- [ ] CI 自动构建成功

---

### 1.2 MLIR 基础概念实践 — 自定义方言与算子（Week 2）

- [ ] **1.2.1** MLIR 核心概念学习 — Dialect、Operation、Type、Attribute、Region、Block
- [x] **1.2.2** ACHigh Dialect 定义 — 使用 ODS/TableGen 定义高层 AI 方言
- [x] **1.2.3** 基本 AI 算子定义 — Conv2D、MatMul、BatchMatMul、ReLU、Add 等
- [x] **1.2.4** 增强算子定义 — GELU、Sigmoid、BatchNorm、Softmax、Dense、ConvReLU 等
- [x] **1.2.5** Type System 设计 — Tensor 类型系统
- [x] **1.2.6** 量化类型支持 — QuantizedType 支持 INT8/INT4
- [ ] **1.2.7** 算子验证逻辑 — 为每个算子实现 verify()
- [ ] **1.2.8** 形状推导接口 — Conv2D 等实现 InferTypeOpInterface
- [x] **1.2.9** ACMid Dialect 定义 — 中层方言 Tile/Loop 级
- [ ] **1.2.10** 方言测试 — FileCheck 测试验证方言正确性

**验收检查**:
- [ ] 成功定义 20+ AI 算子
- [ ] 所有算子都有 verify() 实现
- [ ] Conv2D 实现 InferTypeOpInterface 自动形状推导
- [ ] 通过 `--verify-diagnostics` 测试
- [ ] ACMid Dialect 基本框架就绪

---

### 1.3 MLIR Pass 开发（Week 3）

- [ ] **1.3.1** Pass 管理框架学习 — PassManager、OpPassManager、Pass 注册机制
- [ ] **1.3.2** 常量折叠 Pass — 编译期常量计算优化
- [ ] **1.3.3** 算子融合 Pass — Conv+BN+ReLU、Conv+ReLU 融合模式
- [ ] **1.3.4** 形状推导 Pass — Tensor 形状前向自动推导
- [ ] **1.3.5** 布局变换 Pass — NCHW/NHWC 布局变换优化
- [ ] **1.3.6** 死代码消除 Pass — 消除未使用的算子和变量
- [ ] **1.3.7** DRR 模式匹配 — Declarative Rewrite Rules 模式替换
- [ ] **1.3.8** Pass Pipeline 管理 — 组合多个 Pass 形成优化流水线
- [ ] **1.3.9** Pass 测试 — 每个 Pass 的 FileCheck 测试

**验收检查**:
- [ ] 实现至少 5 个优化 Pass
- [ ] 每个 Pass 都有 FileCheck 测试
- [ ] 融合 Pass 能正确优化 Conv+BN+ReLU 和 Conv+ReLU 模式
- [ ] 形状推导 Pass 能正确推导 Conv2D、MatMul 输出形状
- [ ] Pass Pipeline 能按正确顺序组合运行

---

### 1.4 前端支持（Week 4）

- [ ] **1.4.1** ONNX 导入器设计 — ONNX → ACHigh Dialect 映射框架
- [ ] **1.4.2** ONNX 算子映射 — 20+ ONNX 算子到 ACHigh 算子的映射
- [ ] **1.4.3** 权重/常量处理 — ONNX initializer 到 achigh.constant 转换
- [ ] **1.4.4** DSL 词法分析器 — ACC DSL Lexer
- [ ] **1.4.5** DSL 语法分析器 — 递归下降 Parser
- [ ] **1.4.6** AST 设计与构建 — DSL 抽象语法树
- [ ] **1.4.7** 语义分析 — 类型检查、符号表管理
- [ ] **1.4.8** AST → MLIR 生成 — 从 AST 生成 ACHigh Dialect MLIR
- [ ] **1.4.9** 前端测试 — ONNX 导入和 DSL 解析测试

**验收检查**:
- [ ] ONNX 导入器支持至少 20 个常见算子
- [ ] 能成功导入 ResNet-18 / MobileNet 模型
- [ ] DSL 前端能解析基本模型定义并生成 MLIR
- [ ] 导入后的 MLIR 通过 verify 验证

---

## Phase 2: MLIR Lowering 与 LLVM IR 优化（Week 5-8）

### 2.1 MLIR 多层 Lowering — ACHigh 到 Linalg（Week 5）

- [ ] **2.1.1** ACHigh → ACMid Lowering — 图层算子到 Tile/Loop 层
- [ ] **2.1.2** ACMid → Linalg Lowering — ACMid 映射到 Linalg 通用操作
- [ ] **2.1.3** Conv2D Lowering 实现 — Conv2D → linalg.conv_2d_nchw_fchw
- [ ] **2.1.4** MatMul Lowering 实现 — MatMul → linalg.matmul
- [ ] **2.1.5** 激活函数 Lowering — ReLU/GELU/Sigmoid → linalg.generic
- [ ] **2.1.6** Tiling 策略实现 — 多层 Tile（L1/L2/Register 级）
- [ ] **2.1.7** Lowering 测试 — 每层 Lowering 的 FileCheck 测试

**验收检查**:
- [ ] ACHigh → ACMid → Linalg 完整降级链路可运行
- [ ] Conv2D、MatMul、激活函数核心算子均已实现 Lowering
- [ ] Tiling Pass 能对 Linalg 操作进行分块
- [ ] 每层降级都有 FileCheck 测试

---

### 2.2 MLIR Lowering — Linalg 到 LLVM Dialect（Week 6）

- [ ] **2.2.1** Bufferization — Tensor → MemRef 缓冲区化
- [ ] **2.2.2** Linalg → Loops — 展开为嵌套循环（SCF/Affine）
- [ ] **2.2.3** 循环优化 — 循环展开、循环交换、循环融合
- [ ] **2.2.4** 向量化 Lowering — Vector Dialect 实现 SIMD 向量化
- [ ] **2.2.5** Affine → SCF → CF — 逐层 Lowering 到控制流方言
- [ ] **2.2.6** CF → LLVM Dialect — 最终降级到 LLVM Dialect
- [ ] **2.2.7** 端到端降级测试 — ACHigh 到 LLVM Dialect 完整链路测试

**验收检查**:
- [ ] 完成从 ACHigh 到 LLVM Dialect 的完整降级链路
- [ ] 生成的 LLVM Dialect IR 能通过 mlir-translate 转为 LLVM IR
- [ ] 生成的 LLVM IR 能通过 `llc` 编译为目标代码
- [ ] Bufferization 正确处理 Tensor → MemRef 转换

---

### 2.3 LLVM IR 优化 Pass（Week 7）

- [ ] **2.3.1** LLVM IR 基础学习 — SSA 形式、基本块结构
- [ ] **2.3.2** MLIR → LLVM IR 导出 — 从 LLVM Dialect 导出标准 LLVM IR
- [ ] **2.3.3** 自定义 Function Pass — AI 计算模式特化优化
- [ ] **2.3.4** 自定义 Vectorize Pass — AI 工作负载向量化
- [ ] **2.3.5** LLVM 标准 Pass 集成 — 集成标准优化 Pipeline（O0-O3）
- [ ] **2.3.6** 循环优化 — 基于 LLVM Loop Infrastructure 的循环展开/向量化
- [ ] **2.3.7** Pass Manager 集成 — 注册到 LLVM New Pass Manager
- [ ] **2.3.8** 优化效果分析 — opt-viewer / opt-remark 分析

**验收检查**:
- [ ] 成功从 LLVM Dialect 导出标准 LLVM IR
- [ ] 自定义 LLVM Pass 注册到 NPM 并可运行
- [ ] 集成至少 10 个 LLVM 标准优化 Pass
- [ ] 循环向量化成功应用到卷积/矩阵乘法
- [ ] 优化后性能提升 30%+

---

### 2.4 指令选择与 SIMD 映射（Week 8）

- [ ] **2.4.1** LLVM 后端架构学习 — SelectionDAG、指令选择、GlobalISel
- [ ] **2.4.2** SIMD 指令映射 — Vector Dialect/LLVM Intrinsics 映射 AVX/NEON
- [ ] **2.4.3** 指令代价模型 — x86 指令延迟/吞吐量代价模型
- [ ] **2.4.4** FMA 指令利用 — 确保矩阵乘法使用 FMA 指令
- [ ] **2.4.5** 汇编分析 — 验证生成的汇编包含预期 SIMD 指令

**验收检查**:
- [ ] 卷积和矩阵乘法核心循环使用 SIMD 指令
- [ ] 生成的汇编包含 AVX/AVX2（x86）或 NEON（AArch64）指令
- [ ] SIMD 优化带来 2-4x 性能提升

---

## Phase 3: 代码生成、Runtime 与端到端流水线（Week 9-12）

### 3.1 CPU 后端 CodeGen（Week 9）

- [ ] **3.1.1** CodeGen 驱动实现 — LLVM IR → 目标代码驱动
- [ ] **3.1.2** TargetMachine 配置 — x86-64 / AArch64 配置
- [ ] **3.1.3** Object File 生成 — 生成 .o 目标文件
- [ ] **3.1.4** Assembly 生成 — 生成 .s 汇编文件
- [ ] **3.1.5** JIT 编译引擎 — LLVM ORC JIT 即时编译
- [ ] **3.1.6** 端到端编译驱动 — Frontend→MLIR Opt→Lowering→LLVM Opt→CodeGen

**验收检查**:
- [ ] 能从 MLIR 生成可运行的 .o / .so 文件
- [ ] JIT 引擎能直接执行 MLIR/LLVM IR
- [ ] 编译驱动串联完整流水线

---

### 3.2 Runtime 库实现（Week 10）

- [x] **3.2.1** Runtime C API 设计 — 句柄式 C API（runtime/tensor/model）
- [x] **3.2.2** 内存管理 — 64 字节对齐内存池
- [x] **3.2.3** Naive Kernels — 全部算子 naive 参考实现
- [x] **3.2.4** im2col + GEMM 卷积 — 高性能 Conv2D
- [x] **3.2.5** Tiled MatMul — cache 友好的分块矩阵乘法
- [ ] **3.2.6** 多线程支持 — 集成 OpenMP 并行计算
- [ ] **3.2.7** 模型加载/执行 — ac_load_model / ac_execute 实现
- [ ] **3.2.8** Runtime 测试 — Kernel 正确性和 API 测试

**验收检查**:
- [ ] Runtime API 完整且文档齐全
- [ ] 所有 CPU Kernel 通过正确性测试
- [ ] im2col+GEMM 性能优于 naive 5x+
- [ ] 内存池管理正常，无内存泄漏
- [ ] 多线程执行正常

---

### 3.3 端到端执行与性能优化（Week 11-12）

- [ ] **3.3.1** 端到端 MatMul 测试 — DSL/MLIR → 执行 完整流程
- [ ] **3.3.2** 端到端 Conv 测试 — MLIR → 执行 完整流程
- [ ] **3.3.3** ResNet-18 推理 — ONNX 导入 → 编译 → 推理
- [x] **3.3.4** 性能基准框架 — Benchmark 框架（统计分析、GFLOPS）
- [x] **3.3.5** 单算子 Benchmark — Conv2D、MatMul 多尺寸测试
- [ ] **3.3.6** 模型 Benchmark — ResNet-18、MobileNet 端到端性能
- [ ] **3.3.7** 性能 Profiling — perf/VTune 定位瓶颈
- [ ] **3.3.8** 瓶颈优化 — 至少 3 轮 Profile→Optimize 迭代

**验收检查**:
- [ ] ResNet-18 端到端推理成功（rtol=1e-3）
- [ ] Conv2D 单算子性能达标（60%+ ONNX Runtime）
- [ ] 建立完整性能基准数据
- [ ] 识别并优化至少 3 个性能瓶颈
- [ ] 完成性能分析报告

---

## Phase 4: 高级特性与多后端扩展（Week 13-14）

### 4.1 高级编译器特性

- [ ] **4.1.1** 内存优化 Pass — Buffer 复用、内存分配策略
- [ ] **4.1.2** 并行化 Pass — 循环并行化（OpenMP）
- [ ] **4.1.3** 量化支持 — INT8/FP16 量化编译
- [ ] **4.1.4** AutoTuning 框架 — Tiling Size、Unroll Factor 自动调优
- [ ] **4.1.5** 性能 Profiling 工具 — 编译器内置性能分析

### 4.2 [TODO] NVIDIA GPU CUDA 后端

- [ ] **4.2.1** ACGPU Dialect 定义 — Grid/Block/Thread/SharedMem
- [ ] **4.2.2** ACMid → ACGPU Lowering — Tile 层到 GPU Kernel
- [ ] **4.2.3** GPU → NVVM Lowering — MLIR GPU/NVVM Dialect 生成 PTX
- [ ] **4.2.4** Kernel 优化 — Shared Memory、Tensor Core、Bank Conflict
- [ ] **4.2.5** CUDA Runtime 集成 — runtime/cuda/ 实现

### 4.3 [TODO] 华为 Ascend NPU 后端

- [ ] **4.3.1** ACAscend Dialect 定义 — AI Core/Vector Core/Cube Unit
- [ ] **4.3.2** ACMid → ACAscend Lowering — 降级到 Ascend 方言
- [ ] **4.3.3** Ascend IR 生成 — CCE-C / TBE 算子描述
- [ ] **4.3.4** CANN 工具链对接 — 对接华为 CANN 编译工具链
- [ ] **4.3.5** Ascend Runtime 集成 — runtime/ascend/ 实现

---

## Phase 5: AI 框架对接 — Triton / PyTorch 集成（Week 15-20）

### 5.1 OpenAI Triton 前端对接（Week 15-16）

- [ ] **5.1.1** Triton IR 分析 — 分析 TTIR / TTGIR 结构和语义
- [x] **5.1.2** TritonImporter 框架 — Triton IR → ACC MLIR 导入框架
- [ ] **5.1.3** Triton IR → ACC MLIR 转换实现 — 完整算子映射
- [ ] **5.1.4** Triton 算子映射 — tl.load/tl.store/tl.dot/tl.reduce → ACC 算子
- [ ] **5.1.5** Triton 语义保持 — Block Program、mask 等特性正确转换
- [ ] **5.1.6** Triton → ACC → CPU — 端到端 Triton kernel 编译执行

**验收检查**:
- [ ] 成功解析 Triton matmul kernel 的 IR
- [ ] tl.load/tl.store/tl.dot 映射到 ACC MLIR 算子
- [ ] Triton kernel → ACC → CPU 端到端执行正确
- [ ] 数值结果与 Triton 原生执行一致（rtol=1e-3）

---

### 5.2 PyTorch 框架对接（Week 17-18）

- [ ] **5.2.1** PyTorch 导出分析 — torch.export / torch.compile IR 格式
- [x] **5.2.2** TorchImporter 框架 — Torch-MLIR / ONNX 两条通路
- [ ] **5.2.3** ONNX 通路增强 — 增强 PyTorch → ONNX → ACC 转换
- [ ] **5.2.4** 自定义算子注册 — PyTorch 调用 ACC 编译的算子（torch.library）
- [ ] **5.2.5** Triton 算子集成 — PyTorch 中使用 Triton 算子由 ACC 编译
- [ ] **5.2.6** Python Binding — pybind11 ACC Python 绑定

**验收检查**:
- [ ] PyTorch 模型通过 ONNX 通路成功导入 ACC
- [ ] Triton kernel 成功编译并在 PyTorch 中调用
- [ ] 端到端数值精度验证通过
- [ ] Python 绑定可用

---

### 5.3 端到端验证流程（Week 19-20）

- [x] **5.3.1** Triton→ACC→CPU E2E 验证 — Triton matmul 编译验证
- [x] **5.3.2** PyTorch+Triton+ACC E2E 验证 — MLP 模型端到端流程
- [ ] **5.3.3** PyTorch→ACC 整图编译 — PyTorch 小模型完整编译验证
- [ ] **5.3.4** 精度对比 — 与 PyTorch eager / Triton GPU 数值精度对比
- [ ] **5.3.5** 性能对比 — 与 PyTorch eager / TorchInductor 性能对比

**验收检查**:
- [ ] PyTorch MLP 模型端到端编译和推理成功
- [ ] Triton matmul/softmax kernel 通过 ACC 编译并执行正确
- [ ] 所有 E2E 测试精度验证通过
- [ ] 生成完整的精度和性能对比报告

---

## Phase 6: 高阶特性（Week 20+）

### 6.1 核心高阶特性

- [ ] **6.1.1** Auto-Scheduling — 代价模型 + 搜索策略自动调优
- [ ] **6.1.2** Polyhedral 优化 — 多面体模型循环变换
- [ ] **6.1.3** 动态形状支持 — 符号化动态维度 + Shape Guard + 特化回退
- [ ] **6.1.4** 混合精度编译 — FP16/BF16/INT8 自动混合精度
- [ ] **6.1.5** 图分割与子图编译 — 大模型智能分割和子图独立编译
- [ ] **6.1.6** Kernel 融合引擎 — 跨算子边界的激进 Kernel 融合
- [ ] **6.1.7** 内存规划优化 — 编译期全局内存规划（最小化峰值内存）
- [ ] **6.1.8** 异构调度 — CPU + GPU 自动任务分配
- [ ] **6.1.9** 分布式编译 — 多设备/多节点分布式模型编译
- [ ] **6.1.10** Debug/Profile 集成 — 编译器级别调试和性能分析工具

### 6.2 Auto-Scheduling 框架

- [ ] **6.2.1** 搜索空间定义 — Tiling/Unroll/Vectorize 可调参数空间
- [ ] **6.2.2** 分析型代价模型 — 基于硬件参数的静态代价估算
- [ ] **6.2.3** ML 代价模型 — 基于历史数据的机器学习代价模型
- [ ] **6.2.4** 搜索算法 — Grid Search / Random / Bayesian Optimization
- [ ] **6.2.5** 自动调优集成 — 将搜索结果集成到 Pass Pipeline

### 6.3 动态形状编译

- [ ] **6.3.1** 符号形状表示 — IR 中支持符号化动态维度
- [ ] **6.3.2** Shape Guard 生成 — 运行时形状检查代码
- [ ] **6.3.3** 特化 + 回退 — 常见形状特化编译 + 动态形状回退

---

## 里程碑跟踪

| 里程碑 | 目标时间 | 目标 | 状态 | 完成日期 |
|--------|---------|------|------|---------|
| **M1** | Week 1 末 | 环境就绪，项目骨架搭建完成 | 🚧 进行中 | — |
| **M2** | Week 2 末 | ACHigh 20+ 算子定义完成 | ⏳ 未开始 | — |
| **M3** | Week 3 末 | 5 个优化 Pass 实现 | ⏳ 未开始 | — |
| **M4** | Week 4 末 | ONNX 20+ 算子导入 + DSL 前端 | ⏳ 未开始 | — |
| **M5** | Week 6 末 | ACHigh → LLVM Dialect 降级链路 | ⏳ 未开始 | — |
| **M6** | Week 8 末 | LLVM 优化 Pass + 向量化生效 | ⏳ 未开始 | — |
| **M7** | Week 10 末 | Runtime API + im2col+GEMM | ⏳ 未开始 | — |
| **M8** | Week 12 末 | ResNet-18 推理 + 性能 60%+ | ⏳ 未开始 | — |
| **M9** | Week 16 末 | Triton → ACC → CPU E2E | ⏳ 未开始 | — |
| **M10** | Week 20 末 | PyTorch + Triton + ACC E2E | ⏳ 未开始 | — |

---

## 测试覆盖跟踪

| 阶段 | 目标覆盖率 | 当前覆盖率 | 状态 |
|------|-----------|-----------|------|
| Phase 1 | > 70% | — | ⏳ |
| Phase 2 | > 75% | — | ⏳ |
| Phase 3 | > 80% | — | ⏳ |

---

> **最后更新**: 2026-02-16
> **维护者**: ACC Team
