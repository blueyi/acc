# 教程 2: 创建第一个 MLIR 方言

## 目标

学习如何使用 TableGen/ODS 定义一个 MLIR 方言，理解 Dialect、Operation、Type 等核心概念。

## 核心概念

### Dialect（方言）

MLIR 中的方言是一组相关操作、类型和属性的集合。每个方言都有一个唯一的命名空间。

### Operation（操作）

操作是 MLIR IR 中的基本单元，类似于指令。每个操作属于某个方言。

### Type（类型）

类型描述了值的数据类型，例如 `tensor<4x4xf32>`。

### Attribute（属性）

属性是编译期常量元数据，附加在操作上。

## 学习路径

1. 阅读 `include/ACC/Dialect/ACHigh/ACHighDialect.td`
2. 理解 ODS 语法中各字段的含义
3. 查看 TableGen 生成的 C++ 代码
4. 尝试添加一个新的操作（例如 `achigh.sigmoid`）

## 练习

1. 在 ACHigh 方言中添加 `sigmoid` 操作
2. 添加 `batch_norm` 操作（带多个输入输出）
3. 为操作添加验证逻辑（`verifier`）
