# 开发指南

本文档为ACompiler项目的贡献者提供开发指南。

## 开发环境设置

### 必需工具

- **C++编译器**: GCC 9+ 或 Clang 12+
- **CMake**: 3.20+
- **LLVM/MLIR**: 17.0+
- **Python**: 3.8+
- **Git**: 2.0+

### 推荐工具

- **Ninja**: 更快的构建系统
- **ccache**: 编译缓存
- **clangd**: C++ LSP服务器
- **clang-format**: 代码格式化

### IDE配置

#### VS Code

推荐插件：
- C/C++ (Microsoft)
- clangd
- CMake Tools
- MLIR

#### CLion

配置CMake项目，启用clangd。

## 代码风格

遵循LLVM编码规范：
- [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)

### 格式化

使用clang-format：

```bash
clang-format -i file.cpp
```

或批量格式化：

```bash
./scripts/format.sh
```

## 开发流程

### 1. 创建分支

```bash
git checkout -b feature/your-feature-name
```

### 2. 开发

遵循以下原则：
- 小步提交
- 清晰的提交信息
- 每个提交保持可编译状态

### 3. 测试

```bash
# 构建
ninja

# 运行测试
ninja check-acompiler

# 运行单个测试
./build/bin/acompiler-opt test/Dialect/AI/ops.mlir
```

### 4. 提交

```bash
git add .
git commit -m "Add feature X"
```

提交信息格式：

```
[Component] Brief description

Detailed explanation of what changed and why.

Fixes #123
```

### 5. 推送和PR

```bash
git push origin feature/your-feature-name
```

## 添加新功能

### 添加新Operation

1. 在`.td`文件中定义
2. 实现C++代码
3. 添加测试
4. 更新文档

示例：添加Sigmoid操作

```tablegen
// include/acompiler/Dialect/AI/AIOps.td
def AI_SigmoidOp : AI_Op<"sigmoid", [Pure, SameOperandsAndResultType]> {
  let summary = "Sigmoid activation";
  let arguments = (ins AnyTensor:$input);
  let results = (outs AnyTensor:$output);
}
```

### 添加新Pass

1. 在`Passes.td`中定义
2. 实现Pass逻辑
3. 注册Pass
4. 添加测试

### 添加新后端

1. 创建Target目录
2. 实现Target接口
3. 实现CodeGen
4. 添加Runtime支持

## 测试

### 单元测试

使用GoogleTest框架（未来添加）。

### 集成测试

使用LLVM FileCheck：

```mlir
// RUN: acompiler-opt %s | FileCheck %s

// CHECK-LABEL: func @test
func.func @test(%arg0: tensor<f32>) {
  // CHECK: ai.relu
  %0 = ai.relu %arg0 : tensor<f32>
}
```

### 端到端测试

```bash
# 编译模型
./build/bin/acompiler examples/models/simple.onnx -o model.o

# 运行模型
./build/bin/acompiler-run model.o
```

## 调试

### 打印IR

```cpp
module.dump();
```

### GDB调试

```bash
gdb --args ./build/bin/acompiler-opt input.mlir
```

### LLVM调试工具

```bash
# 打印Pass执行
acompiler-opt input.mlir -mlir-print-ir-after-all

# 打印时序
acompiler-opt input.mlir -mlir-timing
```

## 性能分析

### 编译时间

```bash
ninja clean
time ninja
```

### 运行时性能

```bash
# 使用perf
perf record ./app
perf report

# 使用valgrind
valgrind --tool=callgrind ./app
```

## 文档

### 代码注释

```cpp
/// Brief description.
///
/// Detailed description that can span
/// multiple lines.
///
/// @param arg Description of argument
/// @return Description of return value
ReturnType functionName(ArgType arg);
```

### Markdown文档

- 所有新功能需要文档
- 更新README.md
- 添加教程示例

## 常见问题

### CMake找不到LLVM

```bash
cmake .. -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm
```

### TableGen错误

检查`.td`文件语法，确保包含所需的文件。

### 链接错误

确保所有依赖库都正确链接。

## 贡献规范

- 遵循编码规范
- 所有代码需要测试
- 提交前运行格式化
- 清晰的提交信息
- 及时响应Review意见

## 联系方式

- GitHub Issues: 报告bug和提问
- GitHub Discussions: 技术讨论
- Pull Requests: 代码贡献
