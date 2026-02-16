# ACompiler 目录结构说明

本文档详细说明ACompiler项目的目录结构和各目录的用途。

## 📁 顶层目录结构

```
ACompiler/
├── include/          # 头文件目录（公开接口）
├── lib/              # 实现文件目录
├── tools/            # 可执行工具目录
├── runtime/          # Runtime运行时库
├── examples/         # 示例代码
├── test/             # 测试代码
├── docs/             # 文档
├── third_party/      # 第三方依赖
├── cmake/            # CMake模块
├── scripts/          # 辅助脚本
└── build/            # 构建输出目录（生成，不提交）
```

---

## 📂 include/ - 头文件目录

包含所有对外暴露的C++头文件，定义公共API接口。

### include/acompiler/Dialect/ - Dialect定义

MLIR自定义Dialect的头文件。

```
Dialect/
├── AI/                    # AI Dialect（核心AI算子）
│   ├── AIDialect.h       # Dialect定义
│   ├── AIOps.h           # Operation定义
│   ├── AIOps.td          # TableGen定义文件
│   ├── AITypes.h         # Type定义
│   ├── AITypes.td        # Type的TableGen定义
│   ├── AIAttrs.h         # Attribute定义
│   └── AIAttrs.td        # Attribute的TableGen定义
└── Tensor/                # 张量Dialect（扩展功能）
    ├── TensorDialect.h
    ├── TensorOps.h
    └── TensorOps.td
```

**主要内容**:
- **AIDialect**: 定义AI领域专用算子（Conv2D, MatMul, ReLU等）
- **TensorDialect**: 扩展的张量操作
- **TableGen文件（.td）**: 使用MLIR的TableGen框架声明式定义操作

**关键文件说明**:
- `AIDialect.h`: Dialect类声明，注册Dialect到MLIR
- `AIOps.td`: 使用ODS（Operation Definition Specification）定义操作
- `AITypes.h/td`: 自定义类型系统（如动态/静态形状张量）
- `AIAttrs.h/td`: 自定义属性（如卷积参数、量化参数等）

### include/acompiler/Conversion/ - 转换Pass

Dialect之间的转换Pass头文件。

```
Conversion/
├── AIToLinalg/            # AI Dialect -> Linalg Dialect
│   ├── AIToLinalg.h      # 转换Pass声明
│   └── Passes.h          # Pass注册
├── LinalgToLLVM/          # Linalg -> LLVM Dialect
│   ├── LinalgToLLVM.h
│   └── Passes.h
├── Passes.h               # 所有转换Pass的统一入口
└── Passes.td              # Pass的TableGen定义
```

**转换流程**:
```
AI Dialect → Linalg → Affine → SCF → Std → LLVM Dialect
```

**关键概念**:
- **Linalg**: 线性代数抽象，支持循环融合和tile优化
- **Affine**: 仿射表达式，支持多面体优化
- **SCF**: 结构化控制流（for/while等）
- **LLVM Dialect**: MLIR中的LLVM IR表示

### include/acompiler/Transforms/ - 变换Pass

优化和变换Pass头文件。

```
Transforms/
├── FusionPasses/          # 融合优化
│   ├── ConvReLUFusion.h  # 卷积+ReLU融合
│   ├── MatMulAddFusion.h # 矩阵乘+加法融合
│   └── GenericFusion.h   # 通用融合框架
├── OptimizationPasses/    # 通用优化
│   ├── ConstantFolding.h # 常量折叠
│   ├── DeadCodeElim.h    # 死代码消除
│   ├── MemoryOpt.h       # 内存优化
│   └── ShapeInference.h  # 形状推导
├── Passes.h               # Pass统一入口
└── Passes.td              # TableGen定义
```

**Pass类型**:
1. **融合Pass**: 算子融合减少内存访问
2. **优化Pass**: 代码优化和简化
3. **分析Pass**: 信息收集和分析

### include/acompiler/Target/ - 目标后端

代码生成后端的头文件。

```
Target/
├── CPU/                   # CPU后端
│   ├── CPUTarget.h       # CPU Target定义
│   ├── X86CodeGen.h      # x86_64代码生成
│   ├── ARMCodeGen.h      # ARM代码生成
│   └── Passes.h          # CPU相关Pass
├── CUDA/                  # NVIDIA GPU后端（TODO）
│   ├── CUDATarget.h      # CUDA Target定义
│   ├── PTXCodeGen.h      # PTX代码生成
│   ├── CUDARuntime.h     # CUDA Runtime接口
│   └── Passes.h          # CUDA优化Pass
└── Ascend/                # 华为Ascend NPU后端（TODO）
    ├── AscendTarget.h    # Ascend Target定义
    ├── AscendIRGen.h     # Ascend IR生成
    ├── TBEIntegration.h  # TBE算子集成
    └── Passes.h          # Ascend优化Pass
```

**后端职责**:
- **代码生成**: 将LLVM IR转换为目标机器码
- **指令选择**: 选择最优指令序列
- **寄存器分配**: 管理寄存器使用
- **指令调度**: 优化指令执行顺序

### include/acompiler/Runtime/ - Runtime接口

Runtime库的公共接口。

```
Runtime/
├── API.h                  # Runtime C API
├── MemoryManager.h        # 内存管理
├── ExecutionEngine.h      # 执行引擎
└── Profiling.h            # 性能分析
```

---

## 📂 lib/ - 实现目录

与`include/`结构对应，包含所有的实现文件。

### lib/Dialect/ - Dialect实现

```
Dialect/
├── AI/
│   ├── AIDialect.cpp     # Dialect实现
│   ├── AIOps.cpp         # Operation实现（verify等）
│   ├── AITypes.cpp       # Type实现
│   └── CMakeLists.txt    # 构建配置
└── Tensor/
    ├── TensorDialect.cpp
    ├── TensorOps.cpp
    └── CMakeLists.txt
```

**实现内容**:
- Operation的验证逻辑（verify()）
- Operation的fold()方法（常量折叠）
- Type的解析和打印
- Attribute的解析和验证

### lib/Conversion/ - 转换实现

```
Conversion/
├── AIToLinalg/
│   ├── Conv2DLowering.cpp    # Conv2D降级实现
│   ├── MatMulLowering.cpp    # MatMul降级实现
│   ├── ActivationLowering.cpp # 激活函数降级
│   └── CMakeLists.txt
└── LinalgToLLVM/
    ├── LinalgToLLVMPass.cpp
    └── CMakeLists.txt
```

**实现模式**:
- 使用`ConversionPattern`进行模式匹配和重写
- 定义类型转换器（TypeConverter）
- 实现合法性检查

### lib/Transforms/ - 变换实现

```
Transforms/
├── FusionPasses/
│   ├── ConvReLUFusion.cpp    # 卷积ReLU融合实现
│   ├── FusionPatterns.cpp    # 融合模式库
│   └── CMakeLists.txt
└── OptimizationPasses/
    ├── ConstantFolding.cpp
    ├── DeadCodeElim.cpp
    └── CMakeLists.txt
```

### lib/Target/ - 后端实现

```
Target/
├── CPU/
│   ├── CPUTarget.cpp         # CPU后端注册
│   ├── X86CodeGen.cpp        # x86代码生成
│   ├── VectorizePass.cpp     # 向量化Pass
│   └── CMakeLists.txt
├── CUDA/                      # TODO
│   └── README.md             # CUDA后端规划
└── Ascend/                    # TODO
    └── README.md             # Ascend后端规划
```

### lib/Runtime/ - Runtime实现

```
Runtime/
├── API.cpp                    # Runtime API实现
├── MemoryManager.cpp          # 内存管理实现
├── ExecutionEngine.cpp        # 执行引擎
└── CMakeLists.txt
```

---

## 📂 tools/ - 工具目录

可执行工具程序。

### tools/acompiler-opt/ - MLIR优化工具

```
acompiler-opt/
├── acompiler-opt.cpp          # 主程序
└── CMakeLists.txt
```

**功能**: 类似于`mlir-opt`，用于测试和调试Pass
```bash
acompiler-opt --ai-conv-relu-fusion input.mlir
acompiler-opt --convert-ai-to-linalg input.mlir
```

### tools/acompiler-translate/ - 翻译工具

```
acompiler-translate/
├── acompiler-translate.cpp    # 主程序
├── ONNXImporter.cpp           # ONNX导入器
├── LLVMIRExporter.cpp         # LLVM IR导出器
└── CMakeLists.txt
```

**功能**: 导入/导出不同格式
```bash
acompiler-translate --import-onnx model.onnx -o model.mlir
acompiler-translate --mlir-to-llvmir model.mlir -o model.ll
```

### tools/acompiler/ - 主编译器

```
acompiler/
├── acompiler.cpp              # 主驱动程序
├── CompilerDriver.cpp         # 编译流程控制
├── Options.cpp                # 命令行选项
└── CMakeLists.txt
```

**功能**: 端到端编译
```bash
acompiler model.onnx -o model.o --target=cpu -O3
```

---

## 📂 runtime/ - Runtime库

运行时库，提供模型执行所需的基础设施。

### runtime/cpu/ - CPU Runtime

```
cpu/
├── include/
│   ├── cpu_runtime.h          # CPU Runtime API
│   └── ops/                   # 算子实现
│       ├── conv2d.h
│       ├── matmul.h
│       └── activation.h
├── src/
│   ├── cpu_runtime.cpp
│   ├── memory.cpp             # 内存管理
│   ├── threading.cpp          # 线程池
│   └── ops/                   # 算子实现
│       ├── conv2d.cpp         # 使用OpenMP/SIMD优化
│       ├── matmul.cpp         # BLAS/MKL集成
│       └── activation.cpp
├── CMakeLists.txt
└── README.md
```

**特性**:
- 多线程支持（OpenMP/TBB）
- SIMD优化（SSE/AVX）
- BLAS库集成（可选）

### runtime/cuda/ - CUDA Runtime（TODO）

```
cuda/
├── include/
│   ├── cuda_runtime.h
│   └── kernels/               # CUDA Kernel
│       ├── conv2d.cuh
│       └── matmul.cuh
├── src/
│   ├── cuda_runtime.cu
│   └── kernels/
│       ├── conv2d.cu          # Kernel实现
│       └── matmul.cu          # 使用Tensor Core
├── CMakeLists.txt
└── README.md                  # 规划文档
```

**规划特性**:
- CUDA Kernel库
- cuDNN集成
- Tensor Core支持
- 内存池管理

### runtime/ascend/ - Ascend Runtime（TODO）

```
ascend/
├── include/
│   ├── ascend_runtime.h
│   └── ops/
│       └── tbe_ops.h          # TBE算子接口
├── src/
│   ├── ascend_runtime.cpp
│   ├── cann_integration.cpp   # CANN框架集成
│   └── ops/
│       └── tbe_ops.cpp
├── CMakeLists.txt
└── README.md                  # 规划文档
```

**规划特性**:
- CANN框架集成
- TBE自定义算子
- ACL运行时接口
- 内存管理（DDR/HBM）

---

## 📂 examples/ - 示例代码

学习和测试用的示例。

### examples/mlir/ - MLIR示例

```
mlir/
├── simple_ops.mlir            # 简单算子示例
├── conv2d.mlir                # 卷积示例
├── fusion.mlir                # 算子融合示例
├── lowering.mlir              # 降级示例
└── README.md                  # 示例说明
```

### examples/models/ - 模型示例

```
models/
├── simple_model.onnx          # 简单测试模型
├── resnet18.onnx              # ResNet-18
├── mobilenet_v2.onnx          # MobileNet V2
└── README.md                  # 模型说明
```

### examples/tutorials/ - 教程代码

```
tutorials/
├── 01_hello_mlir/             # MLIR入门
│   ├── hello.mlir
│   └── README.md
├── 02_custom_op/              # 自定义算子
│   ├── custom_op.td
│   ├── custom_op.cpp
│   └── README.md
└── 03_optimization_pass/      # 优化Pass
    ├── simple_pass.cpp
    └── README.md
```

---

## 📂 test/ - 测试目录

单元测试和集成测试。

### test/Dialect/ - Dialect测试

```
Dialect/
├── AI/
│   ├── ops.mlir               # Operation测试（FileCheck）
│   ├── invalid.mlir           # 错误检测测试
│   └── CMakeLists.txt
└── Tensor/
    └── ops.mlir
```

**测试方法**: 使用LLVM FileCheck进行IR验证
```mlir
// RUN: acompiler-opt %s | FileCheck %s
// CHECK-LABEL: func @test_conv2d
func.func @test_conv2d(%arg0: tensor<1x3x224x224xf32>) -> tensor<1x64x224x224xf32>
```

### test/Conversion/ - 转换测试

```
Conversion/
├── AIToLinalg/
│   ├── conv2d_lowering.mlir   # Conv2D降级测试
│   ├── matmul_lowering.mlir
│   └── CMakeLists.txt
└── LinalgToLLVM/
    └── lowering.mlir
```

### test/Transforms/ - 变换测试

```
Transforms/
├── fusion.mlir                # 融合Pass测试
├── constant_folding.mlir      # 常量折叠测试
└── CMakeLists.txt
```

### test/Integration/ - 集成测试

```
Integration/
├── end_to_end/                # 端到端测试
│   ├── simple_model.mlir
│   └── test_runner.py
├── correctness/               # 正确性测试
│   └── numerical_test.py
└── performance/               # 性能测试
    └── benchmark.py
```

---

## 📂 docs/ - 文档目录

项目文档。

### docs/tutorials/ - 教程文档

```
tutorials/
├── 01_mlir_basics.md          # MLIR基础
├── 02_custom_dialect.md       # 自定义Dialect
├── 03_pass_development.md     # Pass开发
├── 04_dialect_lowering.md     # Dialect降级
├── 05_llvm_ir_basics.md       # LLVM IR基础
├── 06_llvm_optimization.md    # LLVM优化
├── 07_codegen_basics.md       # CodeGen基础
└── 08_cpu_backend.md          # CPU后端
```

### docs/design/ - 设计文档

```
design/
├── architecture.md            # 架构设计
├── dialect_design.md          # Dialect设计
├── pass_pipeline.md           # Pass流水线
├── runtime_design.md          # Runtime设计
└── multi_backend.md           # 多后端设计
```

### docs/api/ - API文档

```
api/
├── dialect_api.md             # Dialect API
├── conversion_api.md          # Conversion API
├── transforms_api.md          # Transforms API
└── runtime_api.md             # Runtime API
```

---

## 📂 其他目录

### third_party/ - 第三方依赖

```
third_party/
├── llvm/                      # LLVM/MLIR（如果本地构建）
├── onnx/                      # ONNX
└── README.md                  # 依赖说明
```

### cmake/ - CMake模块

```
cmake/
├── AddACompiler.cmake         # ACompiler构建辅助
├── FindMLIR.cmake             # 查找MLIR
├── AddMLIRDialect.cmake       # Dialect构建辅助
└── TableGen.cmake             # TableGen辅助
```

### scripts/ - 辅助脚本

```
scripts/
├── install_llvm.sh            # 安装LLVM/MLIR
├── build.sh                   # 构建脚本
├── test.sh                    # 测试脚本
├── format.sh                  # 代码格式化
└── generate_docs.sh           # 生成文档
```

---

## 🔄 编译流程示例

完整的编译流程涉及多个目录的协作：

```
1. 前端导入 (tools/acompiler-translate)
   ONNX → MLIR (AI Dialect)

2. MLIR优化 (lib/Transforms)
   - 算子融合
   - 常量折叠
   - 死代码消除

3. 渐进降级 (lib/Conversion)
   AI Dialect → Linalg → Affine → SCF → Std → LLVM Dialect

4. LLVM IR优化
   - 循环优化
   - 向量化
   - 内联

5. 代码生成 (lib/Target)
   LLVM IR → 机器码

6. 运行时链接 (runtime/)
   链接Runtime库 → 可执行文件/共享库
```

---

## 📊 目录依赖关系

```
tools/acompiler
  ├─> lib/Dialect (使用自定义Dialect)
  ├─> lib/Conversion (使用转换Pass)
  ├─> lib/Transforms (使用优化Pass)
  ├─> lib/Target (使用后端)
  └─> lib/Runtime (链接Runtime)

lib/Conversion
  └─> lib/Dialect (依赖Dialect定义)

lib/Transforms
  └─> lib/Dialect (依赖Dialect定义)

lib/Target
  ├─> lib/Runtime (使用Runtime接口)
  └─> LLVM库 (代码生成)

runtime/
  └─> 独立编译，可单独使用
```

---

## 🎯 开发工作流

### 添加新算子

1. 在`include/acompiler/Dialect/AI/AIOps.td`定义算子
2. 在`lib/Dialect/AI/AIOps.cpp`实现验证逻辑
3. 在`lib/Conversion/AIToLinalg/`添加降级实现
4. 在`runtime/cpu/src/ops/`添加Runtime实现
5. 在`test/Dialect/AI/`添加测试
6. 在`examples/mlir/`添加示例

### 添加新Pass

1. 在`include/acompiler/Transforms/`添加头文件
2. 在`lib/Transforms/`实现Pass
3. 在`lib/Transforms/Passes.td`注册Pass
4. 在`test/Transforms/`添加测试
5. 更新`tools/acompiler-opt/`以支持新Pass

### 添加新后端

1. 在`include/acompiler/Target/`创建后端目录
2. 实现Target类和CodeGen
3. 在`runtime/`添加对应Runtime
4. 在`lib/Target/`实现后端逻辑
5. 在`tools/acompiler/`添加后端选项
6. 添加测试和文档

---

## 📝 文件命名规范

- **头文件**: `UpperCamelCase.h`
- **源文件**: `UpperCamelCase.cpp`
- **TableGen**: `UpperCamelCase.td`
- **MLIR文件**: `snake_case.mlir`
- **测试文件**: `test_feature.mlir` 或 `feature_test.cpp`
- **文档**: `snake_case.md`

---

*最后更新: 2026-02-16*
