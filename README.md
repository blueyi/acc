# ACompiler

`ACompiler` 是一个基于 LLVM/MLIR 的 AI 编译器学习与实现项目，目标是通过“实例驱动”的方式，快速建立从高层 IR 到底层代码生成的完整编译器认知与实战能力。

## 项目定位

- 学习路径：**由浅入深、先 MLIR 后 LLVM IR、最终到 CodeGen**
- 实现策略：**先 CPU 单后端打通完整链路，再扩展多后端**
- 实践原则：**每个阶段都有可运行样例、可观测 IR、可验证结果**

## 第一阶段范围（当前目标）

- 支持后端：`CPU`（唯一实现后端）
- 核心内容：
  - AI 计算图到 MLIR 的表达
  - MLIR Dialect/Pass/Lowering
  - LLVM IR 优化与指令映射
  - CodeGen 与可执行产物验证

## 后续后端（仅列入 TODO，暂不实现）

- NVIDIA GPU（CUDA）
- Huawei Ascend NPU（AscendNPU IR）

详见：

- 详细规划：[`docs/ACompiler_Project_Plan.md`](docs/ACompiler_Project_Plan.md)
- 待办列表：[`TODO.md`](TODO.md)

## 当前仓库状态（基础工程已落地）

已完成基础工程配置与目录骨架，包含：

- CMake 顶层构建与模块拆分（`lib/`、`runtime/`、`tools/`、`test/`、`benchmark/`）
- Frontend/Runtime 最小可编译代码
- CLI 工具入口（`acompiler-c`、`acompiler-opt`、`acompiler-translate`、`acompiler-run`）
- 示例与脚本（`examples/`、`scripts/`）
- 文档占位（MLIR 概念、Pass 开发、LLVM 后端笔记、Roadmap）

## 快速开始

```bash
./scripts/configure.sh
./scripts/build.sh
./scripts/test.sh
./build/tools/acompiler-run
```
