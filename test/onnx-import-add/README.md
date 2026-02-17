# ONNX import add test

This folder contains a minimal ONNX model (single Add op) and the expected ACHigh MLIR for testing `ac-translate --import-onnx`.

- **model.onnx**: Small ONNX graph with one Add node (inputs X, Y; output Z). Included in repo; can be regenerated with the script below.
- **input.mlir**: Example ACHigh MLIR that corresponds to the add graph (expected output shape after ONNX import is implemented).
- **gen_add_model.py**: Script to regenerate `model.onnx`. Run: `python3 gen_add_model.py` (requires `pip install onnx`).

Usage:

```bash
ac-translate --import-onnx model.onnx -o output.mlir
```
