// Expected ACHigh MLIR for the add-only ONNX model (model.onnx).
// When ONNX import is implemented, ac-translate --import-onnx model.onnx
// should produce a module equivalent to this.

module {
  func.func @add_model(%X: tensor<4x16xf32>, %Y: tensor<4x16xf32>) -> tensor<4x16xf32> {
    %Z = achigh.add(%X, %Y) : tensor<4x16xf32>, tensor<4x16xf32> -> tensor<4x16xf32>
    return %Z : tensor<4x16xf32>
  }
}
