# 总览

本项目采用两段式流水线：

- **MLIR 段**：从高层 AI 语义（`acomp`）逐步 Lower 到结构化算子（`linalg/tosa`）、循环/内存（`scf/affine/memref`）、向量化（`vector`），最终到 `llvm` dialect。
- **LLVM IR 段**：在 LLVM IR 上完成优化与 codegen，生成 CPU 可执行（AOT 或 JIT）。

详细阶段规划见：`docs/ACompiler_Plan.md`。

