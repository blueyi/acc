module {
  func.func @main(%input: tensor<1x8x8x3xf32>, %filter: tensor<3x3x3x8xf32>) -> tensor<1x6x6x8xf32> {
    %0 = "acompiler.conv2d"(%input, %filter)
      : (tensor<1x8x8x3xf32>, tensor<3x3x3x8xf32>) -> tensor<1x6x6x8xf32>
    %1 = "acompiler.relu"(%0) : (tensor<1x6x6x8xf32>) -> tensor<1x6x6x8xf32>
    return %1 : tensor<1x6x6x8xf32>
  }
}
