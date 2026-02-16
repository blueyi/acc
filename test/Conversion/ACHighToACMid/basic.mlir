// RUN: ac-opt %s --ac-high-to-mid | FileCheck %s

// Test basic ACHigh to ACMid lowering

// CHECK-LABEL: func.func @test_matmul_lowering
func.func @test_matmul_lowering(%arg0: tensor<4x8xf32>, %arg1: tensor<8x16xf32>) -> tensor<4x16xf32> {
  %0 = achigh.matmul(%arg0, %arg1) : tensor<4x8xf32>, tensor<8x16xf32> -> tensor<4x16xf32>
  // TODO: After lowering, expect acmid.tiled_matmul
  // CHECK: achigh.matmul
  return %0 : tensor<4x16xf32>
}
