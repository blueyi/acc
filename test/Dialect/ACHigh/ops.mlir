// RUN: ac-opt %s | ac-opt | FileCheck %s

// Test basic ACHigh dialect operations

// CHECK-LABEL: func.func @test_matmul
func.func @test_matmul(%arg0: tensor<4x8xf32>, %arg1: tensor<8x16xf32>) -> tensor<4x16xf32> {
  // CHECK: achigh.matmul
  %0 = achigh.matmul(%arg0, %arg1) : tensor<4x8xf32>, tensor<8x16xf32> -> tensor<4x16xf32>
  return %0 : tensor<4x16xf32>
}

// CHECK-LABEL: func.func @test_conv2d
func.func @test_conv2d(%input: tensor<1x3x224x224xf32>, %filter: tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32> {
  // CHECK: achigh.conv2d
  %0 = achigh.conv2d(%input, %filter) {strides = [2, 2], padding = [3, 3, 3, 3]} : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
  return %0 : tensor<1x64x112x112xf32>
}

// CHECK-LABEL: func.func @test_relu
func.func @test_relu(%arg0: tensor<1x64x112x112xf32>) -> tensor<1x64x112x112xf32> {
  // CHECK: achigh.relu
  %0 = achigh.relu(%arg0) : tensor<1x64x112x112xf32>
  return %0 : tensor<1x64x112x112xf32>
}

// CHECK-LABEL: func.func @test_add
func.func @test_add(%arg0: tensor<4x16xf32>, %arg1: tensor<4x16xf32>) -> tensor<4x16xf32> {
  // CHECK: achigh.add
  %0 = achigh.add(%arg0, %arg1) : tensor<4x16xf32>, tensor<4x16xf32> -> tensor<4x16xf32>
  return %0 : tensor<4x16xf32>
}
