// RUN: ac-opt %s --ac-dead-code-elimination | FileCheck %s

// Test dead code elimination pass (pass runs; function preserved)

// CHECK-LABEL: func.func @test_relu
func.func @test_relu(%arg0: tensor<1x64x112x112xf32>) -> tensor<1x64x112x112xf32> {
  // CHECK: achigh.relu
  %0 = achigh.relu(%arg0) : tensor<1x64x112x112xf32> -> tensor<1x64x112x112xf32>
  return %0 : tensor<1x64x112x112xf32>
}
