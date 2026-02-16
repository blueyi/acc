# ACompiler - AI编译器学习项目

<div align="center">

**基于LLVM/MLIR的AI编译器实践项目**

通过实例快速学习和掌握编译器技术

[项目规划](PROJECT_PLAN.md) • [快速开始](#快速开始) • [文档](docs/) • [示例](examples/)

</div>

---

## 📋 项目简介

ACompiler是一个教育性质的AI编译器项目，基于LLVM和MLIR框架构建。本项目采用**自上而下、由浅入深**的学习路线，帮助开发者通过实践掌握现代编译器技术。

### 核心特点

- 🎯 **实践导向**: 通过完整的AI编译器实现学习编译原理
- 📚 **循序渐进**: 从MLIR高层抽象到LLVM IR底层优化
- 🔧 **模块化设计**: 清晰的模块划分，便于理解和扩展
- 🚀 **多后端支持**: 设计支持CPU、GPU、NPU等多种后端
- 📖 **完整文档**: 详细的学习教程和API文档

## 🏗️ 项目架构

```
AI模型 (ONNX/...)
      ↓
前端导入器 (Frontend)
      ↓
MLIR表示 (AI Dialect)
      ↓
MLIR优化 (Fusion, Folding, etc.)
      ↓
渐进降级 (Linalg → Affine → SCF)
      ↓
LLVM IR生成
      ↓
LLVM优化 (Loop, Vectorization, etc.)
      ↓
代码生成 (CodeGen)
      ↓
目标代码 (CPU/GPU/NPU)
```

## 📂 目录结构

```
ACompiler/
├── docs/                      # 文档目录
│   ├── tutorials/            # 学习教程
│   ├── design/               # 设计文档
│   └── api/                  # API文档
├── include/                   # 头文件
│   └── acompiler/
│       ├── Dialect/          # MLIR Dialect定义
│       ├── Conversion/       # 转换Pass
│       ├── Transforms/       # 变换Pass
│       ├── Target/           # 后端目标
│       └── Runtime/          # Runtime接口
├── lib/                       # 实现文件
│   ├── Dialect/              # Dialect实现
│   ├── Conversion/           # 转换Pass实现
│   ├── Transforms/           # 变换Pass实现
│   ├── Target/               # 后端实现
│   └── Runtime/              # Runtime实现
├── tools/                     # 工具
│   ├── acompiler-opt/        # MLIR优化工具
│   ├── acompiler-translate/  # 翻译工具
│   └── acompiler/            # 主编译器驱动
├── runtime/                   # Runtime库
│   ├── cpu/                  # CPU Runtime
│   ├── cuda/                 # CUDA Runtime (TODO)
│   └── ascend/               # Ascend Runtime (TODO)
├── examples/                  # 示例代码
│   ├── mlir/                 # MLIR示例
│   ├── models/               # 模型示例
│   └── tutorials/            # 教程代码
├── test/                      # 测试
│   ├── Dialect/              # Dialect测试
│   ├── Conversion/           # 转换测试
│   ├── Transforms/           # 变换测试
│   └── Integration/          # 集成测试
├── third_party/               # 第三方依赖
├── cmake/                     # CMake模块
├── scripts/                   # 辅助脚本
├── CMakeLists.txt            # 主CMake配置
├── PROJECT_PLAN.md           # 详细项目规划
└── README.md                 # 本文件
```

## 🚀 快速开始

### 环境要求

- **操作系统**: Linux / macOS / Windows (WSL)
- **编译器**: GCC 9+ / Clang 12+
- **CMake**: 3.20+
- **LLVM/MLIR**: 17.0+
- **Python**: 3.8+ (用于脚本工具)

### 构建步骤

```bash
# 1. 克隆仓库
git clone <repository-url>
cd ACompiler

# 2. 安装LLVM/MLIR（如果系统未安装）
./scripts/install_llvm.sh

# 3. 创建构建目录
mkdir build && cd build

# 4. 配置CMake
cmake -G Ninja .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_DIR=/path/to/llvm/install/lib/cmake/llvm \
  -DMLIR_DIR=/path/to/llvm/install/lib/cmake/mlir

# 5. 编译
ninja

# 6. 运行测试
ninja check-acompiler
```

### 快速测试

```bash
# 编译一个简单的MLIR文件
./build/bin/acompiler-opt examples/mlir/simple.mlir

# 端到端编译示例模型
./build/bin/acompiler examples/models/simple_model.onnx -o output.o
```

## 📖 学习路线

### 第一步：MLIR基础（1-2周）

学习MLIR的核心概念和自定义Dialect开发：

1. 理解MLIR基本概念（Operation, Block, Region）
2. 学习Dialect定义和TableGen
3. 实现简单的AI算子Dialect
4. 编写基础的优化Pass

**推荐阅读**:
- [docs/tutorials/01_mlir_basics.md](docs/tutorials/01_mlir_basics.md)
- [docs/tutorials/02_custom_dialect.md](docs/tutorials/02_custom_dialect.md)

### 第二步：MLIR Pass开发（1-2周）

深入学习Pass开发和降级转换：

1. Pattern Rewriting框架
2. Dialect Conversion框架
3. 实现算子融合Pass
4. 实现多层降级流程

**推荐阅读**:
- [docs/tutorials/03_pass_development.md](docs/tutorials/03_pass_development.md)
- [docs/tutorials/04_dialect_lowering.md](docs/tutorials/04_dialect_lowering.md)

### 第三步：LLVM IR与优化（2-3周）

学习LLVM IR和优化技术：

1. 理解LLVM IR结构
2. 学习LLVM Pass开发
3. 实现从MLIR到LLVM IR的降级
4. 集成LLVM优化Pass

**推荐阅读**:
- [docs/tutorials/05_llvm_ir_basics.md](docs/tutorials/05_llvm_ir_basics.md)
- [docs/tutorials/06_llvm_optimization.md](docs/tutorials/06_llvm_optimization.md)

### 第四步：代码生成（2-3周）

学习后端代码生成：

1. 理解CodeGen流程
2. 学习指令选择和寄存器分配
3. 实现CPU后端
4. Runtime库开发

**推荐阅读**:
- [docs/tutorials/07_codegen_basics.md](docs/tutorials/07_codegen_basics.md)
- [docs/tutorials/08_cpu_backend.md](docs/tutorials/08_cpu_backend.md)

## 🎯 当前进度

### ✅ 已完成

- [x] 项目规划和目录结构设计
- [x] 基础文档框架

### 🚧 进行中（第一阶段：CPU后端）

- [ ] CMake构建系统配置
- [ ] AI Dialect定义
- [ ] 基础优化Pass实现
- [ ] 到LLVM IR的降级
- [ ] CPU代码生成
- [ ] Runtime库实现

### 📋 计划中

#### TODO: NVIDIA GPU CUDA支持
- [ ] CUDA后端框架
- [ ] PTX代码生成
- [ ] CUDA Runtime集成
- [ ] GPU特定优化Pass
- [ ] Kernel启动代码生成

#### TODO: 华为Ascend NPU支持
- [ ] Ascend后端框架
- [ ] CANN框架集成
- [ ] Ascend IR生成
- [ ] TBE算子支持
- [ ] NPU特定优化Pass

## 🔬 示例

### MLIR Dialect示例

```mlir
// AI Dialect中的简单卷积操作
func.func @conv2d(%input: tensor<1x3x224x224xf32>, 
                   %weight: tensor<64x3x3x3xf32>) 
                   -> tensor<1x64x224x224xf32> {
  %result = ai.conv2d %input, %weight 
    {stride = [1, 1], padding = [1, 1]} 
    : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) 
    -> tensor<1x64x224x224xf32>
  return %result : tensor<1x64x224x224xf32>
}
```

### Pass优化示例

```cpp
// Conv+ReLU融合Pass
struct ConvReLuFusion : public PassWrapper<ConvReLuFusion, 
                                            OperationPass<func::FuncOp>> {
  void runOnOperation() override {
    // 查找Conv后跟ReLU的模式并融合
    getOperation().walk([&](ai::Conv2DOp conv) {
      if (auto relu = dyn_cast_or_null<ai::ReLUOp>(
            *conv->getUsers().begin())) {
        // 执行融合逻辑
        fusedOp = rewriter.create<ai::ConvReLUOp>(/*...*/);
      }
    });
  }
};
```

## 🛠️ 开发工具

### acompiler-opt

MLIR优化和转换工具，类似于`mlir-opt`：

```bash
# 运行特定Pass
acompiler-opt --ai-conv-relu-fusion input.mlir

# 降级到LLVM Dialect
acompiler-opt --convert-ai-to-llvm input.mlir
```

### acompiler-translate

MLIR翻译工具，用于导入/导出：

```bash
# 导入ONNX模型
acompiler-translate --import-onnx model.onnx -o model.mlir

# 导出LLVM IR
acompiler-translate --mlir-to-llvmir model.mlir -o model.ll
```

### acompiler

主编译器驱动程序：

```bash
# 编译ONNX模型到目标代码
acompiler model.onnx -o model.o --target=cpu

# 生成共享库
acompiler model.onnx -o libmodel.so --shared

# 启用优化
acompiler model.onnx -o model.o -O3
```

## 📚 文档

- [项目详细规划](PROJECT_PLAN.md) - 完整的项目规划和学习路线
- [架构设计](docs/design/architecture.md) - 系统架构设计
- [API参考](docs/api/) - 详细的API文档
- [开发指南](docs/development.md) - 贡献代码指南
- [常见问题](docs/FAQ.md) - 常见问题解答

## 🤝 贡献

欢迎贡献代码、文档或建议！请查看[开发指南](docs/development.md)了解详情。

## 📄 许可证

本项目采用 Apache 2.0 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

本项目基于以下优秀的开源项目：

- [LLVM](https://llvm.org/) - 编译器基础设施
- [MLIR](https://mlir.llvm.org/) - 多层IR框架
- [ONNX](https://onnx.ai/) - 模型交换格式

## 📞 联系方式

- 项目问题: [GitHub Issues](../../issues)
- 讨论交流: [GitHub Discussions](../../discussions)

---

<div align="center">
Made with ❤️ for learning compiler technology
</div>
