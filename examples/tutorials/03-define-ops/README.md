# 教程 3: 定义 AI 算子

## 目标

深入学习 MLIR Operation 的定义，包括 Traits、Interfaces、Assembly Format 等。

## 学习要点

1. **Traits**: `Pure`, `SameOperandsAndResultType`, `Commutative` 等
2. **Interfaces**: `InferTypeOpInterface`, `SideEffectInterfaces`
3. **Assembly Format**: 声明式的 IR 打印/解析格式
4. **Custom Verifier**: 自定义验证逻辑

## 练习

1. 定义一个 `achigh.batch_norm` 操作，包含 5 个输入参数
2. 为 `achigh.conv2d` 添加 `dilations` 属性
3. 实现一个自定义的 `verifier` 验证输入形状合法性
