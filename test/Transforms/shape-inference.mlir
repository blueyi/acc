// RUN: ac-opt %s --ac-shape-inference | FileCheck %s

// Test shape inference pass (pass runs; ACHigh ops preserved)

// CHECK-LABEL: func.func @test_matmul
func.func @test_matmul(%arg0: tensor<4x8xf32>, %arg1: tensor<8x16xf32>) -> tensor<4x16xf32> {
  // CHECK: achigh.matmul
  %0 = achigh.matmul(%arg0, %arg1) : tensor<4x8xf32>, tensor<8x16xf32> -> tensor<4x16xf32>
  return %0 : tensor<4x16xf32>
}
