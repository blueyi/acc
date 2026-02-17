# Tutorial 3: Defining AI Operators

## Goal

Deep dive into MLIR Operation definitions, including Traits, Interfaces, Assembly Format, and custom verifiers.

## Key Topics

1. **Traits**: `Pure`, `SameOperandsAndResultType`, `Commutative`, etc.
2. **Interfaces**: `InferTypeOpInterface`, `SideEffectInterfaces`
3. **Assembly Format**: Declarative IR print/parse format
4. **Custom Verifier**: Custom verification logic

## Exercises

1. Define an `achigh.batch_norm` operation with 5 input arguments
2. Add `dilations` attribute to `achigh.conv2d`
3. Implement a custom `verifier` to validate input shape legality
