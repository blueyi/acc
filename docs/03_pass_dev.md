# MLIR Pass 编写指南（占位）

建议内容结构：

- Pass 的基本骨架（OperationPass / ModulePass 等）
- `getDependentDialects` 与 registry
- Pattern + GreedyRewrite
- pass option（参数化）
- 计时与统计（`-mlir-pass-statistics` 等）
- lit + FileCheck 测试写法约定

