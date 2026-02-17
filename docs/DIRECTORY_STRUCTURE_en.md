# ACC Project Directory Structure

## Top-level Directory Overview

```
ACC/
├── CMakeLists.txt                # Top-level CMake build file
├── README.md                     # Project documentation
├── LICENSE                       # License
├── .gitignore                    # Git Ignore rules
├── .github/                      # GitHub CI/CD Configuration
├── cmake/                        # CMake Modules and toolchain files
├── docs/                         # Project documentation
├── include/                      # Public headers（对外接口）
│   └── ACC/
├── lib/                          # Core library implementations
│   ├── Dialect/                  # MLIR Dialect definitions and implementations
│   ├── Conversion/               # Inter-dialect Lowering/Conversion
│   ├── Transforms/               # MLIR General transform passes
│   ├── Frontend/                 # Language frontend（Lexer/Parser/AST/Sema）
│   ├── IROptimizer/              # LLVM IR optimization passes
│   ├── CodeGen/                  # Target code generation
│   └── Support/                  # Common utilities and helpers
├── runtime/                      # Runtime support library
│   ├── cpu/                      # CPU Runtime
│   ├── cuda/                     # [TODO] CUDA Runtime
│   └── ascend/                   # [TODO] Ascend Runtime
├── tools/                        # Executable tools
│   ├── acc/                # Main compiler driver
│   ├── ac-opt/                   # MLIR Optimization tool（类似 mlir-opt）
│   ├── ac-translate/             # IR Translation tool
│   └── ac-runner/                # JIT Execution tool
├── test/                         # Test cases
│   ├── lit.cfg.py                # lit 测试Configuration
│   ├── Dialect/                  # 方言测试
│   ├── Conversion/               # Lowering 测试
│   ├── Transforms/               # 变换 Pass 测试
│   ├── Frontend/                 # 前端测试
│   ├── IROptimizer/              # LLVM IR 优化测试
│   ├── CodeGen/                  # 代码生成测试
│   └── E2E/                      # 端到端测试
├── unittests/                    # C++ Unit tests（GoogleTest）
│   ├── Dialect/
│   ├── Frontend/
│   └── Support/
├── examples/                     # Example code
│   ├── models/                   # 示例 AI 模型
│   ├── dsl/                      # DSL 示例
│   └── tutorials/                # Tutorials
├── scripts/                      # Helper scripts
│   ├── build_llvm.sh             # LLVM 编译脚本
│   ├── build.sh                  # 项目编译脚本
│   └── run_tests.sh              # 测试运行脚本
└── third_party/                  # Third-party dependencies
    └── onnx/                     # ONNX protobuf 定义
```

---

## Detailed Directory Description

### 1. `cmake/` — CMake 构建模块

```
cmake/
├── modules/
│   ├── FindLLVM.cmake            # LLVM 查找模块
│   ├── FindMLIR.cmake            # MLIR 查找模块
│   ├── AddACC.cmake        # ACC 自定义 CMake 函数
│   └── TableGen.cmake            # TableGen 相关 CMake 规则
└── toolchains/
    ├── x86_64-linux.cmake        # x86_64 Linux 工具链
    └── aarch64-linux.cmake       # AArch64 Linux 工具链
```

**Description**: 包含 CMake 构建所需的模块文件和交叉编译工具链Configuration，封装了 LLVM/MLIR 的查找逻辑和 TableGen 代码生成规则。

---

### 2. `include/ACC/` — Public headers

```
include/ACC/
├── Dialect/
│   ├── ACHigh/                   # 高层 AI 方言 (20+ 算子)
│   │   ├── ACHighDialect.h       # 方言声明
│   │   ├── ACHighOps.h           # 算子声明
│   │   ├── ACHighTypes.h         # 类型声明（含量化类型）
│   │   ├── ACHighDialect.td      # TableGen 方言定义
│   │   ├── ACHighOps.td          # TableGen 算子定义（Conv2D/MatMul/BatchMatMul/
│   │   │                         #   ReLU/GELU/Sigmoid/Tanh/BatchNorm/Softmax/
│   │   │                         #   Add/Mul/MaxPool2D/AvgPool2D/Dense/Flatten/
│   │   │                         #   Reshape/Transpose/ConvReLU/Constant）
│   │   └── ACHighTypes.td        # TableGen 类型定义（含 QuantizedType）
│   ├── ACMid/                    # 中层优化方言
│   │   ├── ACMidDialect.h
│   │   ├── ACMidOps.h
│   │   ├── ACMidTypes.h
│   │   ├── ACMidDialect.td
│   │   ├── ACMidOps.td
│   │   └── ACMidTypes.td
│   └── ACGPU/                    # [TODO] GPU 方言
│       └── README.md
├── Conversion/
│   ├── ACHighToACMid/            # 高层 → 中层 Lowering
│   │   └── ACHighToACMid.h
│   ├── ACMidToLinalg/            # 中层 → Linalg Lowering
│   │   └── ACMidToLinalg.h
│   ├── ACMidToLLVM/              # 中层 → LLVM Lowering (完整链路)
│   │   └── ACMidToLLVM.h
│   └── Passes.h                  # 所有 Conversion Pass 的注册头文件
├── Transforms/
│   ├── Passes.h                  # 所有 Transform Pass 的注册头文件
│   ├── Passes.td                 # Pass 声明 TableGen 文件
│   ├── ConstantFolding.h         # 常量折叠
│   ├── OpFusion.h                # 算子融合
│   ├── ShapeInference.h          # 形状推导
│   ├── LayoutTransform.h         # 布局变换
│   └── DeadCodeElimination.h     # 死代码消除
├── Frontend/
│   ├── Lexer.h                   # 词法分析器
│   ├── Token.h                   # Token 定义
│   ├── Parser.h                  # 语法分析器
│   ├── AST.h                     # 抽象语法树
│   ├── Sema.h                    # 语义分析
│   ├── MLIRGen.h                 # AST → MLIR 生成
│   └── ONNXImporter.h            # ONNX 模型导入
├── IROptimizer/
│   ├── Passes.h                  # LLVM IR Pass 注册
│   ├── CustomLoopOpt.h           # 自定义循环优化
│   ├── CustomVectorize.h         # 自定义向量化
│   └── PassPipeline.h            # LLVM Pass Pipeline Configuration
├── CodeGen/
│   ├── CodeGenDriver.h           # 代码生成驱动
│   ├── CPUCodeGen.h              # CPU 后端代码生成
│   └── JITEngine.h               # JIT 编译引擎
├── Support/
│   ├── Logger.h                  # 日志系统
│   ├── Timer.h                   # 计时工具
│   ├── ErrorHandling.h           # 错误处理
│   └── CommandLine.h             # 命令行解析辅助
└── InitAllDialects.h             # 注册所有方言
```

**Description**: Public headers遵循 LLVM 的目录约定，按功能模块组织。`.td` 文件（TableGen 定义）和 `.h` 文件（C++ 声明）放在同一目录下，方便开发时对照查阅。

---

### 3. `lib/` — Core library implementations

```
lib/
├── Dialect/
│   ├── ACHigh/                   # 高层 AI 方言实现
│   │   ├── CMakeLists.txt
│   │   ├── ACHighDialect.cpp     # 方言注册和初始化
│   │   ├── ACHighOps.cpp         # 算子的验证、折叠、规范化
│   │   └── ACHighTypes.cpp       # 类型的解析和打印
│   └── ACMid/                    # 中层优化方言实现
│       ├── CMakeLists.txt
│       ├── ACMidDialect.cpp
│       ├── ACMidOps.cpp
│       └── ACMidTypes.cpp
├── Conversion/                   # 方言间转换 (Lowering)
│   ├── ACHighToACMid/            # 高层 → 中层
│   │   ├── CMakeLists.txt
│   │   └── ACHighToACMid.cpp
│   ├── ACMidToLinalg/            # 中层 → Linalg
│   │   ├── CMakeLists.txt
│   │   └── ACMidToLinalg.cpp
│   └── ACMidToLLVM/              # 中层 → LLVM（via Linalg/Affine/SCF）
│       ├── CMakeLists.txt
│       └── ACMidToLLVM.cpp
├── Transforms/                   # MLIR 变换 Pass
│   ├── CMakeLists.txt
│   ├── ConstantFolding.cpp       # 常量折叠 Pass
│   ├── OpFusion.cpp              # 算子融合 Pass
│   ├── ShapeInference.cpp        # 形状推导 Pass
│   ├── LayoutTransform.cpp       # 布局变换 Pass
│   ├── DeadCodeElimination.cpp   # 死代码消除 Pass
│   └── PassPipeline.cpp          # MLIR Pass Pipeline 组装
├── Frontend/                     # Language frontend
│   ├── CMakeLists.txt
│   ├── Lexer.cpp                 # 词法分析器实现
│   ├── Token.cpp                 # Token 工具函数
│   ├── Parser.cpp                # 语法分析器实现
│   ├── AST.cpp                   # AST 节点实现
│   ├── Sema.cpp                  # 语义分析实现
│   ├── MLIRGen.cpp               # AST → MLIR 代码生成
│   └── ONNXImporter.cpp          # ONNX 模型导入实现
├── IROptimizer/                  # LLVM IR 优化
│   ├── CMakeLists.txt
│   ├── CustomLoopOpt.cpp         # 自定义循环优化 Pass
│   ├── CustomVectorize.cpp       # 自定义向量化 Pass
│   └── PassPipeline.cpp          # LLVM IR 优化 Pipeline Configuration
├── CodeGen/                      # 代码生成
│   ├── CMakeLists.txt
│   ├── CodeGenDriver.cpp         # CodeGen 主驱动
│   ├── CPUCodeGen.cpp            # CPU 后端实现
│   └── JITEngine.cpp             # JIT 引擎实现
└── Support/                      # 公共支持库
    ├── CMakeLists.txt
    ├── Logger.cpp                # 日志系统实现
    ├── Timer.cpp                 # 计时工具实现
    └── ErrorHandling.cpp         # 错误处理实现
```

**Description**: `lib/` 目录包含所有核心功能的 C++ 实现。每个子目录对应一个独立的 CMake 库目标，便于模块化构建和依赖管理。

---

### 4. `runtime/` — Runtime support library

```
runtime/
├── CMakeLists.txt
├── include/
│   └── ac_runtime.h              # Runtime 公共 C API（句柄式设计）
│                                  #   ac_runtime_t / ac_tensor_t / ac_model_t
│                                  #   模型加载、推理执行、内存管理
├── cpu/
│   ├── CMakeLists.txt
│   ├── cpu_runtime.h             # CPU Runtime 接口
│   ├── cpu_runtime.cpp           # CPU Runtime 实现
│   ├── cpu_kernels.h             # CPU 算子 Kernel 声明
│   ├── cpu_kernels.cpp           # CPU 算子 Kernel 实现
│   │                              #   MatMul (naive + tiled)
│   │                              #   Conv2D (naive + im2col+GEMM)
│   │                              #   ReLU/GELU/Sigmoid/Tanh
│   │                              #   BatchNorm/Softmax/MaxPool/AvgPool
│   ├── memory_manager.h          # 内存池管理（64字节对齐）
│   └── memory_manager.cpp
├── cuda/                         # [TODO] CUDA Runtime
│   └── README.md
└── ascend/                       # [TODO] Ascend Runtime
    └── README.md
```

**Description**: Runtime 库提供编译后代码在运行时所需的支持。API 采用句柄模式（`ac_runtime_t` / `ac_tensor_t` / `ac_model_t`），支持模型加载和批量推理执行。CPU Kernel 包含 naive 参考实现和 im2col+GEMM 优化实现。

---

### 5. `tools/` — Executable tools

```
tools/
├── acc/                    # Main compiler driver（类似 clang）
│   ├── CMakeLists.txt
│   └── main.cpp                  # 编译器入口，串联整个编译流水线
├── ac-opt/                       # MLIR Optimization tool（类似 mlir-opt）
│   ├── CMakeLists.txt
│   └── main.cpp                  # 注册所有方言和 Pass，提供交互式优化
├── ac-translate/                 # IR Translation tool
│   ├── CMakeLists.txt
│   └── main.cpp                  # MLIR ↔ LLVM IR ↔ 其他格式的翻译
└── ac-runner/                    # JIT Execution tool
    ├── CMakeLists.txt
    └── main.cpp                  # 基于 JIT 的 MLIR/LLVM IR 即时执行
```

**Description**: 提供用户可直接使用的命令行工具。`ac-opt` 是日常开发中最常用的工具，用于测试和调试 MLIR Pass。

---

### 6. `test/` — 测试

```
test/
├── CMakeLists.txt
├── lit.cfg.py                    # lit 测试框架全局Configuration
├── lit.site.cfg.py.in            # 构建目录的 lit 站点Configuration模板
├── Dialect/
│   ├── ACHigh/
│   │   ├── ops.mlir              # 算子定义正确性测试
│   │   ├── types.mlir            # 类型系统测试
│   │   └── invalid.mlir          # 非法 IR 的错误检测测试
│   └── ACMid/
│       ├── ops.mlir
│       └── types.mlir
├── Conversion/
│   ├── ACHighToACMid/
│   │   └── basic.mlir            # 高层到中层 Lowering 测试
│   ├── ACMidToLinalg/
│   │   └── basic.mlir
│   └── ACMidToLLVM/
│       └── basic.mlir
├── Transforms/
│   ├── constant-folding.mlir     # 常量折叠测试
│   ├── op-fusion.mlir            # 算子融合测试
│   ├── shape-inference.mlir      # 形状推导测试
│   └── layout-transform.mlir    # 布局变换测试
├── Frontend/
│   ├── lexer/                    # 词法分析Test cases
│   ├── parser/                   # 语法分析Test cases
│   └── sema/                     # 语义分析Test cases
├── IROptimizer/
│   ├── loop-opt.ll               # LLVM IR 循环优化测试
│   └── vectorize.ll              # LLVM IR 向量化测试
├── CodeGen/
│   ├── cpu-codegen.mlir          # CPU 代码生成测试
│   └── jit-execution.mlir        # JIT 执行测试
├── Performance/                  # 性能基准测试
│   ├── CMakeLists.txt
│   ├── benchmark.h               # 性能测试框架（统计分析、GFLOPS计算）
│   ├── bench_matmul.cpp          # 矩阵乘法 benchmark
│   └── bench_conv2d.cpp          # 卷积 benchmark
└── E2E/
    ├── simple-matmul.mlir        # 端到端矩阵乘法测试
    ├── conv2d-relu.mlir          # 端到端 Conv+ReLU 测试
    └── simple-model.onnx         # ONNX 模型端到端测试
```

**Description**: 三层测试体系:
- **FileCheck 测试** (`lit` + `FileCheck`): MLIR/LLVM IR 变换正确性
- **Unit tests** (`unittests/`, GoogleTest): C++ 逻辑正确性
- **性能基准** (`Performance/`): 算子和模型推理性能

---

### 7. `examples/` — 示例与教程

```
examples/
├── models/
│   ├── simple_matmul.onnx        # 简单矩阵乘法模型
│   ├── simple_conv.onnx          # 简单卷积模型
│   └── resnet18.onnx             # ResNet-18 模型
├── dsl/
│   ├── hello.ac                  # DSL Hello World 示例
│   ├── matmul.ac                 # 矩阵乘法 DSL
│   └── simple_net.ac             # 简单网络 DSL
└── tutorials/
    ├── 01-build-llvm/            # 教程1: 编译 LLVM/MLIR
    │   └── README.md
    ├── 02-first-dialect/         # 教程2: 创建第一个方言
    │   └── README.md
    ├── 03-define-ops/            # 教程3: 定义算子
    │   └── README.md
    ├── 04-write-pass/            # 教程4: 编写优化 Pass
    │   └── README.md
    ├── 05-lowering/              # 教程5: 实现 Lowering
    │   └── README.md
    ├── 06-llvm-ir-opt/           # 教程6: LLVM IR 优化
    │   └── README.md
    ├── 07-codegen/               # 教程7: 代码生成
    │   └── README.md
    └── 08-e2e-pipeline/          # 教程8: 端到端编译流水线
        └── README.md
```

**Description**: 提供丰富的示例和由浅入深的教程，帮助开发者快速上手。

---

### 8. `scripts/` — Helper scripts

```
scripts/
├── build_llvm.sh                 # LLVM/MLIR 一键编译脚本
├── build.sh                      # ACC 一键编译脚本
├── run_tests.sh                  # 运行全部测试
├── format.sh                     # 代码格式化（clang-format）
└── gen_docs.sh                   # 生成 API 文档
```

---

### 9. `.github/` — CI/CD Configuration

```
.github/
├── workflows/
│   ├── build.yml                 # 自动构建 workflow
│   ├── test.yml                  # 自动测试 workflow
│   └── lint.yml                  # 代码检查 workflow
└── ISSUE_TEMPLATE/
    ├── bug_report.md
    └── feature_request.md
```

---

## Core Module Dependencies

```
Frontend ──→ Dialect/ACHigh ──→ Transforms ──→ Conversion/HighToMid
                                                      │
                                               Dialect/ACMid
                                                      │
                                            Conversion/MidToLinalg
                                                      │
                                            Conversion/MidToLLVM
                                                      │
                                               IROptimizer
                                                      │
                                                  CodeGen
                                                      │
                                                  Runtime
```

## Build Artifacts

Build artifacts are located in `build/` 目录（不纳入版本管理）：

```
build/
├── bin/
│   ├── acc                 # 主编译器
│   ├── ac-opt                    # MLIR Optimization tool
│   ├── ac-translate              # Translation tool
│   └── ac-runner                 # JIT 执行器
├── lib/
│   ├── libACDialectACHigh.a      # 各模块静态库
│   ├── libACDialectACMid.a
│   ├── libACTransforms.a
│   ├── libACConversion.a
│   ├── libACFrontend.a
│   ├── libACIROptimizer.a
│   ├── libACCodeGen.a
│   ├── libACSupport.a
│   └── libACRuntime.so           # Runtime 动态库
└── test/                         # 测试产物
```
