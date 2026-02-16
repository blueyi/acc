// RUN: acompiler-opt %s -ai-conv-relu-fusion | FileCheck %s

// Conv + ReLU fusion example

module {
  // CHECK-LABEL: func @test_fusion
  func.func @test_fusion(%input: tensor<1x3x224x224xf32>, 
                          %weight: tensor<64x3x3x3xf32>) 
                          -> tensor<1x64x224x224xf32> {
    // CHECK: ai.conv_relu
    // CHECK-NOT: ai.conv2d
    // CHECK-NOT: ai.relu
    %conv = ai.conv2d %input, %weight {
      stride = array<i64: 1, 1>,
      padding = array<i64: 1, 1>
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) -> tensor<1x64x224x224xf32>
    
    %relu = ai.relu %conv : tensor<1x64x224x224xf32>
    
    return %relu : tensor<1x64x224x224xf32>
  }
}
