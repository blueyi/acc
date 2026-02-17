//===- ACMidOps.td - ACMid Operations Definition -----------*- tablegen -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Defines operations for the ACMid dialect at the tile/loop level.
//
//===----------------------------------------------------------------------===//

#ifndef ACMID_OPS_TD
#define ACMID_OPS_TD

include "ACC/Dialect/ACMid/ACMidDialect.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/IR/OpBase.td"

// Compatibility alias: some MLIR versions don't provide this predefined union.
def AnyMemRefOrRankedTensor : AnyTypeOf<[AnyMemRef, AnyRankedTensor]>;

//===----------------------------------------------------------------------===//
// TiledMatMul Operation
//===----------------------------------------------------------------------===//

def ACMid_TiledMatMulOp : ACMid_Op<"tiled_matmul", [Pure]> {
  let summary = "Tiled matrix multiplication";
  let description = [{
    Represents a tiled matrix multiplication operation where the computation
    has been decomposed into tiles for better cache locality.
  }];

  let arguments = (ins
    AnyMemRefOrRankedTensor:$lhs,
    AnyMemRefOrRankedTensor:$rhs,
    AnyMemRefOrRankedTensor:$output,
    I64ArrayAttr:$tile_sizes
  );
  let results = (outs Optional<AnyRankedTensor>:$result);
}

//===----------------------------------------------------------------------===//
// TiledConv2D Operation
//===----------------------------------------------------------------------===//

def ACMid_TiledConv2DOp : ACMid_Op<"tiled_conv2d", [Pure]> {
  let summary = "Tiled 2D convolution";
  let description = [{
    Represents a tiled 2D convolution operation.
  }];

  let arguments = (ins
    AnyMemRefOrRankedTensor:$input,
    AnyMemRefOrRankedTensor:$filter,
    AnyMemRefOrRankedTensor:$output,
    I64ArrayAttr:$tile_sizes,
    OptionalAttr<I64ArrayAttr>:$strides,
    OptionalAttr<I64ArrayAttr>:$padding
  );
  let results = (outs Optional<AnyRankedTensor>:$result);
}

//===----------------------------------------------------------------------===//
// ElementwiseUnary Operation (generic unary op at mid level)
//===----------------------------------------------------------------------===//

def ACMid_ElemUnaryOp : ACMid_Op<"elem_unary", [Pure]> {
  let summary = "Element-wise unary operation";
  let description = [{
    Represents a generic element-wise unary operation (ReLU, Sigmoid, etc.)
    at the tile level.
  }];

  let arguments = (ins
    AnyMemRefOrRankedTensor:$input,
    StrAttr:$op_type
  );
  let results = (outs AnyRankedTensor:$result);
}

//===----------------------------------------------------------------------===//
// ElementwiseBinary Operation (generic binary op at mid level)
//===----------------------------------------------------------------------===//

def ACMid_ElemBinaryOp : ACMid_Op<"elem_binary", [Pure]> {
  let summary = "Element-wise binary operation";
  let description = [{
    Represents a generic element-wise binary operation (Add, Mul, etc.)
    at the tile level.
  }];

  let arguments = (ins
    AnyMemRefOrRankedTensor:$lhs,
    AnyMemRefOrRankedTensor:$rhs,
    StrAttr:$op_type
  );
  let results = (outs AnyRankedTensor:$result);
}

#endif // ACMID_OPS_TD
