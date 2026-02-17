//===- ACHighOps.td - ACHigh Operations Definition ---------*- tablegen -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Defines the operations for the ACHigh dialect, including common AI/ML
// operations: convolution, matrix multiplication, activation functions,
// pooling, normalization, and other neural network operations.
//
//===----------------------------------------------------------------------===//

#ifndef ACHIGH_OPS_TD
#define ACHIGH_OPS_TD

include "ACC/Dialect/ACHigh/ACHighDialect.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// MatMul Operation
//===----------------------------------------------------------------------===//

def ACHigh_MatMulOp : ACHigh_Op<"matmul", [Pure]> {
  let summary = "Matrix multiplication operation";
  let description = [{
    Performs matrix multiplication of two tensors: C = A * B.
    Supports batch dimensions.
  }];

  let arguments = (ins
    AnyRankedTensor:$lhs,
    AnyRankedTensor:$rhs
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $lhs `,` $rhs `)` attr-dict `:` type($lhs) `,` type($rhs) `->` type($result)
  }];
}

//===----------------------------------------------------------------------===//
// BatchMatMul Operation (supports batch dimensions and transpose)
//===----------------------------------------------------------------------===//

def ACHigh_BatchMatMulOp : ACHigh_Op<"batch_matmul", [Pure]> {
  let summary = "Batch matrix multiplication";
  let description = [{
    Batch matrix multiplication: C[b,m,n] = A[b,m,k] @ B[b,k,n]
    Supports broadcasting and optional transposition of operands.
  }];

  let arguments = (ins
    AnyRankedTensor:$lhs,
    AnyRankedTensor:$rhs,
    DefaultValuedAttr<BoolAttr, "false">:$transpose_lhs,
    DefaultValuedAttr<BoolAttr, "false">:$transpose_rhs
  );
  let results = (outs AnyRankedTensor:$output);

  let assemblyFormat = [{
    $lhs `,` $rhs attr-dict `:`
    `(` type($lhs) `,` type($rhs) `)` `->` type($output)
  }];
}

//===----------------------------------------------------------------------===//
// Conv2D Operation (enhanced: supports bias, dilation, groups)
//===----------------------------------------------------------------------===//

def ACHigh_Conv2DOp : ACHigh_Op<"conv2d",
    [Pure, DeclareOpInterfaceMethods<InferTypeOpInterface>]> {
  let summary = "2D convolution operation";
  let description = [{
    Performs 2D convolution: output[n,k,h',w'] = sum(input[n,c,h,w] * weight[k,c,r,s])
    Supports configurable strides, padding, dilation, groups, and optional bias.

    Example:
    ```mlir
    %out = achigh.conv2d %input, %weight, %bias {
      strides = [1, 1], padding = [1, 1], dilation = [1, 1], groups = 1
    } : (tensor<1x3x224x224xf32>, tensor<64x3x3x3xf32>, tensor<64xf32>)
      -> tensor<1x64x224x224xf32>
    ```
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$weight,
    Optional<AnyRankedTensor>:$bias,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$strides,
    DefaultValuedAttr<I64ArrayAttr, "{0, 0}">:$padding,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$dilation,
    DefaultValuedAttr<I64Attr, "1">:$groups
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    $input `,` $weight (`,` $bias^)? attr-dict `:`
    `(` type($input) `,` type($weight) (`,` type($bias)^)? `)` `->` type($result)
  }];

  let hasVerifier = 1;
  let hasFolder = 1;
}

//===----------------------------------------------------------------------===//
// ReLU Operation
//===----------------------------------------------------------------------===//

def ACHigh_ReLUOp : ACHigh_Op<"relu", [Pure, SameOperandsAndResultType]> {
  let summary = "Rectified Linear Unit activation";
  let description = [{
    Applies the ReLU activation function element-wise: output = max(0, input).
  }];

  let arguments = (ins AnyRankedTensor:$input);
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` type($input)
  }];
}

//===----------------------------------------------------------------------===//
// Add Operation
//===----------------------------------------------------------------------===//

def ACHigh_AddOp : ACHigh_Op<"add", [Pure, Commutative]> {
  let summary = "Element-wise addition";
  let description = [{
    Performs element-wise addition of two tensors with broadcasting support.
  }];

  let arguments = (ins
    AnyRankedTensor:$lhs,
    AnyRankedTensor:$rhs
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $lhs `,` $rhs `)` attr-dict `:` type($lhs) `,` type($rhs) `->` type($result)
  }];
}

//===----------------------------------------------------------------------===//
// Softmax Operation
//===----------------------------------------------------------------------===//

def ACHigh_SoftmaxOp : ACHigh_Op<"softmax", [Pure, SameOperandsAndResultType]> {
  let summary = "Softmax activation";
  let description = [{
    Applies the softmax function along the specified axis.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    DefaultValuedAttr<I64Attr, "-1">:$axis
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` type($input)
  }];
}

//===----------------------------------------------------------------------===//
// Constant Operation
//===----------------------------------------------------------------------===//

def ACHigh_ConstantOp : ACHigh_Op<"constant", [Pure, ConstantLike]> {
  let summary = "Constant tensor value";
  let description = [{
    Produces a constant tensor value.
  }];

  let arguments = (ins ElementsAttr:$value);
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    $value attr-dict `:` type($result)
  }];

  let hasFolder = 1;
}

//===----------------------------------------------------------------------===//
// MaxPool2D Operation
//===----------------------------------------------------------------------===//

def ACHigh_MaxPool2DOp : ACHigh_Op<"maxpool2d", [Pure]> {
  let summary = "2D max pooling operation";
  let description = [{
    Applies 2D max pooling over the input tensor.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    I64ArrayAttr:$kernel_size,
    OptionalAttr<I64ArrayAttr>:$strides,
    OptionalAttr<I64ArrayAttr>:$padding
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` functional-type(operands, results)
  }];
}

//===----------------------------------------------------------------------===//
// Reshape Operation
//===----------------------------------------------------------------------===//

def ACHigh_ReshapeOp : ACHigh_Op<"reshape", [Pure]> {
  let summary = "Tensor reshape operation";
  let description = [{
    Reshapes the input tensor to a new shape.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    I64ArrayAttr:$new_shape
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` functional-type(operands, results)
  }];
}

//===----------------------------------------------------------------------===//
// GELU Activation (Transformer core activation)
//===----------------------------------------------------------------------===//

def ACHigh_GELUOp : ACHigh_Op<"gelu", [Pure, SameOperandsAndResultType]> {
  let summary = "Gaussian Error Linear Unit activation";
  let description = [{
    GELU(x) = x * Phi(x) where Phi(x) is the CDF of the standard normal
    distribution. Widely used in Transformer architectures (BERT, GPT, etc.).
  }];

  let arguments = (ins AnyRankedTensor:$input);
  let results = (outs AnyRankedTensor:$output);

  let assemblyFormat = "$input attr-dict `:` type($input)";
}

//===----------------------------------------------------------------------===//
// Sigmoid Activation
//===----------------------------------------------------------------------===//

def ACHigh_SigmoidOp : ACHigh_Op<"sigmoid", [Pure, SameOperandsAndResultType]> {
  let summary = "Sigmoid activation function";
  let description = [{
    Sigmoid(x) = 1 / (1 + exp(-x)). Used in gating mechanisms and
    binary classification outputs.
  }];

  let arguments = (ins AnyRankedTensor:$input);
  let results = (outs AnyRankedTensor:$output);

  let assemblyFormat = "$input attr-dict `:` type($input)";
}

//===----------------------------------------------------------------------===//
// Tanh Activation
//===----------------------------------------------------------------------===//

def ACHigh_TanhOp : ACHigh_Op<"tanh", [Pure, SameOperandsAndResultType]> {
  let summary = "Hyperbolic tangent activation";
  let description = [{
    Tanh(x) = (exp(x) - exp(-x)) / (exp(x) + exp(-x)).
  }];

  let arguments = (ins AnyRankedTensor:$input);
  let results = (outs AnyRankedTensor:$output);

  let assemblyFormat = "$input attr-dict `:` type($input)";
}

//===----------------------------------------------------------------------===//
// BatchNorm Operation
//===----------------------------------------------------------------------===//

def ACHigh_BatchNormOp : ACHigh_Op<"batch_norm", [Pure]> {
  let summary = "Batch normalization";
  let description = [{
    Applies batch normalization over the input:
    output = scale * (input - mean) / sqrt(variance + epsilon) + bias
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$scale,
    AnyRankedTensor:$bias,
    AnyRankedTensor:$mean,
    AnyRankedTensor:$variance,
    DefaultValuedAttr<F64Attr, "1e-5">:$epsilon,
    DefaultValuedAttr<F64Attr, "0.1">:$momentum
  );
  let results = (outs AnyRankedTensor:$output);

  let assemblyFormat = [{
    `(` $input `,` $scale `,` $bias `,` $mean `,` $variance `)`
    attr-dict `:` functional-type(operands, results)
  }];
}

//===----------------------------------------------------------------------===//
// ConvReLU Fused Operation (target of Conv+ReLU fusion pass)
//===----------------------------------------------------------------------===//

def ACHigh_ConvReLUOp : ACHigh_Op<"conv_relu", [Pure]> {
  let summary = "Fused Conv2D + ReLU operation";
  let description = [{
    A fused convolution followed by ReLU activation. This is the result of
    the operator fusion pass combining achigh.conv2d + achigh.relu into a
    single operation to reduce memory traffic.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$weight,
    Optional<AnyRankedTensor>:$bias,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$strides,
    DefaultValuedAttr<I64ArrayAttr, "{0, 0}">:$padding,
    DefaultValuedAttr<I64ArrayAttr, "{1, 1}">:$dilation,
    DefaultValuedAttr<I64Attr, "1">:$groups
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    $input `,` $weight (`,` $bias^)? attr-dict `:`
    `(` type($input) `,` type($weight) (`,` type($bias)^)? `)` `->` type($result)
  }];
}

//===----------------------------------------------------------------------===//
// Mul Operation (element-wise)
//===----------------------------------------------------------------------===//

def ACHigh_MulOp : ACHigh_Op<"mul", [Pure, Commutative]> {
  let summary = "Element-wise multiplication";
  let description = [{
    Performs element-wise multiplication with broadcasting support.
  }];

  let arguments = (ins
    AnyRankedTensor:$lhs,
    AnyRankedTensor:$rhs
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $lhs `,` $rhs `)` attr-dict `:` type($lhs) `,` type($rhs) `->` type($result)
  }];
}

//===----------------------------------------------------------------------===//
// Flatten Operation
//===----------------------------------------------------------------------===//

def ACHigh_FlattenOp : ACHigh_Op<"flatten", [Pure]> {
  let summary = "Flatten tensor dimensions";
  let description = [{
    Flattens tensor from start_dim to end_dim into a single dimension.
    Default: flatten all dims except batch (start_dim=1).
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    DefaultValuedAttr<I64Attr, "1">:$start_dim,
    DefaultValuedAttr<I64Attr, "-1">:$end_dim
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` functional-type(operands, results)
  }];
}

//===----------------------------------------------------------------------===//
// Dense (Fully Connected / Linear) Operation
//===----------------------------------------------------------------------===//

def ACHigh_DenseOp : ACHigh_Op<"dense", [Pure]> {
  let summary = "Fully connected (linear) layer";
  let description = [{
    Performs a linear transformation: output = input @ weight^T + bias.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$weight,
    Optional<AnyRankedTensor>:$bias
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    $input `,` $weight (`,` $bias^)? attr-dict `:`
    `(` type($input) `,` type($weight) (`,` type($bias)^)? `)` `->` type($result)
  }];
}

//===----------------------------------------------------------------------===//
// AveragePool2D Operation
//===----------------------------------------------------------------------===//

def ACHigh_AvgPool2DOp : ACHigh_Op<"avgpool2d", [Pure]> {
  let summary = "2D average pooling operation";
  let description = [{
    Applies 2D average pooling over the input tensor.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    I64ArrayAttr:$kernel_size,
    OptionalAttr<I64ArrayAttr>:$strides,
    OptionalAttr<I64ArrayAttr>:$padding
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` functional-type(operands, results)
  }];
}

//===----------------------------------------------------------------------===//
// Transpose Operation
//===----------------------------------------------------------------------===//

def ACHigh_TransposeOp : ACHigh_Op<"transpose", [Pure]> {
  let summary = "Tensor transpose / permute dimensions";
  let description = [{
    Permutes the dimensions of the input tensor according to the given order.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    I64ArrayAttr:$perm
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `)` attr-dict `:` functional-type(operands, results)
  }];
}

#endif // ACHIGH_OPS_TD
