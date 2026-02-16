# ACompiler 项目规划

## 一、项目概述

**ACompiler** 是一个基于 LLVM 实现的 AI 编译器项目，旨在通过实例实践的方式快速学习和掌握编译器知识。项目采用自上而下、由浅入深的学习路径，从 MLIR 入手，逐步深入到 LLVM IR 优化和 Codegen 环节。

### 核心目标

1. **教育导向**：通过实践理解现代编译器架构
2. **AI 编译器聚焦**：面向 AI/ML 工作负载的编译优化
3. **多后端架构**：设计支持多目标后端的扩展架构

---

## 二、编译器实现环节分析（以 LLVM 为例）

基于 LLVM 生态，编译器实现可分为以下核心环节：

### 2.1 高层环节：MLIR

MLIR (Multi-Level Intermediate Representation) 提供多层次的中间表示，适合 AI/ML 领域的表达与优化。

| 环节 | 描述 | 关键技术 |
|------|------|----------|
| Dialect 定义 | 定义领域特定的操作和类型系统 | TableGen、ODS |
| 变换与 Lowering | 高层 IR 向低层 IR 的转换 | Conversion Pass、Pattern Rewrite |
| 图优化 | 算子融合、布局优化等 | Canonicalize、CSE |
| 调度与流水线 | 循环变换、tiling 等 | Affine Dialect、Loop Transform |

### 2.2 底层环节：LLVM IR

| 环节 | 描述 | 关键技术 |
|------|------|----------|
| LLVM IR 前端 | 生成 LLVM IR | MLIR-to-LLVM Lowering |
| Pass 优化 | 标量/向量/循环优化 | PassManager、Analysis |
| 指令选择 | IR 到 DAG 再到机器指令 | SelectionDAG、GlobalISel |
| 寄存器分配 | 虚拟寄存器到物理寄存器 | RegAlloc Fast/Greedy |
| 代码生成 | 生成目标机器码 | AsmPrinter、MC Layer |

---

## 三、ACompiler 学习路径设计

```
┌─────────────────────────────────────────────────────────────────┐
│                    ACompiler 学习路径                            │
├─────────────────────────────────────────────────────────────────┤
│  Phase 1: MLIR 入门                                               │
│  ├── 1.1 工程编译与环境搭建                                       │
│  ├── 1.2 MLIR 核心概念 (Dialect, Operation, Region, Block)       │
│  ├── 1.3 自定义 Dialect 开发                                      │
│  └── 1.4 MLIR Pass 编写与优化                                     │
├─────────────────────────────────────────────────────────────────┤
│  Phase 2: MLIR 进阶                                               │
│  ├── 2.1 图级别优化 (算子融合、常量折叠)                          │
│  ├── 2.2 Lowering 到 Affine/Linalg                                │
│  └── 2.3 调度与循环变换                                           │
├─────────────────────────────────────────────────────────────────┤
│  Phase 3: LLVM IR 与 Codegen                                      │
│  ├── 3.1 MLIR 到 LLVM IR Lowering                                │
│  ├── 3.2 LLVM Pass 优化                                           │
│  ├── 3.3 指令映射与选择                                           │
│  └── 3.4 CPU Codegen 实现                                        │
└─────────────────────────────────────────────────────────────────┘
```

---

## 四、项目阶段详细规划

### 第一阶段：工程编译与 MLIR 基础（CPU 后端）

#### 4.1 项目工程编译

- **目标**：搭建可编译、可运行的最小 ACompiler 环境
- **内容**：
  - CMake 构建系统配置
  - LLVM/MLIR 作为依赖（子模块或外部依赖）
  - 最小 Dialect 注册与 `mlir-opt` 集成
  **产出**：`acompiler-opt` 工具可加载自定义 Dialect

#### 4.2 MLIR 关键技术概念

- **目标**：掌握 MLIR 核心抽象
- **内容**：
  - **Dialect**：命名空间与操作分组
  - **Operation**：操作定义、属性、区域
  - **Region/Block**：控制流与数据流表示
  - **Type**：类型系统与自定义类型
  - **Attribute**：编译时常量表示
- **产出**：文档 + 示例 Dialect（如 `ACDialect`）

#### 4.3 MLIR 优化 Pass 编写

- **目标**：能够编写并运行 MLIR Pass
- **内容**：
  - OpRewritePattern：模式匹配与重写
  - Pass 注册与 Pipeline 配置
  - 常量折叠、公共子表达式消除
  - 简单算子融合 Pass
- **产出**：多个可复用的 MLIR Pass

#### 4.4 Lowering 到 LLVM IR

- **目标**：将 MLIR 表示 Lower 到 LLVM IR
- **内容**：
  - Conversion 框架使用
  - 从高层 Dialect 到 `builtin`/`arith`/`memref` 的 Lowering
  - 从 `memref`/`arith` 到 `llvm` Dialect 的 Lowering
- **产出**：完整的 Lowering Pipeline

#### 4.5 LLVM Pass 优化与 Codegen

- **目标**：完成 CPU 后端代码生成
- **内容**：
  - LLVM Pass 集成（标量/向量优化）
  - 指令选择与寄存器分配（复用 LLVM 后端）
  - 生成 x86_64 / AArch64 汇编
- **产出**：`acompiler-compile` 可编译输入到可执行文件

---

### 第二阶段：TODO - 扩展后端（暂不实现）

以下后端在规划中预留，第一阶段不实现：

1. **NVIDIA GPU - CUDA 支持**
   - Lowering 到 GPU Dialect / NVVM
   - Kernel 发射与内存管理
   - 与 CUDA Driver/Runtime 集成

2. **华为 Ascend NPU - Ascend IR 支持**
   - Ascend 自定义 Dialect
   - 映射到 CANN 图/算子
   - Ascend 运行时集成

---

## 五、最终源码目录结构

```
ACompiler/
├── CMakeLists.txt              # 根 CMake 配置
├── README.md                   # 项目说明
├── cmake/                      # CMake 模块
│   ├── FindLLVM.cmake
│   ├── FindMLIR.cmake
│   └── ACompilerOptions.cmake
│
├── include/                    # 公共头文件
│   └── ACompiler/
│       ├── Version.h
│       └── Common.h
│
├── lib/                        # 核心库实现
│   ├── Dialect/                # MLIR Dialect 定义
│   │   ├── AC/                 # ACompiler 自定义 Dialect
│   │   │   ├── AC dialect 定义 (TableGen)
│   │   │   ├── ACOps.h / ACOps.cpp
│   │   │   └── ACTypes.h / ACTypes.cpp
│   │   └── CMakeLists.txt
│   │
│   ├── Conversion/             # MLIR Conversion Pass
│   │   ├── ACToStandard/       # AC -> Standard/Builtin
│   │   ├── ACToArith/          # AC -> Arith
│   │   ├── ACToMemref/         # AC -> MemRef
│   │   ├── ACToLLVM/           # 到 LLVM Dialect
│   │   └── CMakeLists.txt
│   │
│   ├── Transforms/             # MLIR 优化 Pass
│   │   ├── Canonicalize/
│   │   ├── Fusion/             # 算子融合
│   │   ├── ConstantFold/
│   │   └── CMakeLists.txt
│   │
│   └── Target/                 # 目标后端（多后端架构）
│       ├── CPU/                # CPU 后端
│       │   ├── CPUPasses.h
│       │   └── CPUPasses.cpp
│       ├── GPU/                # TODO: NVIDIA CUDA 后端
│       │   └── (暂不实现)
│       ├── NPU/                # TODO: 华为 Ascend NPU 后端
│       │   └── (暂不实现)
│       └── CMakeLists.txt
│
├── tools/                      # 可执行工具
│   ├── acompiler-opt/          # MLIR 优化工具（类似 mlir-opt）
│   │   ├── ACompilerOpt.cpp
│   │   └── CMakeLists.txt
│   └── acompiler-compile/      # 端到端编译工具
│       ├── ACompilerCompile.cpp
│       └── CMakeLists.txt
│
├── test/                       # 测试
│   ├── Dialect/                # Dialect 单元测试
│   ├── Conversion/             # Conversion 测试
│   ├── Transforms/             # Pass 测试
│   └── lit.cfg.py              # LIT 配置
│
├── examples/                   # 示例与教程
│   ├── hello_dialect/          # 最小 Dialect 示例
│   ├── simple_pass/            # 简单 Pass 示例
│   └── end_to_end/             # 端到端示例
│
└── docs/                       # 文档
    ├── PROJECT_PLAN.md         # 本规划文档
    ├── MLIR_Concepts.md        # MLIR 概念详解
    ├── Pass_Writing_Guide.md   # Pass 编写指南
    └── Lowering_Guide.md       # Lowering 指南
```

---

## 六、目录说明

| 目录/文件 | 说明 |
|-----------|------|
| `cmake/` | CMake 辅助模块，用于查找 LLVM、MLIR 依赖及编译选项配置 |
| `include/ACompiler/` | 项目公共头文件，版本信息、通用宏与工具定义 |
| `lib/Dialect/AC/` | ACompiler 自定义 MLIR Dialect，包含算子定义（ODS）和类型定义 |
| `lib/Conversion/` | MLIR Conversion Pass，实现高层 IR 到低层 IR 的 Lowering 链路 |
| `lib/Transforms/` | MLIR 优化 Pass，如算子融合、常量折叠、规范化等 |
| `lib/Target/CPU/` | CPU 后端实现，负责 LLVM IR 优化及 Codegen |
| `lib/Target/GPU/` | **TODO**：NVIDIA CUDA 后端，第一阶段不实现 |
| `lib/Target/NPU/` | **TODO**：华为 Ascend NPU 后端，第一阶段不实现 |
| `tools/acompiler-opt/` | MLIR 级优化工具，加载 Dialect 与 Pass，类似 `mlir-opt` |
| `tools/acompiler-compile/` | 端到端编译工具，从输入到可执行文件/目标码 |
| `test/` | 各模块的单元测试与集成测试，使用 LIT 框架 |
| `examples/` | 教学示例，从 Dialect 到端到端编译的完整流程 |
| `docs/` | 项目文档，包含规划、概念与使用指南 |

---

## 七、依赖与构建

### 依赖

- **LLVM** (>= 16.0)：包含 LLVM 核心、Support、IR
- **MLIR**：与 LLVM 同源，需包含 MLIR 库及 mlir-tblgen 等工具

### 构建命令

```bash
mkdir build && cd build
cmake -G Ninja ../ -DLLVM_DIR=<path_to_llvm> -DMLIR_DIR=<path_to_mlir>
ninja
```

---

## 八、里程碑

| 里程碑 | 内容 | 预计完成 |
|--------|------|----------|
| M1 | 工程编译通过，`acompiler-opt` 可运行 | 第 1 周 |
| M2 | AC Dialect 完成，可解析/打印 AC IR | 第 2 周 |
| M3 | 至少 2 个 MLIR Pass 完成 | 第 3 周 |
| M4 | Lowering 到 LLVM IR 打通 | 第 4 周 |
| M5 | CPU 端到端编译成功 | 第 5 周 |

---

## 九、TODO 清单（扩展后端）

- [ ] **NVIDIA GPU - CUDA 支持**：Lowering 到 GPU/NVVM，Kernel 发射，CUDA Runtime 集成
- [ ] **华为 Ascend NPU - Ascend IR 支持**：Ascend Dialect，CANN 图映射，Ascend 运行时

以上两个后端在第一阶段仅作为 TODO 记录，不做实现。
