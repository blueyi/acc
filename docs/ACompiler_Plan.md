# ACompiler 项目规划（基于 LLVM/MLIR 的 AI 编译器实践路线）

## 1. 项目目标与范围

### 1.1 目标

ACompiler 是一个基于 LLVM 生态（MLIR + LLVM IR）的 AI 编译器学习与实践项目。它以“可运行的端到端样例”为主线，通过逐步实现 Dialect、Pass、Lowering、LLVM 优化与 CodeGen，使学习者能够快速掌握编译器的核心概念与工程落地方法。

### 1.2 第一阶段范围（必须实现）

- **仅支持 CPU 后端**（优先 x86_64，可扩展到 AArch64）
- 覆盖从“输入（DSL/图）→ MLIR → LLVM IR → 可执行（AOT 或 JIT）”的端到端流水线
- 覆盖 MLIR 的关键概念与实践：Dialect/Op/Type/Pattern、Pass 编写、Conversion/Lowering、Bufferization、Vectorization 等
- 覆盖 LLVM IR 段的关键实践：LLVM Pass pipeline、目标架构 codegen（通过 LLVM 完成）、可选 ORC JIT

### 1.3 后续扩展（写入 TODO，暂不实现）

- **NVIDIA GPU（CUDA）后端支持**（仅规划与目录占位，不在第一阶段实现）
- **华为 Ascend NPU（Ascend IR/算子映射）后端支持**（仅规划与目录占位，不在第一阶段实现）

## 2. 编译器环节拆解（以 LLVM 为例：MLIR + LLVM IR 两大段）

### 2.1 前端（Frontend）

AI 编译器常见输入形态（建议由浅入深）：

- **文本 DSL**：最利于学习 IR 与 Pass（推荐作为第一阶段输入）
- **计算图**：如 ONNX 子集 / 自定义 JSON 图（第二阶段起可引入）
- **语言前端**：词法/语法/AST/语义分析 → IR 生成（更偏传统编译器）

前端产出建议统一导入到 MLIR 的高层 Dialect（本项目自定义 `acomp` Dialect，并逐步 Lower 到 `linalg/tosa/...`）。

### 2.2 MLIR 段（High/Mid-level）

关键组成：

- **Dialect 设计**：Operation/Type/Attribute/Region/Block/SSA；Verifier；Trait/Interface
- **重写与优化**：
  - Pattern rewrite（C++ PatternRewriter / DRR）
  - Canonicalize、CSE、DCE、ConstFold
  - Shape inference（形状推导）
- **Lowering 路线（推荐逐级降低）**：
  - `acomp`（图/张量语义）
  - → `linalg` / `tosa`（结构化算子）
  - → `tensor` / `scf` / `affine`（控制流与循环/内存）
  - → `vector`（向量化）
  - → `memref`（显式内存）
  - → `llvm` dialect
  - → **LLVM IR**

### 2.3 LLVM IR 段（Low-level）

关键组成：

- **LLVM IR 优化（Middle-end）**：instcombine、gvn、simplifycfg、SROA、LICM、loop 系列等（以可配置 pipeline 形式落地）
- **CodeGen（Backend）**：TargetMachine、指令选择、寄存器分配、调度、生成目标文件/汇编，或 ORC JIT 直接执行
- **运行时（Runtime）**：内存管理、ABI、基础算子/库调用封装（如 BLAS/oneDNN 的薄封装）

## 3. 分阶段里程碑规划（由浅入深：先 MLIR，后 LLVM IR）

> 目标是每个阶段都能形成“可运行闭环”，并且每阶段都有可检验的 IR 输出与测试（lit + FileCheck）。

### 阶段 0：工程与最小可运行骨架（1–2 周）

- CMake + Ninja 工程骨架
- 工具链：
  - `acompiler-opt`：类似 `mlir-opt`，用于跑 pass 与调试 IR
  - `acompiler-translate`（可选）：MLIR ↔ LLVM IR 翻译
  - `acompilerc`：driver，一键编译/运行/生成产物
- 测试框架：LLVM `lit` + `FileCheck`

验收：

- 能编译出最小 Dialect + 工具
- `lit` 跑通 1–2 个 smoke 用例

### 阶段 1：自定义 Dialect + 基础优化（2–4 周）

- `acomp` Dialect（先少量核心 op）：
  - `acomp.matmul`, `acomp.conv2d`, `acomp.relu`, `acomp.add`, `acomp.reshape`, `acomp.transpose`
- 类型/属性：张量 element type、shape、可选 layout（第一阶段可简化）
- Pass：
  - Shape inference
  - Canonicalize/ConstFold（pattern + fold）
  - Elementwise 融合（从简单规则开始）

验收：

- DSL/图能导入为 `acomp` MLIR
- Pass 作用前后 IR 变化能被 FileCheck 验证

### 阶段 2：Lowering 到结构化算子（`linalg` 优先）（3–5 周）

- `acomp` → `linalg`（或 `tosa`）
- Bufferization：tensor → memref（先用现成 pipeline，再逐步定制）
- 引入 `scf/affine`：把算子变成可调度的循环

验收：

- `matmul/conv2d` 等示例能够 lower 到 `linalg + scf/affine + memref`

### 阶段 3：面向 CPU 的 MLIR 优化（3–6 周）

- `linalg` tiling（缓存友好）
- `vector` 向量化（SIMD）
- `affine` 循环变换（可选）
- 简单代价模型（可选）：从规则驱动到可配置参数（tile size / vector width）

验收：

- 同一算子优化前后性能可观察提升（提供 micro-bench）

### 阶段 4：Lowering 到 LLVM IR + CPU CodeGen（3–6 周）

- `memref/scf/vector` → `llvm` dialect → LLVM IR
- AOT：生成 object/exe（可选调用系统 clang 链接）
- JIT：ORC JIT（可选，但非常利于快速实验）
- LLVM pass pipeline：提供 `-O0/-O2/-O3` 风格配置

验收：

- 端到端：输入 → 可执行（CPU），输出数值正确
- `acompilerc` 可以一键编译/运行样例

### 阶段 5：工程化与扩展（持续迭代）

- ONNX 子集导入（或与 stablehlo/tosa 等桥接）
- 更完整算子覆盖、动态 shape（先限制再放开）
- 性能回归测试与 CI
- 调试可视化：pass 计时、统计信息、IR dump 约定

## 4. 多后端支持（规划与 TODO）

### 4.1 后端抽象（建议）

- `Target` 抽象：`CPU` / `CUDA` / `Ascend`
- Pipeline 以“目标相关的最后几段 Lowering + runtime”差异化，其余尽量复用

### 4.2 TODO：NVIDIA GPU（CUDA）后端（暂不实现）

- IR 路线：`linalg/vector` → `gpu` dialect → `nvvm` dialect → PTX
- CUDA runtime：kernel launch、memory copy、stream 管理
- 算子 legality 与 layout 策略

### 4.3 TODO：Ascend NPU 后端（暂不实现）

- Ascend IR/Graph/算子映射层（取决于实际 SDK）
- 格式/layout、fusion、量化（如涉及）
- 独立 runtime 与编译调用链封装

## 5. ACompiler 最终源码目录结构（建议）与说明

> 目录会在本仓库以“可扩展骨架”形式创建；其中 CUDA/Ascend 先做占位（TODO），第一阶段只实现 CPU。

```text
ACompiler/
  CMakeLists.txt                 # 顶层构建入口
  cmake/                         # CMake 工具函数、LLVM/MLIR 查找与选项
  third_party/                   # 依赖占位（submodule/FetchContent）
  docs/                          # 学习/工程文档（本规划在此）
  include/acompiler/             # 公共头文件（对外 API + 内部模块接口）
  lib/                           # 主要实现（Dialect/Pass/Lowering/Target/Runtime）
  tools/                         # 可执行工具（opt/translate/driver）
  examples/                      # 示例：matmul/conv/attention 等
  benchmarks/                    # 性能基准
  test/                          # lit + FileCheck 测试
  scripts/                       # 开发脚本（格式化、跑测试等）
  .github/workflows/             # CI（可选但建议）
  README.md                      # 项目入口：快速开始、里程碑、贡献指南
```

关键说明：

- `lib/Dialect`、`lib/Passes`、`lib/Lowering` 是 MLIR 学习与实现的主战场。
- `lib/Target/CPU` 是第一阶段唯一落地后端；`CUDA/Ascend` 仅目录与接口占位。
- `tools/acompiler-opt` 是调试与学习 IR 的第一工具，建议优先完成并贯穿所有阶段。

