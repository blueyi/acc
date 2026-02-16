# Lowering Pipeline（占位）

建议逐步补齐 ACompiler 的分层 lowering 说明：

- `acomp` → `linalg`（或 `tosa`）
- `linalg` → `scf/affine`（循环与调度形态）
- `vector` 向量化路径
- bufferization：tensor → memref
- `memref/scf/vector` → `llvm` dialect
- `llvm` dialect → LLVM IR

并为每一层提供：

- legality 条件
- 典型转换 pattern
- 可调参数（tile size / vector width 等）

