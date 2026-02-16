# 教程 6: LLVM IR 优化

## 目标

学习 LLVM IR 优化框架，编写自定义的 LLVM IR 优化 Pass。

## 核心概念

### LLVM IR 基础
- SSA (Static Single Assignment) 形式
- 基本块 (Basic Block) 和控制流图 (CFG)
- PHI 节点
- LLVM 内置类型系统

### New Pass Manager (NPM)
- `PassInfoMixin` 基类
- `PreservedAnalyses` 返回值
- Analysis Pass 与 Transform Pass

### 常用 Analysis
- `DominatorTree`: 支配树
- `LoopInfo`: 循环信息
- `AliasAnalysis`: 别名分析
- `ScalarEvolution`: 标量进化

## 练习

1. 编写一个统计函数中基本块数量的 Analysis Pass
2. 编写一个简单的死代码消除 Pass
3. 编写一个循环展开 Pass（利用 LoopInfo）
