# Dialect 目录

本目录包含ACompiler的MLIR自定义Dialect定义。

## 目录结构

- **AI/**: 核心AI算子Dialect，包含常见的深度学习算子
- **Tensor/**: 扩展的张量操作Dialect

## AI Dialect

AI Dialect定义了深度学习领域的常用算子，包括：

### 卷积算子
- `ai.conv2d`: 2D卷积
- `ai.conv3d`: 3D卷积
- `ai.depthwise_conv2d`: 深度可分离卷积

### 矩阵运算
- `ai.matmul`: 矩阵乘法
- `ai.batch_matmul`: 批量矩阵乘法

### 激活函数
- `ai.relu`: ReLU激活
- `ai.sigmoid`: Sigmoid激活
- `ai.tanh`: Tanh激活
- `ai.gelu`: GELU激活

### 池化操作
- `ai.maxpool2d`: 最大池化
- `ai.avgpool2d`: 平均池化
- `ai.global_avgpool`: 全局平均池化

### 归一化操作
- `ai.batch_norm`: 批归一化
- `ai.layer_norm`: 层归一化

## 使用示例

```mlir
func.func @example(%input: tensor<1x3x224x224xf32>) -> tensor<1x64x224x224xf32> {
  %weight = arith.constant dense<1.0> : tensor<64x3x3x3xf32>
  %conv = ai.conv2d %input, %weight {stride = [1, 1], padding = [1, 1]} 
    : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) -> tensor<1x64x224x224xf32>
  %relu = ai.relu %conv : tensor<1x64x224x224xf32>
  return %relu : tensor<1x64x224x224xf32>
}
```

## 开发指南

### 添加新算子

1. 在`AI/AIOps.td`中使用TableGen定义算子
2. 在`AI/AIOps.cpp`中实现验证逻辑和fold方法
3. 添加对应的类型和属性定义
4. 在`test/Dialect/AI/`添加测试用例
