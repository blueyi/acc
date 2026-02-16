# Transforms 目录

本目录包含各种优化和变换Pass。

## Pass分类

### 融合Pass (FusionPasses)

算子融合优化，减少内存访问和kernel启动开销。

**Pass列表**:
- `ConvReLUFusion`: 卷积+ReLU融合
- `MatMulAddFusion`: 矩阵乘+加法融合（对应GEMM）
- `BatchNormFusion`: 批归一化融合
- `GenericFusion`: 通用融合框架

**示例**:
```mlir
// 融合前
%conv = ai.conv2d %input, %weight : ...
%relu = ai.relu %conv : ...

// 融合后
%fused = ai.conv_relu %input, %weight : ...
```

### 优化Pass (OptimizationPasses)

通用优化技术。

**Pass列表**:
- `ConstantFolding`: 常量折叠
- `DeadCodeElimination`: 死代码消除
- `CommonSubexpressionElimination`: 公共子表达式消除
- `MemoryOptimization`: 内存优化
- `ShapeInference`: 形状推导

## 使用方法

```bash
# 运行融合Pass
acompiler-opt --ai-conv-relu-fusion input.mlir

# 运行优化Pass
acompiler-opt --ai-constant-folding input.mlir

# 组合使用
acompiler-opt --ai-conv-relu-fusion --ai-constant-folding input.mlir
```

## Pass开发模板

```cpp
struct MyOptimizationPass : public PassWrapper<MyOptimizationPass, 
                                                OperationPass<func::FuncOp>> {
  void runOnOperation() override {
    auto func = getOperation();
    // 实现Pass逻辑
  }
};
```
