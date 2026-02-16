# third_party

用于放置外部依赖的占位目录。

建议策略（二选一）：

- 使用 CMake `FetchContent` 拉取少量小依赖
- 使用 git submodule（适合较大依赖）

说明：LLVM/MLIR 通常不作为 `third_party` vendoring，建议通过系统安装或单独构建后用 `LLVM_DIR/MLIR_DIR` 引入。

