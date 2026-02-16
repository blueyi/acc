//===- AIOps.td - AI dialect operations -------------------*- tablegen -*-===//
//
// This file defines operations in the AI dialect.
//
//===----------------------------------------------------------------------===//

#ifndef AI_OPS
#define AI_OPS

include "acompiler/Dialect/AI/AI.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

//===----------------------------------------------------------------------===//
// Conv2D Operation
//===----------------------------------------------------------------------===//

def AI_Conv2DOp : AI_Op<"conv2d", [Pure]> {
  let summary = "2D convolution operation";
  let description = [{
    Performs 2D convolution on input tensor with given weights.
    
    Syntax:
    ```
    %output = ai.conv2d %input, %weight 
      {stride = [s_h, s_w], padding = [p_h, p_w]} 
      : (tensor<NxCxHxWxf32>, tensor<KxCxRxSxf32>) -> tensor<NxKxH'xW'xf32>
    ```
  }];
  
  let arguments = (ins
    AnyTensor:$input,
    AnyTensor:$weight,
    OptionalAttr<I64ArrayAttr>:$stride,
    OptionalAttr<I64ArrayAttr>:$padding,
    OptionalAttr<I64ArrayAttr>:$dilation
  );
  
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $input `,` $weight attr-dict `:` 
    `(` type($input) `,` type($weight) `)` `->` type($output)
  }];
}

//===----------------------------------------------------------------------===//
// MatMul Operation
//===----------------------------------------------------------------------===//

def AI_MatMulOp : AI_Op<"matmul", [Pure]> {
  let summary = "Matrix multiplication operation";
  let description = [{
    Performs matrix multiplication: C = A * B
    
    Syntax:
    ```
    %C = ai.matmul %A, %B : (tensor<MxKxf32>, tensor<KxNxf32>) -> tensor<MxNxf32>
    ```
  }];
  
  let arguments = (ins AnyTensor:$lhs, AnyTensor:$rhs);
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $lhs `,` $rhs attr-dict `:` 
    `(` type($lhs) `,` type($rhs) `)` `->` type($output)
  }];
}

//===----------------------------------------------------------------------===//
// ReLU Operation
//===----------------------------------------------------------------------===//

def AI_ReLUOp : AI_Op<"relu", [Pure, SameOperandsAndResultType]> {
  let summary = "ReLU activation function";
  let description = [{
    Applies ReLU activation: output = max(0, input)
    
    Syntax:
    ```
    %output = ai.relu %input : tensor<*xf32>
    ```
  }];
  
  let arguments = (ins AnyTensor:$input);
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $input attr-dict `:` type($input)
  }];
}

//===----------------------------------------------------------------------===//
// MaxPool2D Operation
//===----------------------------------------------------------------------===//

def AI_MaxPool2DOp : AI_Op<"maxpool2d", [Pure]> {
  let summary = "2D max pooling operation";
  let description = [{
    Performs 2D max pooling on input tensor.
    
    Syntax:
    ```
    %output = ai.maxpool2d %input 
      {kernel_size = [k_h, k_w], stride = [s_h, s_w]} 
      : tensor<NxCxHxWxf32> -> tensor<NxCxH'xW'xf32>
    ```
  }];
  
  let arguments = (ins
    AnyTensor:$input,
    I64ArrayAttr:$kernel_size,
    OptionalAttr<I64ArrayAttr>:$stride,
    OptionalAttr<I64ArrayAttr>:$padding
  );
  
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $input attr-dict `:` type($input) `->` type($output)
  }];
}

//===----------------------------------------------------------------------===//
// BatchNorm Operation
//===----------------------------------------------------------------------===//

def AI_BatchNormOp : AI_Op<"batch_norm", [Pure]> {
  let summary = "Batch normalization operation";
  let description = [{
    Applies batch normalization on input tensor.
    
    Syntax:
    ```
    %output = ai.batch_norm %input, %mean, %var, %scale, %bias 
      {epsilon = 1e-5} : tensor<NxCxHxWxf32>
    ```
  }];
  
  let arguments = (ins
    AnyTensor:$input,
    AnyTensor:$mean,
    AnyTensor:$variance,
    AnyTensor:$scale,
    AnyTensor:$bias,
    OptionalAttr<F32Attr>:$epsilon
  );
  
  let results = (outs AnyTensor:$output);
}

//===----------------------------------------------------------------------===//
// Fused Conv2D + ReLU Operation
//===----------------------------------------------------------------------===//

def AI_ConvReLUOp : AI_Op<"conv_relu", [Pure]> {
  let summary = "Fused Conv2D and ReLU operation";
  let description = [{
    Performs Conv2D followed by ReLU activation (fused for efficiency).
    
    Syntax:
    ```
    %output = ai.conv_relu %input, %weight 
      {stride = [s_h, s_w], padding = [p_h, p_w]} 
      : (tensor<NxCxHxWxf32>, tensor<KxCxRxSxf32>) -> tensor<NxKxH'xW'xf32>
    ```
  }];
  
  let arguments = (ins
    AnyTensor:$input,
    AnyTensor:$weight,
    OptionalAttr<I64ArrayAttr>:$stride,
    OptionalAttr<I64ArrayAttr>:$padding
  );
  
  let results = (outs AnyTensor:$output);
  
  let assemblyFormat = [{
    $input `,` $weight attr-dict `:` 
    `(` type($input) `,` type($weight) `)` `->` type($output)
  }];
}

#endif // AI_OPS
