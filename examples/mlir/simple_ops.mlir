// RUN: acompiler-opt %s | FileCheck %s

// Simple AI operations example

module {
  // CHECK-LABEL: func @conv2d_example
  func.func @conv2d_example(%input: tensor<1x3x224x224xf32>, 
                             %weight: tensor<64x3x7x7xf32>) 
                             -> tensor<1x64x112x112xf32> {
    // CHECK: ai.conv2d
    %output = ai.conv2d %input, %weight {
      stride = array<i64: 2, 2>,
      padding = array<i64: 3, 3>
    } : (tensor<1x3x224x224xf32>, tensor<64x3x7x7xf32>) -> tensor<1x64x112x112xf32>
    
    return %output : tensor<1x64x112x112xf32>
  }

  // CHECK-LABEL: func @matmul_example
  func.func @matmul_example(%lhs: tensor<128x256xf32>, 
                             %rhs: tensor<256x512xf32>) 
                             -> tensor<128x512xf32> {
    // CHECK: ai.matmul
    %output = ai.matmul %lhs, %rhs 
      : (tensor<128x256xf32>, tensor<256x512xf32>) -> tensor<128x512xf32>
    
    return %output : tensor<128x512xf32>
  }

  // CHECK-LABEL: func @relu_example
  func.func @relu_example(%input: tensor<1x64x112x112xf32>) 
                          -> tensor<1x64x112x112xf32> {
    // CHECK: ai.relu
    %output = ai.relu %input : tensor<1x64x112x112xf32>
    
    return %output : tensor<1x64x112x112xf32>
  }

  // CHECK-LABEL: func @conv_relu_pattern
  func.func @conv_relu_pattern(%input: tensor<1x3x224x224xf32>, 
                                %weight: tensor<64x3x3x3xf32>) 
                                -> tensor<1x64x224x224xf32> {
    // This pattern should be fused by ConvReLUFusion pass
    %conv = ai.conv2d %input, %weight {
      stride = array<i64: 1, 1>,
      padding = array<i64: 1, 1>
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>) -> tensor<1x64x224x224xf32>
    
    %relu = ai.relu %conv : tensor<1x64x224x224xf32>
    
    return %relu : tensor<1x64x224x224xf32>
  }
}
