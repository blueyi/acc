// ResNet基本块示例
// examples/mlir/resnet_block.mlir

// RUN: acompiler-opt %s | FileCheck %s

module {
  // ResNet-18的基本残差块
  // BasicBlock: 3x3 conv -> BN -> ReLU -> 3x3 conv -> BN -> (+shortcut) -> ReLU
  
  // CHECK-LABEL: @resnet_basic_block
  func.func @resnet_basic_block(%input: tensor<1x64x56x56xf32>) 
                                 -> tensor<1x64x56x56xf32> {
    // 第一个卷积层
    %weight1 = arith.constant dense<1.0> : tensor<64x64x3x3xf32>
    // CHECK: ai.conv2d
    %conv1 = ai.conv2d %input, %weight1 {
      stride = [1, 1],
      padding = [1, 1]
    } : (tensor<1x64x56x56xf32>, tensor<64x64x3x3xf32>) 
     -> tensor<1x64x56x56xf32>
    
    // BatchNorm (简化为scale+bias)
    %scale1 = arith.constant dense<1.0> : tensor<64xf32>
    %bias1 = arith.constant dense<0.0> : tensor<64xf32>
    %mean1 = arith.constant dense<0.0> : tensor<64xf32>
    %var1 = arith.constant dense<1.0> : tensor<64xf32>
    
    // CHECK: ai.batch_norm
    %bn1 = ai.batch_norm %conv1, %mean1, %var1, %scale1, %bias1 {
      epsilon = 1.0e-5 : f32
    } : (tensor<1x64x56x56xf32>, tensor<64xf32>, tensor<64xf32>, 
         tensor<64xf32>, tensor<64xf32>) -> tensor<1x64x56x56xf32>
    
    // ReLU
    // CHECK: ai.relu
    %relu1 = ai.relu %bn1 : tensor<1x64x56x56xf32>
    
    // 第二个卷积层
    %weight2 = arith.constant dense<1.0> : tensor<64x64x3x3xf32>
    %conv2 = ai.conv2d %relu1, %weight2 {
      stride = [1, 1],
      padding = [1, 1]
    } : (tensor<1x64x56x56xf32>, tensor<64x64x3x3xf32>) 
     -> tensor<1x64x56x56xf32>
    
    // BatchNorm
    %scale2 = arith.constant dense<1.0> : tensor<64xf32>
    %bias2 = arith.constant dense<0.0> : tensor<64xf32>
    %mean2 = arith.constant dense<0.0> : tensor<64xf32>
    %var2 = arith.constant dense<1.0> : tensor<64xf32>
    
    %bn2 = ai.batch_norm %conv2, %mean2, %var2, %scale2, %bias2 {
      epsilon = 1.0e-5 : f32
    } : (tensor<1x64x56x56xf32>, tensor<64xf32>, tensor<64xf32>,
         tensor<64xf32>, tensor<64xf32>) -> tensor<1x64x56x56xf32>
    
    // Residual连接 (element-wise add)
    // CHECK: ai.add
    %add = ai.add %bn2, %input : tensor<1x64x56x56xf32>
    
    // 最后的ReLU
    %relu2 = ai.relu %add : tensor<1x64x56x56xf32>
    
    return %relu2 : tensor<1x64x56x56xf32>
  }
  
  // ResNet-50的瓶颈块
  // Bottleneck: 1x1 conv -> BN -> ReLU -> 3x3 conv -> BN -> ReLU 
  //             -> 1x1 conv -> BN -> (+shortcut) -> ReLU
  
  // CHECK-LABEL: @resnet_bottleneck
  func.func @resnet_bottleneck(%input: tensor<1x256x56x56xf32>)
                                -> tensor<1x256x56x56xf32> {
    // 1x1 conv (降维)
    %weight1 = arith.constant dense<1.0> : tensor<64x256x1x1xf32>
    %conv1 = ai.conv2d %input, %weight1 {
      stride = [1, 1],
      padding = [0, 0]
    } : (tensor<1x256x56x56xf32>, tensor<64x256x1x1xf32>)
     -> tensor<1x64x56x56xf32>
    
    %bn1 = ai.batch_norm %conv1, 
                         arith.constant dense<0.0> : tensor<64xf32>,
                         arith.constant dense<1.0> : tensor<64xf32>,
                         arith.constant dense<1.0> : tensor<64xf32>,
                         arith.constant dense<0.0> : tensor<64xf32> {
      epsilon = 1.0e-5 : f32
    } : (tensor<1x64x56x56xf32>, tensor<64xf32>, tensor<64xf32>,
         tensor<64xf32>, tensor<64xf32>) -> tensor<1x64x56x56xf32>
    
    %relu1 = ai.relu %bn1 : tensor<1x64x56x56xf32>
    
    // 3x3 conv
    %weight2 = arith.constant dense<1.0> : tensor<64x64x3x3xf32>
    %conv2 = ai.conv2d %relu1, %weight2 {
      stride = [1, 1],
      padding = [1, 1]
    } : (tensor<1x64x56x56xf32>, tensor<64x64x3x3xf32>)
     -> tensor<1x64x56x56xf32>
    
    %bn2 = ai.batch_norm %conv2,
                         arith.constant dense<0.0> : tensor<64xf32>,
                         arith.constant dense<1.0> : tensor<64xf32>,
                         arith.constant dense<1.0> : tensor<64xf32>,
                         arith.constant dense<0.0> : tensor<64xf32> {
      epsilon = 1.0e-5 : f32
    } : (tensor<1x64x56x56xf32>, tensor<64xf32>, tensor<64xf32>,
         tensor<64xf32>, tensor<64xf32>) -> tensor<1x64x56x56xf32>
    
    %relu2 = ai.relu %bn2 : tensor<1x64x56x56xf32>
    
    // 1x1 conv (升维)
    %weight3 = arith.constant dense<1.0> : tensor<256x64x1x1xf32>
    %conv3 = ai.conv2d %relu2, %weight3 {
      stride = [1, 1],
      padding = [0, 0]
    } : (tensor<1x64x56x56xf32>, tensor<256x64x1x1xf32>)
     -> tensor<1x256x56x56xf32>
    
    %bn3 = ai.batch_norm %conv3,
                         arith.constant dense<0.0> : tensor<256xf32>,
                         arith.constant dense<1.0> : tensor<256xf32>,
                         arith.constant dense<1.0> : tensor<256xf32>,
                         arith.constant dense<0.0> : tensor<256xf32> {
      epsilon = 1.0e-5 : f32
    } : (tensor<1x256x56x56xf32>, tensor<256xf32>, tensor<256xf32>,
         tensor<256xf32>, tensor<256xf32>) -> tensor<1x256x56x56xf32>
    
    // Residual连接
    %add = ai.add %bn3, %input : tensor<1x256x56x56xf32>
    %relu3 = ai.relu %add : tensor<1x256x56x56xf32>
    
    return %relu3 : tensor<1x256x56x56xf32>
  }
}
