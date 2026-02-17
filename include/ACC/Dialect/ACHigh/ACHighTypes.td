//===- ACHighTypes.td - ACHigh Type Definitions ------------*- tablegen -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Defines custom types for the ACHigh dialect, including quantized tensor
// types for INT8/INT4 inference support.
//
//===----------------------------------------------------------------------===//

#ifndef ACHIGH_TYPES_TD
#define ACHIGH_TYPES_TD

include "ACC/Dialect/ACHigh/ACHighDialect.td"
include "mlir/IR/AttrTypeBase.td"

class ACHigh_Type<string name, string typeMnemonic, list<Trait> traits = []>
    : TypeDef<ACHigh_Dialect, name, traits> {
  let mnemonic = typeMnemonic;
}

//===----------------------------------------------------------------------===//
// Quantized Tensor Type
//===----------------------------------------------------------------------===//

def ACHigh_QuantizedType : ACHigh_Type<"Quantized", "quant"> {
  let summary = "Quantized tensor type for INT8/INT4 inference";
  let description = [{
    Represents a quantized tensor with scale and zero-point parameters.
    Used for INT8/INT4 quantized inference to reduce memory bandwidth
    and leverage integer compute units.

    Parameters:
    - shape: the tensor dimensions
    - storageType: the quantized storage type (e.g., i8, i4)
    - expressedType: the original floating-point type (e.g., f32)
    - scale: quantization scale factor
    - zeroPoint: quantization zero point

    Example: `!achigh.quant<[1,64,56,56], i8, f32, 0.0235, 128>`
  }];

  let parameters = (ins
    ArrayRefParameter<"int64_t">:$shape,
    "Type":$storageType,
    "Type":$expressedType,
    "double":$scale,
    "int64_t":$zeroPoint
  );

  let assemblyFormat = [{
    `<` $shape `,` $storageType `,` $expressedType `,` $scale `,` $zeroPoint `>`
  }];
}

#endif // ACHIGH_TYPES_TD
