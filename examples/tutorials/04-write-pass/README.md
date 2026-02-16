# 教程 4: 编写 MLIR 优化 Pass

## 目标

学习 MLIR Pass 框架，编写常量折叠和算子融合优化 Pass。

## 核心概念

### Pass 管理框架
- `PassManager` / `OpPassManager`
- `FunctionPass` / `OperationPass`
- Pass 注册与命令行集成

### Pattern Rewriting
- `RewritePattern` 基类
- `matchAndRewrite` 方法
- `GreedyPatternRewriteDriver`

### DRR (Declarative Rewrite Rules)
- 使用 TableGen 声明式定义模式替换
- 自动生成 C++ 匹配/重写代码

## 练习

1. 实现一个简单的常量折叠 Pass（折叠 `achigh.add(constant, constant)`）
2. 使用 DRR 实现 `ReLU(ReLU(x)) → ReLU(x)` 简化
3. 实现 `Conv2D + ReLU → FusedConvReLU` 融合
