// RUN: ac-opt %s --ac-op-fusion | FileCheck %s

// Test operator fusion pass

// CHECK-LABEL: func.func @test_conv_relu_fusion
func.func @test_conv_relu_fusion(%input: tensor<1x3x224x224xf32>, %filter: tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32> {
  %conv = achigh.conv2d(%input, %filter) {strides = [2, 2], padding = [3, 3, 3, 3]} : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
  %relu = achigh.relu(%conv) : tensor<1x64x112x112xf32>
  // TODO: After fusion, expect a single fused operation
  // CHECK: achigh.relu
  return %relu : tensor<1x64x112x112xf32>
}
