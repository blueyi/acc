# ACompiler 项目详细规划（LLVM/MLIR AI 编译器）

## 1. 项目目标与学习方法

## 1.1 总体目标

基于 LLVM 生态实现一个可运行的 AI 编译器原型 `ACompiler`，通过真实工程与实例实践，掌握以下关键能力：

1. 编译器工程搭建与构建（LLVM/MLIR 工程化）
2. MLIR 表达、转换与优化（Dialect + Pass + Lowering）
3. LLVM IR 优化、目标相关转换与 CodeGen
4. 端到端执行验证（模型输入 -> 编译 -> 二进制 -> 推理结果）

## 1.2 设计原则

- **由浅入深**：先打通链路，再增强优化深度。
- **自上而下**：先完成前端语义和高层 IR，再逐层下沉到 LLVM IR 与目标代码。
- **实例驱动**：每阶段都有“小模型样例 + 可观测 IR + 可测结果”。
- **单后端优先**：第一阶段仅 CPU，保证闭环稳定。

---

## 2. 编译器环节拆分：MLIR 与 LLVM IR 两大主线

## 2.1 MLIR 环节（高层到中层）

目标：将 AI 计算语义从高层算子表达逐步 Lower 到接近机器执行的中间形式。

核心内容：

1. **IR 建模**
   - Tensor/Shape/Memory Layout
   - 控制流（如循环、分支）
2. **Dialect 设计**
   - `acompiler` 自定义 Dialect（AI 算子与属性）
3. **Pass 体系**
   - Canonicalization、CSE、常量折叠
   - 算子融合（如 Conv+Bias+ReLU、Elementwise 融合）
   - Shape Inference
4. **Lowering**
   - `acompiler` Dialect -> `linalg`/`tensor`/`scf`/`arith`
   - 再 Lower 到 `llvm` Dialect（可生成 LLVM IR）

## 2.2 LLVM IR 环节（中层到底层）

目标：围绕目标平台（先 CPU）完成 LLVM IR 优化与代码生成。

核心内容：

1. **LLVM IR 优化**
   - 常见优化：GVN、LICM、Loop Unroll、Vectorize
   - 针对 AI 计算图生成 IR 的模式优化
2. **指令映射与目标相关处理**
   - LLVM IR 到目标指令选择（SelectionDAG/GlobalISel 路径理解）
   - CPU 特性感知（SIMD 指令、向量宽度）
3. **CodeGen**
   - 目标文件生成、链接、运行时调用
   - 产物校验（数值正确性 + 性能基线）

---

## 3. 分阶段实施计划（建议 5 个阶段）

## 阶段 0：工程基线与构建体系（1~2 周）

目标：建立稳定、可复现、可调试的开发环境。

交付物：

- 统一构建系统（CMake + Ninja）
- LLVM/MLIR 版本固定策略（Tag/Commit）
- 开发脚本（格式化、测试、IR dump）
- 最小可运行样例（Hello MLIR Pass）

关键任务：

1. LLVM/MLIR 编译（Debug 与 Release）
2. 项目模板生成（lib/include/tools/tests）
3. 单元测试框架接入（lit/FileCheck + gtest）

验收标准：

- 一条命令构建成功
- 能跑通一个自定义 Pass 并验证 IR 输出

## 阶段 1：MLIR 入门与高层 IR 建模（2~3 周）

目标：让 ACompiler 能表达 AI 子图（先聚焦常见算子）。

交付物：

- `acompiler` Dialect（基础算子与类型）
- 样例模型：`matmul`、`conv2d`、`relu`、`add`
- IR Parser/Printer（文本 IR 可读写）

关键任务：

1. 定义 Op/Type/Attr（TableGen）
2. 编写 Verifier（维度、类型检查）
3. 基础语义测试（解析、打印、一致性）

验收标准：

- 可稳定生成并解析 ACompiler MLIR
- 基础算子语义校验通过

## 阶段 2：MLIR 优化 Pass 与 Lowering（3~4 周）

目标：实现可观收益的 MLIR 优化，并下沉到 LLVM Dialect。

交付物：

- Pass Pipeline v1
- 关键优化 Pass（算子融合、常量折叠、形状推导）
- Lowering Pipeline（`acompiler` -> `linalg/scf/arith` -> `llvm`）

关键任务：

1. Pass 编写与注册（Pipeline 可配置）
2. Pattern Rewriter 与 Greedy Rewrite 应用
3. 性能/IR 体积对比工具（优化前后）

验收标准：

- 至少 3 个核心 Pass 可独立启停
- 目标样例在 MLIR 层有可观优化效果
- 可生成合法 LLVM Dialect

## 阶段 3：LLVM IR 优化、指令映射与 CPU CodeGen（3~4 周）

目标：从 LLVM Dialect/LLVM IR 到可执行 CPU 程序。

交付物：

- LLVM IR 优化配置（`opt` pipeline）
- CPU 目标产物生成（obj/so/bin）
- 端到端运行脚本与数值校验

关键任务：

1. MLIR 翻译到 LLVM IR（`mlir-translate` 或集成 API）
2. LLVM Pass 管线调优（性能与编译时间平衡）
3. 目标架构参数控制（`-march/-mcpu`）
4. 运行时接口（内存分配、张量布局、调用 ABI）

验收标准：

- 样例模型可编译为 CPU 可执行产物
- 推理结果与参考实现一致
- 提供基础性能报告

## 阶段 4：LLVM 前端实践与工程增强（2~3 周）

目标：补齐“语言前端 -> IR -> 优化 -> 后端”完整认知链。

交付物：

- Toy 前端（可选简化 DSL 或表达式语言）
- 前端 AST -> MLIR/LLVM IR 路径示例
- 调试与可视化工具链（IR dump、pass trace）

关键任务：

1. 词法/语法到 AST（最小子集）
2. AST Lower 到 ACompiler Dialect 或直接 LLVM IR（教学路径）
3. 增加 CI（构建 + 测试 + 样例验证）

验收标准：

- 新增一个“从源码到二进制”的可教学 Demo
- CI 能自动验证关键链路

---

## 4. 实例实践路线（建议从易到难）

1. **Elementwise Add + ReLU**  
   重点：类型系统、广播规则、基础融合。
2. **MatMul**  
   重点：循环结构、向量化、内存访问模式。
3. **Conv2D + Bias + ReLU**  
   重点：多算子融合、layout 处理、cache 友好。
4. **小型 MLP 子图**  
   重点：多 Pass 协同、端到端基准测试。

每个实例都应输出：

- 原始 MLIR
- 优化后 MLIR
- LLVM IR
- CPU 可执行产物
- 数值正确性报告 + 性能对比

---

## 5. 关键技术清单（学习/实现并行）

## 5.1 MLIR 必学

- Dialect/Op/Type/Attr（TableGen）
- RewritePattern 与 PatternRewriter
- PassManager 与 Pipeline 组织
- ConversionTarget + TypeConverter + Lowering
- linalg/scf/arith/memref/llvm 常用 Dialect

## 5.2 LLVM 必学

- LLVM IR 结构与 SSA
- LLVM Pass 基础与常用优化
- TargetMachine、DataLayout、Triple
- 指令选择与寄存器分配基本流程
- CodeGen 流程与目标文件生成

---

## 6. 质量保障与工程规范

- 测试分层：
  - 单元测试（Verifier、工具函数）
  - IR 测试（lit + FileCheck）
  - 端到端测试（输入输出一致性）
- 性能基线：
  - 对每个实例记录优化前后耗时与 IR 大小
- 工程规范：
  - 模块解耦（Frontend/Passes/Backend/Runtime）
  - 文档同步（每个 Pass 有设计说明）

---

## 7. 多后端策略

## 第一阶段（实现）

- `CPU`：完整实现并作为主线验证后端

## 后续阶段（仅规划，不实现）

- `NVIDIA GPU (CUDA)`：列入 TODO，暂不实现
- `Huawei AscendNPU IR`：列入 TODO，暂不实现

详细条目见根目录 `TODO.md`。

---

## 8. ACompiler 最终实现源码目录（规划版）

> 以下为“最终形态”的建议目录结构，当前阶段可按里程碑逐步落地。

```text
ACompiler/
├── CMakeLists.txt
├── README.md
├── TODO.md
├── third_party/
│   └── llvm-project/                 # 可选：子模块或外部依赖说明
├── cmake/
│   ├── LLVMConfigHelpers.cmake       # LLVM/MLIR 查找与配置封装
│   └── ACompilerOptions.cmake        # 编译选项、告警、Sanitizer
├── include/
│   └── acompiler/
│       ├── Frontend/
│       │   ├── AST.h
│       │   ├── Lexer.h
│       │   └── Parser.h
│       ├── Dialect/
│       │   └── ACompiler/
│       │       ├── ACompilerDialect.h
│       │       ├── ACompilerOps.h
│       │       ├── ACompilerTypes.h
│       │       └── ACompilerAttrs.h
│       ├── Conversion/
│       │   ├── ACompilerToLinalg.h
│       │   ├── ACompilerToSCF.h
│       │   └── ACompilerToLLVM.h
│       ├── Transforms/
│       │   ├── Passes.h
│       │   ├── Fusion.h
│       │   ├── ShapeInference.h
│       │   └── CanonicalizeExt.h
│       ├── Backend/
│       │   ├── CPU/
│       │   │   ├── TargetInfo.h
│       │   │   ├── LLVMIRPipeline.h
│       │   │   └── Codegen.h
│       │   ├── CUDA/                  # TODO: 暂不实现
│       │   └── AscendNPU/             # TODO: 暂不实现
│       └── Runtime/
│           ├── Tensor.h
│           └── Executor.h
├── lib/
│   └── acompiler/
│       ├── Frontend/
│       │   ├── Lexer.cpp
│       │   ├── Parser.cpp
│       │   └── AST.cpp
│       ├── Dialect/
│       │   └── ACompiler/
│       │       ├── ACompilerDialect.cpp
│       │       ├── ACompilerOps.cpp
│       │       ├── ACompilerTypes.cpp
│       │       └── ACompilerAttrs.cpp
│       ├── Conversion/
│       │   ├── ACompilerToLinalg.cpp
│       │   ├── ACompilerToSCF.cpp
│       │   └── ACompilerToLLVM.cpp
│       ├── Transforms/
│       │   ├── Fusion.cpp
│       │   ├── ShapeInference.cpp
│       │   ├── CanonicalizeExt.cpp
│       │   └── PassRegistration.cpp
│       ├── Backend/
│       │   ├── CPU/
│       │   │   ├── LLVMIRPipeline.cpp
│       │   │   └── Codegen.cpp
│       │   ├── CUDA/                  # TODO: 暂不实现
│       │   └── AscendNPU/             # TODO: 暂不实现
│       └── Runtime/
│           ├── Tensor.cpp
│           └── Executor.cpp
├── tools/
│   ├── acompiler-opt/                # 类似 mlir-opt 的优化驱动
│   │   └── main.cpp
│   ├── acompiler-translate/          # IR 翻译工具
│   │   └── main.cpp
│   ├── acompiler-c/                  # 前端编译入口（CLI）
│   │   └── main.cpp
│   └── acompiler-run/                # 运行时执行工具
│       └── main.cpp
├── runtime/
│   ├── cpu/
│   │   ├── allocator.cpp
│   │   └── kernels.cpp
│   └── include/
│       └── acompiler_runtime.h
├── examples/
│   ├── mlir/
│   │   ├── elementwise_add_relu.mlir
│   │   ├── matmul.mlir
│   │   └── conv2d_bias_relu.mlir
│   ├── model/
│   │   └── tiny_mlp.json
│   └── scripts/
│       ├── run_cpu_demo.sh
│       └── dump_all_ir.sh
├── test/
│   ├── lit.cfg.py
│   ├── Dialect/
│   ├── Transforms/
│   ├── Conversion/
│   ├── Backend/
│   └── E2E/
├── benchmark/
│   ├── cpu/
│   │   ├── bench_matmul.cpp
│   │   └── bench_conv.cpp
│   └── reports/
│       └── baseline.md
├── docs/
│   ├── ACompiler_Project_Plan.md
│   ├── MLIR_Key_Concepts.md
│   ├── Pass_Development_Guide.md
│   ├── LLVM_Backend_Notes.md
│   └── Roadmap.md
└── scripts/
    ├── build_llvm.sh
    ├── configure.sh
    ├── build.sh
    ├── test.sh
    └── format.sh
```

## 目录说明（按模块）

- `include/` + `lib/`：核心源码，按 Frontend/Dialect/Conversion/Transforms/Backend/Runtime 分层。
- `tools/`：编译器工具入口，覆盖优化、翻译、编译、运行。
- `runtime/`：运行时支持（内存、内核封装、调用接口）。
- `examples/`：教学与回归样例，保证“改动可见、结果可验”。
- `test/`：IR 级与端到端测试，支撑持续迭代。
- `benchmark/`：性能基线与回归报告。
- `docs/`：学习材料与设计文档沉淀。
- `scripts/`：构建、测试、格式化、开发自动化脚本。

---

## 9. 近期执行建议（可直接开工）

1. 先完成阶段 0 的构建脚手架与测试链路。
2. 以 `elementwise_add_relu` 作为第一条端到端样例（最小闭环）。
3. 先写 `ShapeInference + Fusion` 两个 Pass，形成“有优化收益”的第一版。
4. 打通 CPU codegen 后再扩充更多算子与模型。

这样可以在最短路径内获得“能编译、能运行、能优化、能验证”的完整成就点。
