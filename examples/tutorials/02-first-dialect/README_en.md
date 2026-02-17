# Tutorial 2: Creating Your First MLIR Dialect

## Goal

Learn how to define an MLIR dialect using TableGen/ODS and understand core concepts: Dialect, Operation, Type, Attribute.

## Core Concepts

### Dialect
A dialect in MLIR is a collection of related operations, types, and attributes. Each dialect has a unique namespace.

### Operation
Operations are the basic units in MLIR IR, similar to instructions. Each operation belongs to a dialect.

### Type
Types describe the data type of values, e.g., `tensor<4x4xf32>`.

### Attribute
Attributes are compile-time constant metadata attached to operations.

## Learning Path

1. Read `include/ACC/Dialect/ACHigh/ACHighDialect.td`
2. Understand the meaning of each ODS field
3. Examine the generated C++ code from TableGen
4. Try adding a new operation (e.g., `achigh.sigmoid`)

## Exercises

1. Add a `sigmoid` operation to the ACHigh dialect
2. Add a `batch_norm` operation (with multiple inputs/outputs)
3. Add verification logic (`verifier`) to an operation
