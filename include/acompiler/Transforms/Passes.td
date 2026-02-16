//===- Passes.td - ACompiler transform passes -------------*- tablegen -*-===//
//
// This file defines transform passes for ACompiler.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_TRANSFORMS_PASSES
#define ACOMPILER_TRANSFORMS_PASSES

include "mlir/Pass/PassBase.td"

//===----------------------------------------------------------------------===//
// ConvReLUFusion Pass
//===----------------------------------------------------------------------===//

def ConvReLUFusion : Pass<"ai-conv-relu-fusion", "::mlir::func::FuncOp"> {
  let summary = "Fuse Conv2D and ReLU operations";
  let description = [{
    This pass fuses consecutive Conv2D and ReLU operations into a single
    ConvReLU operation for better performance.
    
    Example:
    ```
    %conv = ai.conv2d %input, %weight : ...
    %relu = ai.relu %conv : ...
    ```
    
    becomes:
    ```
    %fused = ai.conv_relu %input, %weight : ...
    ```
  }];
  
  let constructor = "mlir::createConvReLUFusionPass()";
}

//===----------------------------------------------------------------------===//
// ConstantFolding Pass
//===----------------------------------------------------------------------===//

def AIConstantFolding : Pass<"ai-constant-folding", "::mlir::func::FuncOp"> {
  let summary = "Fold constant operations in AI dialect";
  let description = [{
    This pass performs constant folding for AI dialect operations.
  }];
  
  let constructor = "mlir::createAIConstantFoldingPass()";
}

//===----------------------------------------------------------------------===//
// ShapeInference Pass
//===----------------------------------------------------------------------===//

def AIShapeInference : Pass<"ai-shape-inference", "::mlir::ModuleOp"> {
  let summary = "Infer shapes for AI operations";
  let description = [{
    This pass infers and propagates tensor shapes throughout the IR.
  }];
  
  let constructor = "mlir::createAIShapeInferencePass()";
}

#endif // ACOMPILER_TRANSFORMS_PASSES
