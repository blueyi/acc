//===- ACHighDialect.td - ACHigh Dialect Definition --------*- tablegen -*-===//
//
// Part of the ACompiler Project
// https://github.com/acompiler/acompiler
//
//===----------------------------------------------------------------------===//
//
// This file defines the ACHigh dialect — the high-level AI dialect in
// ACompiler. It represents AI model operations at the graph level, including
// convolutions, matrix multiplications, activations, and other common
// neural network operations.
//
//===----------------------------------------------------------------------===//

#ifndef ACHIGH_DIALECT_TD
#define ACHIGH_DIALECT_TD

include "mlir/IR/OpBase.td"

def ACHigh_Dialect : Dialect {
  let name = "achigh";
  let summary = "High-level AI dialect for graph-level representation";
  let description = [{
    The ACHigh dialect represents AI model computations at the graph level.
    Operations in this dialect correspond to high-level neural network layers
    such as convolution, matrix multiplication, pooling, and activation
    functions. This dialect serves as the entry point for AI model compilation
    and is the target of the frontend (DSL/ONNX import).

    Key design goals:
    - Faithful representation of common AI operations
    - Support for shape inference and type propagation
    - Enable graph-level optimizations (operator fusion, constant folding, etc.)
    - Clean lowering path to ACMid dialect for loop-level optimization
  }];

  let cppNamespace = "::acompiler::achigh";
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
}

// Base class for ACHigh dialect operations
class ACHigh_Op<string mnemonic, list<Trait> traits = []> :
    Op<ACHigh_Dialect, mnemonic, traits>;

#endif // ACHIGH_DIALECT_TD
