// RUN: ac-opt %s --ac-constant-folding | FileCheck %s

// Test constant folding pass (pass runs without error; op structure preserved)

// CHECK-LABEL: func.func @test_add
func.func @test_add(%arg0: tensor<4x16xf32>, %arg1: tensor<4x16xf32>) -> tensor<4x16xf32> {
  // CHECK: achigh.add
  %0 = achigh.add(%arg0, %arg1) : tensor<4x16xf32>, tensor<4x16xf32> -> tensor<4x16xf32>
  return %0 : tensor<4x16xf32>
}
