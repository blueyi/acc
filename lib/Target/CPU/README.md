# CPU Target（第一阶段实现）

本目录用于实现 ACompiler 的 CPU 后端：

- MLIR → LLVM dialect → LLVM IR
- LLVM 优化 pipeline（可配置）
- AOT/JIT（可选）
- 运行时 ABI 与调用封装

