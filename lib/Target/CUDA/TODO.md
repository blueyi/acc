# TODO: CUDA Target（暂不实现）

规划目标：NVIDIA GPU（CUDA）后端支持（仅占位，不在第一阶段实现）。

建议 IR 路线：

- `linalg/vector` → `gpu` dialect → `nvvm` dialect → PTX

需要补齐：

- kernel launch / memory copy / stream 管理
- 算子 legality 与 layout 策略
- 与 runtime 的边界与 ABI 设计

