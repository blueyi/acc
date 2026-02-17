// RUN: ac-translate --import-onnx %S/../onnx-import-add/model.onnx 2>&1 | FileCheck %s
// Verify --import-onnx is accepted (no "Unknown command line argument").
// When ONNX import is implemented, this test may be extended to check output MLIR.

// CHECK-NOT: Unknown command line argument
