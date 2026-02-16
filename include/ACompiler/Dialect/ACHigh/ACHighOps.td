//===- ACHighOps.td - ACHigh Operations Definition ---------*- tablegen -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// Defines the operations for the ACHigh dialect, including common AI/ML
// operations such as MatMul, Conv2D, ReLU, Add, etc.
//
//===----------------------------------------------------------------------===//

#ifndef ACHIGH_OPS_TD
#define ACHIGH_OPS_TD

include "ACompiler/Dialect/ACHigh/ACHighDialect.td"
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
// Conv2D Operation
//===----------------------------------------------------------------------===//

def ACHigh_Conv2DOp : ACHigh_Op<"conv2d", [Pure]> {
  let summary = "2D convolution operation";
  let description = [{
    Performs 2D convolution on the input tensor with the given filter.
    Supports configurable strides, padding, and dilation.
  }];

  let arguments = (ins
    AnyRankedTensor:$input,
    AnyRankedTensor:$filter,
    OptionalAttr<I64ArrayAttr>:$strides,
    OptionalAttr<I64ArrayAttr>:$padding,
    OptionalAttr<I64ArrayAttr>:$dilation
  );
  let results = (outs AnyRankedTensor:$result);

  let assemblyFormat = [{
    `(` $input `,` $filter `)` attr-dict `:` functional-type(operands, results)
  }];
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

#endif // ACHIGH_OPS_TD
