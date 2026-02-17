//===- ACMidDialect.td - ACMid Dialect Definition ----------*- tablegen -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// This file defines the ACMid dialect — the mid-level dialect in ACC.
// It represents computations at the tile/loop level, bridging the gap between
// graph-level operations (ACHigh) and low-level LLVM Dialect operations.
//
//===----------------------------------------------------------------------===//

#ifndef ACMID_DIALECT_TD
#define ACMID_DIALECT_TD

include "mlir/IR/OpBase.td"

def ACMid_Dialect : Dialect {
  let name = "acmid";
  let summary = "Mid-level dialect for tile and loop-level representation";
  let description = [{
    The ACMid dialect represents computations at the tile and loop level.
    It serves as an intermediate representation between the graph-level ACHigh
    dialect and the low-level MLIR built-in dialects (Linalg, Affine, SCF).

    Key responsibilities:
    - Express tiled computations with explicit loop structures
    - Enable loop-level optimizations (tiling, interchange, fusion, etc.)
    - Provide a clear lowering target from ACHigh dialect
    - Bridge to MLIR's Linalg/Affine ecosystem for further lowering
  }];

  let cppNamespace = "::acc::acmid";
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
}

class ACMid_Op<string mnemonic, list<Trait> traits = []> :
    Op<ACMid_Dialect, mnemonic, traits>;

#endif // ACMID_DIALECT_TD
