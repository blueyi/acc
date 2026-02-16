# CI TODO

本仓库当前提供工程骨架与规划文档。

后续建议补齐 CI（例如 GitHub Actions）：

- 构建（需要可用的 LLVM/MLIR 环境，或在 CI 中拉取预构建工具链）
- `lit` 回归测试（`check-acompiler`）
- clang-format / cmake-format（如采用）
- 性能基准的 smoke（可选）

