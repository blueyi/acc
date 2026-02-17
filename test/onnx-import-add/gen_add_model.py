#!/usr/bin/env python3
"""Generate a minimal ONNX model with a single Add node. Requires: pip install onnx"""

from onnx import TensorProto
from onnx.helper import make_model, make_node, make_graph, make_tensor_value_info
from onnx.checker import check_model
import os

# Shape matching test/Dialect/ACHigh/ops.mlir add example
SHAPE = [4, 16]

def main():
    X = make_tensor_value_info("X", TensorProto.FLOAT, SHAPE)
    Y = make_tensor_value_info("Y", TensorProto.FLOAT, SHAPE)
    Z = make_tensor_value_info("Z", TensorProto.FLOAT, SHAPE)

    add_node = make_node("Add", ["X", "Y"], ["Z"])
    graph = make_graph([add_node], "add_model", [X, Y], [Z])
    model = make_model(graph, producer_name="acc-test")
    check_model(model)

    out_path = os.path.join(os.path.dirname(__file__), "model.onnx")
    with open(out_path, "wb") as f:
        f.write(model.SerializeToString())
    print("Wrote", out_path)

if __name__ == "__main__":
    main()
