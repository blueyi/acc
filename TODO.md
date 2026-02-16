# ACompiler TODO

> 说明：以下为后续迭代待办，当前阶段仅实现 CPU 后端。

## 高优先级（当前阶段）

- [ ] 打通 CPU 端到端编译链路（MLIR -> LLVM IR -> CodeGen -> 运行）
- [ ] 完成基础 MLIR Pass（Shape Inference / Fusion / Canonicalization）
- [ ] 建立 IR 测试与 E2E 测试基线

## 多后端扩展（暂不实现）

- [ ] **NVIDIA GPU CUDA 支持**  
      目标：新增 CUDA 后端 Lowering 与代码生成路径，支持常见 AI 算子在 GPU 上执行。  
      状态：规划中，第一阶段不实现。

- [ ] **Huawei AscendNPU IR 支持**  
      目标：新增 AscendNPU IR 对接层与后端编译路径，支持 NPU 专用算子映射。  
      状态：规划中，第一阶段不实现。
