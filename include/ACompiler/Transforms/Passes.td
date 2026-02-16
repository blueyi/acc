//===- Passes.td - ACompiler Transform Passes Definition ---*- tablegen -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_TRANSFORMS_PASSES_TD
#define ACOMPILER_TRANSFORMS_PASSES_TD

include "mlir/Pass/PassBase.td"

def ConstantFolding : Pass<"ac-constant-folding"> {
  let summary = "Fold constant expressions at compile time";
  let description = [{
    This pass evaluates operations with constant operands at compile time,
    replacing them with their computed results. This reduces runtime
    computation and enables further optimization opportunities.
  }];
  let constructor = "acompiler::createConstantFoldingPass()";
}

def OpFusion : Pass<"ac-op-fusion"> {
  let summary = "Fuse compatible adjacent operations";
  let description = [{
    This pass identifies and fuses compatible operation patterns such as
    Conv2D + BatchNorm + ReLU into a single fused operation. Operator fusion
    reduces memory bandwidth requirements and improves cache utilization.
  }];
  let constructor = "acompiler::createOpFusionPass()";
}

def ShapeInference : Pass<"ac-shape-inference"> {
  let summary = "Infer tensor shapes through the computation graph";
  let description = [{
    This pass propagates known tensor shapes through the computation graph,
    resolving dynamic dimensions where possible. Shape information is critical
    for subsequent optimizations like tiling and vectorization.
  }];
  let constructor = "acompiler::createShapeInferencePass()";
}

def LayoutTransform : Pass<"ac-layout-transform"> {
  let summary = "Optimize data layout for target architecture";
  let description = [{
    This pass analyzes and transforms tensor data layouts (e.g., NCHW to NHWC)
    to better match the target hardware's memory access patterns and SIMD
    instruction requirements.
  }];
  let constructor = "acompiler::createLayoutTransformPass()";
}

def DeadCodeElimination : Pass<"ac-dead-code-elimination"> {
  let summary = "Eliminate unused operations and values";
  let description = [{
    This pass removes operations whose results are not used by any other
    operation, reducing the IR size and compilation time.
  }];
  let constructor = "acompiler::createDeadCodeEliminationPass()";
}

#endif // ACOMPILER_TRANSFORMS_PASSES_TD
