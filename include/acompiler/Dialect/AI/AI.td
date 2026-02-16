//===- AI.td - AI dialect definition ----------------------*- tablegen -*-===//
//
// This file defines the AI dialect.
//
//===----------------------------------------------------------------------===//

#ifndef AI_DIALECT
#define AI_DIALECT

include "mlir/IR/OpBase.td"
include "mlir/IR/AttrTypeBase.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Interfaces/InferTypeOpInterface.td"

//===----------------------------------------------------------------------===//
// AI Dialect Definition
//===----------------------------------------------------------------------===//

def AI_Dialect : Dialect {
  let name = "ai";
  let summary = "AI operations dialect for deep learning";
  let description = [{
    This dialect defines operations commonly used in deep learning and AI
    applications, including convolution, matrix multiplication, activation
    functions, pooling, and normalization operations.
  }];
  
  let cppNamespace = "::mlir::ai";
  
  let useDefaultTypePrinterParser = 1;
  let useDefaultAttributePrinterParser = 1;
}

//===----------------------------------------------------------------------===//
// AI Type Definitions
//===----------------------------------------------------------------------===//

class AI_Type<string name, string typeMnemonic, list<Trait> traits = []>
    : TypeDef<AI_Dialect, name, traits> {
  let mnemonic = typeMnemonic;
}

//===----------------------------------------------------------------------===//
// AI Attribute Definitions
//===----------------------------------------------------------------------===//

class AI_Attr<string name, string attrMnemonic, list<Trait> traits = []>
    : AttrDef<AI_Dialect, name, traits> {
  let mnemonic = attrMnemonic;
}

//===----------------------------------------------------------------------===//
// Base AI Operation
//===----------------------------------------------------------------------===//

class AI_Op<string mnemonic, list<Trait> traits = []> :
    Op<AI_Dialect, mnemonic, traits>;

#endif // AI_DIALECT
