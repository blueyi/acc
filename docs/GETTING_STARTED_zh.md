# ACC 项目入门指南 / ACC Getting Started Guide

> 结合下文「项目进度与任务清单」一步步开始本项目，并了解项目结构与代码。  
> Step-by-step guide to start the ACC project and understand its structure.

---

## 一、当前进度与建议起点 / Current Progress & Where to Start

根据 **项目进度与任务清单**（见第八节）：

- **Phase 1** 进行中（约 0% 进度条）
- **1.1 项目工程搭建**：1.1.1–1.1.3、1.1.5–1.1.6 已完成；**待做**：1.1.4 单元测试框架（GoogleTest）、验收（无警告编译、`ac-opt --version`、测试与 CI）
- **1.2 MLIR 基础**：方言与多数算子已定义；**待做**：1.2.7 算子 verify()、1.2.8 形状推导接口、1.2.10 方言测试
- **1.3 Pass 开发**、**1.4 前端** 尚未开始

**建议起点**：先跑通环境与测试，再按 1.1.4 → 1.2.7/1.2.8 → 1.3 顺序推进。

---

## 二、环境与构建（对应 TODO 1.1）/ Environment & Build

### 2.1 依赖

- **LLVM 17+** 与 **MLIR**（项目可通过脚本从源码构建）
- **CMake 3.20+**、**Ninja**、**C++17** 编译器
- （可选）Python 3、`lit`（跑 lit 测试）

### 2.2 一键构建

```bash
# 在项目根目录
./scripts/build.sh
```

脚本会：

1. 若未检测到 `$LLVM_INSTALL_DIR`（默认 `$HOME/llvm-install`），先执行 `scripts/build_llvm.sh` 编译 LLVM/MLIR  
2. 在 `build/` 下用 Ninja 配置并编译 ACC

如需指定 LLVM 安装路径：

```bash
LLVM_INSTALL_DIR=/path/to/llvm-install ./scripts/build.sh
```

### 2.3 验收（TODO 1.1 验收检查）

```bash
# 1. 无警告编译：构建时观察无 warning
./scripts/build.sh

# 2. ac-opt 可用
./build/bin/ac-opt --version

# 3. 工具入 PATH（可选）
source scripts/acc_env.sh
ac-opt --version
acc --help
```

---

## 三、项目结构速览 / Project Structure Overview

```
acc/
├── CMakeLists.txt              # 顶层 CMake，拉取 LLVM/MLIR、子目录、测试
├── include/ACC/                # 公开头文件与 TableGen 定义
│   ├── Dialect/
│   │   ├── ACHigh/             # 高层 AI 方言（Conv2D, MatMul, ReLU, GELU...）
│   │   │   ├── ACHighDialect.td, ACHighOps.td, ACHighTypes.td
│   │   │   └── *.h
│   │   ├── ACMid/              # 中层 Tile/Loop 方言
│   │   └── ACGPU/              # [TODO] GPU 方言
│   ├── Conversion/             # ACHigh→ACMid→Linalg→LLVM 等 Lowering
│   ├── Transforms/             # MLIR Pass：融合、常量折叠、形状推导等
│   ├── Frontend/               # Lexer/Parser/AST、ONNX/Triton/Torch 导入
│   ├── IROptimizer/            # LLVM IR 层 Pass
│   ├── CodeGen/                # 代码生成、JIT
│   └── InitAllDialects.h
├── lib/                        # 实现（与 include/ACC 模块对应）
│   ├── Dialect/ACHigh, ACMid
│   ├── Conversion/, Transforms/, Frontend/, IROptimizer/, CodeGen/, Support/
├── runtime/                    # Runtime C API + CPU Kernels（im2col+GEMM 等）
├── tools/                      # 可执行工具
│   ├── acc-compiler/           # 主编译器驱动（二进制名 acc）
│   ├── ac-opt/                 # MLIR 优化（类似 mlir-opt）
│   ├── ac-translate/           # IR 翻译、ONNX 导入等
│   └── ac-runner/              # JIT 执行
├── test/                       # lit + FileCheck 测试
├── unittests/                  # C++ 单元测试（GoogleTest）
├── examples/e2e/               # 端到端示例（Triton→ACC、PyTorch+Triton+ACC）
├── scripts/
│   ├── build_llvm.sh           # 编译 LLVM/MLIR
│   ├── build.sh                # 编译 ACC
│   ├── acc_env.sh              # 将 build/bin 加入 PATH
│   └── run_tests.sh            # 运行测试
└── docs/                       # PROJECT_PLAN, DIRECTORY_STRUCTURE 等
```

更细的目录说明见 [项目规划](PROJECT_PLAN_zh) 中「二、项目目录结构」。

---

## 四、核心代码入口 / Where to Read the Code

| 目标 | 位置 | 说明 |
|------|------|------|
| **ACHigh 算子定义** | `include/ACC/Dialect/ACHigh/ACHighOps.td` | TableGen：MatMul, Conv2D, ReLU, GELU, BatchNorm, Dense... |
| **ACHigh 类型** | `include/ACC/Dialect/ACHigh/ACHighTypes.td` | Tensor、量化类型等 |
| **ACMid 方言** | `include/ACC/Dialect/ACMid/*.td` | Tile/Loop 级抽象 |
| **MLIR Pass 注册** | `include/ACC/Transforms/Passes.td`, `Passes.h` | 融合、常量折叠、形状推导、布局、DCE |
| **Lowering** | `include/ACC/Conversion/*/` | ACHighToACMid, ACMidToLinalg, ACMidToLLVM |
| **ac-opt 入口** | `tools/ac-opt/main.cpp` | 注册 Dialect、Pass、解析 MLIR、跑 Pipeline |
| **ac-translate** | `tools/ac-translate/main.cpp` | `--import-onnx` 等 |
| **acc 主编译器** | `tools/acc-compiler/` | 驱动入口 |
| **方言测试** | `test/Dialect/ACHigh/ops.mlir` | 用 FileCheck 验证 achigh.matmul/conv2d/relu/add |
| **Pass 测试** | `test/Transforms/*.mlir` | 各 Transform 的 FileCheck |
| **Conversion 测试** | `test/Conversion/ACHighToACMid/basic.mlir` | Lowering 测试 |

---

## 五、运行测试（对应 TODO 1.1 验收）/ Running Tests

```bash
# 推荐：先构建再跑全量测试
./scripts/run_tests.sh

# 仅运行测试（假定已构建）
./scripts/run_tests.sh --no-build
```

或在 build 目录：

```bash
cd build && ninja check-acc
```

若提示找不到 `llvm-lit`，可：

- 使用已安装 LLVM 的 `llvm-lit`：  
  `cmake -DLLVM_EXTERNAL_LIT=/path/to/llvm-install/bin/llvm-lit ...`
- 或安装 lit：`pip install lit`，再 `./scripts/run_tests.sh --no-build`

**TODO 验收**：GoogleTest 和 lit 测试框架均可运行、CI 自动构建成功。

---

## 六、端到端示例（无需 C++ 构建）/ E2E Examples (No Build)

```bash
# Triton MatMul → ACC → CPU 验证
python3 examples/e2e/triton_matmul_to_acc.py

# PyTorch + Triton + ACC 端到端
python3 examples/e2e/pytorch_triton_acc_e2e.py
```

---

## 七、建议的下一步（对齐 TODO）/ Suggested Next Steps

1. **完成 1.1.4 与 1.1 验收**  
   - 集成 GoogleTest，确保 `ninja check-acc` 与 CI 通过，项目无警告编译、`ac-opt --version` 正常。

2. **1.2.7 / 1.2.8 / 1.2.10**  
   - 为 ACHigh 算子实现 `verify()`，为 Conv2D 等实现 `InferTypeOpInterface`，补充方言 FileCheck 测试。

3. **1.3 MLIR Pass**  
   - 按 TODO 实现常量折叠、算子融合、形状推导、布局变换、DCE 等 Pass，并各写 FileCheck 测试。

4. **1.4 前端**  
   - ONNX 导入与 DSL 前端（Lexer/Parser/AST → MLIR）。

文档与进度以本文第八节「项目进度与任务清单」为准，本文兼作入门与索引。

---

## 八、项目进度与任务清单 / Progress & TODO

> 完成任务后将 `[ ]` 改为 `[x]`，并在备注中记录完成日期或相关 PR。  
> 进度概览：Phase 1 → Phase 2 → Phase 3 → Phase 4 → Phase 5 → Phase 6。

### 总体进度

| 阶段 | 状态 | 进度 | 周期 |
|------|------|------|------|
| Phase 1: 基础架构与 MLIR 入门 | 🚧 进行中 | ░░░░░░░░░░ 0% | Week 1-4 |
| Phase 2: MLIR Lowering 与 LLVM IR 优化 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 5-8 |
| Phase 3: CodeGen、Runtime 与 E2E | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 9-12 |
| Phase 4: 高级特性与多后端扩展 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 13-14 |
| Phase 5: Triton / PyTorch 框架对接 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 15-20 |
| Phase 6: 高阶特性 | ⏳ 未开始 | ░░░░░░░░░░ 0% | Week 20+ |

### Phase 1: 基础架构与 MLIR 入门（Week 1-4）

**1.1 项目工程搭建（Week 1）**

- [x] **1.1.1** LLVM/MLIR 源码编译
- [x] **1.1.2** CMake 工程搭建
- [x] **1.1.3** 项目目录结构设计
- [ ] **1.1.4** 单元测试框架搭建 — 集成 GoogleTest
- [x] **1.1.5** lit/FileCheck 测试搭建
- [x] **1.1.6** CI/CD 配置

**验收**：无警告编译、`ac-opt --version` 正常、GoogleTest 与 lit 可运行、CI 成功。

**1.2 MLIR 基础（Week 2）**

- [ ] **1.2.1** MLIR 核心概念学习
- [x] **1.2.2** ACHigh Dialect 定义
- [x] **1.2.3** 基本 AI 算子定义
- [x] **1.2.4** 增强算子定义
- [x] **1.2.5** Type System 设计
- [x] **1.2.6** 量化类型支持
- [ ] **1.2.7** 算子验证逻辑 verify()
- [ ] **1.2.8** 形状推导接口 InferTypeOpInterface
- [x] **1.2.9** ACMid Dialect 定义
- [ ] **1.2.10** 方言测试

**1.3 MLIR Pass 开发（Week 3）**

- [ ] **1.3.1** Pass 管理框架学习
- [ ] **1.3.2** 常量折叠 Pass
- [ ] **1.3.3** 算子融合 Pass
- [ ] **1.3.4** 形状推导 Pass
- [ ] **1.3.5** 布局变换 Pass
- [ ] **1.3.6** 死代码消除 Pass
- [ ] **1.3.7** DRR 模式匹配
- [ ] **1.3.8** Pass Pipeline 管理
- [ ] **1.3.9** Pass 测试

**1.4 前端支持（Week 4）**

- [ ] **1.4.1** ONNX 导入器设计
- [ ] **1.4.2** ONNX 算子映射（20+）
- [ ] **1.4.3** 权重/常量处理
- [ ] **1.4.4** DSL Lexer
- [ ] **1.4.5** DSL Parser
- [ ] **1.4.6** AST 设计与构建
- [ ] **1.4.7** 语义分析
- [ ] **1.4.8** AST → MLIR 生成
- [ ] **1.4.9** 前端测试

### Phase 2–6 任务概要

- **Phase 2**（Week 5-8）：ACHigh → ACMid → Linalg → LLVM Dialect Lowering，LLVM IR 优化 Pass，指令选择与 SIMD。
- **Phase 3**（Week 9-12）：CPU CodeGen，Runtime 库，端到端执行与性能优化。
- **Phase 4**（Week 13-14）：高级特性，CUDA/Ascend 后端规划。
- **Phase 5**（Week 15-20）：Triton/PyTorch 对接，E2E 验证。
- **Phase 6**（Week 20+）：Auto-Scheduling、动态形状、高阶特性。

完整任务列表与验收标准见 [项目规划](PROJECT_PLAN_zh) 中「三、项目分期规划」。

### 里程碑与测试覆盖

| 里程碑 | 目标时间 | 目标 | 状态 |
|--------|---------|------|------|
| M1 | Week 1 末 | 环境就绪，项目骨架完成 | 🚧 进行中 |
| M2 | Week 2 末 | ACHigh 20+ 算子定义完成 | ⏳ 未开始 |
| M3 | Week 3 末 | 5 个优化 Pass 实现 | ⏳ 未开始 |
| M4 | Week 4 末 | ONNX 20+ 算子导入 + DSL 前端 | ⏳ 未开始 |
| M5–M8 | Week 6–12 末 | Lowering、LLVM 优化、Runtime、ResNet-18 推理 | ⏳ 未开始 |

| 阶段 | 目标覆盖率 | 状态 |
|------|-----------|------|
| Phase 1 | > 70% | ⏳ |
| Phase 2 | > 75% | ⏳ |
| Phase 3 | > 80% | ⏳ |

---

**最后更新**: 2026-03-01
