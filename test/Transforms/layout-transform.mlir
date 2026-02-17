// RUN: ac-opt %s --ac-layout-transform | FileCheck %s

// Test layout transform pass (pass runs; conv2d preserved)

// CHECK-LABEL: func.func @test_conv2d
func.func @test_conv2d(%input: tensor<1x3x224x224xf32>, %filter: tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32> {
  // CHECK: achigh.conv2d
  %0 = achigh.conv2d(%input, %filter) {strides = [2, 2], padding = [3, 3, 3, 3]} : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
  return %0 : tensor<1x64x112x112xf32>
}
