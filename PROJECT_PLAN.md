# ACompiler 项目规划

## 项目概述

ACompiler是一个基于LLVM的AI编译器项目，旨在通过实例实践的方式快速学习和掌握编译器知识。项目采用自上而下、由浅入深的学习路线，从MLIR高层抽象开始，逐步深入到LLVM IR优化和代码生成。

### 项目目标

1. **学习目标**: 通过实践掌握现代编译器技术，特别是LLVM/MLIR框架
2. **技术目标**: 实现一个支持多后端的AI模型编译器
3. **工程目标**: 建立完整的编译器工程体系，包括前端、优化、后端等模块

## 项目分期规划

### 第一阶段：基础架构与MLIR入门（当前阶段）

#### 1.1 项目工程搭建
- [x] 创建项目目录结构
- [ ] 配置CMake构建系统
- [ ] 集成LLVM/MLIR依赖
- [ ] 建立单元测试框架
- [ ] 配置CI/CD流程

#### 1.2 MLIR基础概念实践
- [ ] **Dialect定义**: 创建自定义AI Dialect
  - [ ] 定义基本的张量操作（TensorOps）
  - [ ] 定义常见的AI算子（Conv2D, MatMul, ReLU等）
  - [ ] 实现Operation定义和验证
- [ ] **Type System**: 实现类型系统
  - [ ] 张量类型（TensorType）
  - [ ] 数据类型（Float32, Int8等）
  - [ ] 形状推导
- [ ] **Attribute系统**: 属性定义和使用
  - [ ] 常量属性
  - [ ] 配置属性（stride, padding等）

#### 1.3 MLIR Pass开发
- [ ] **转换Pass（Transformation Passes）**
  - [ ] 算子融合Pass（如Conv+ReLU融合）
  - [ ] 常量折叠Pass
  - [ ] 死代码消除Pass
- [ ] **降级Pass（Lowering Passes）**
  - [ ] AI Dialect -> Linalg降级
  - [ ] Linalg -> Affine降级
  - [ ] Affine -> SCF降级
- [ ] **分析Pass（Analysis Passes）**
  - [ ] 形状分析
  - [ ] 内存使用分析
  - [ ] 计算图可视化

#### 1.4 前端支持
- [ ] **模型导入器**
  - [ ] ONNX模型导入
  - [ ] 模型解析为MLIR表示
  - [ ] 类型推导和验证
- [ ] **IR构建器**
  - [ ] Builder API设计
  - [ ] 图构建工具

### 第二阶段：LLVM IR优化

#### 2.1 MLIR到LLVM IR降级
- [ ] **降级流程**
  - [ ] SCF -> CFG转换
  - [ ] MemRef -> LLVM降级
  - [ ] 函数调用约定转换
- [ ] **ABI适配**
  - [ ] C语言接口生成
  - [ ] Runtime库接口

#### 2.2 LLVM优化Pass
- [ ] **通用优化**
  - [ ] 循环优化（Loop Unrolling, Vectorization）
  - [ ] 内联优化
  - [ ] 内存访问优化
- [ ] **AI特定优化**
  - [ ] 张量内存布局优化
  - [ ] 缓存局部性优化
  - [ ] 计算密集型循环优化

#### 2.3 指令选择与映射
- [ ] **指令映射**
  - [ ] 标准算术指令映射
  - [ ] SIMD指令映射（SSE, AVX）
  - [ ] 内存操作指令优化
- [ ] **代价模型**
  - [ ] 指令延迟建模
  - [ ] 吞吐量分析

### 第三阶段：代码生成（CodeGen）

#### 3.1 CPU后端实现
- [ ] **x86_64后端**
  - [ ] 指令选择（Instruction Selection）
  - [ ] 寄存器分配（Register Allocation）
  - [ ] 指令调度（Instruction Scheduling）
  - [ ] 代码发射（Code Emission）
- [ ] **ARM后端（可选）**
  - [ ] NEON SIMD支持
  - [ ] ARM64优化

#### 3.2 Runtime库
- [ ] **基础Runtime**
  - [ ] 内存管理（张量分配/释放）
  - [ ] 数学库函数
  - [ ] 并行执行支持（OpenMP/TBB）
- [ ] **性能工具**
  - [ ] Profiling接口
  - [ ] 调试工具

#### 3.3 目标文件生成
- [ ] **输出格式**
  - [ ] 目标代码生成（Object File）
  - [ ] 共享库生成（.so/.dll）
  - [ ] 静态库支持

### 第四阶段：多后端扩展（TODO）

#### 4.1 NVIDIA GPU CUDA支持（未实现）
- [ ] **CUDA后端规划**
  - [ ] PTX代码生成
  - [ ] Kernel启动代码生成
  - [ ] 内存传输优化（Host-Device）
  - [ ] Shared Memory优化
  - [ ] Warp级优化
  - [ ] Tensor Core支持
- [ ] **CUDA特定Pass**
  - [ ] Grid/Block配置优化
  - [ ] 线程合并优化
  - [ ] Bank Conflict消除

#### 4.2 华为Ascend NPU支持（未实现）
- [ ] **Ascend后端规划**
  - [ ] CANN框架集成
  - [ ] Ascend IR生成
  - [ ] 算子映射到Ascend内置算子
  - [ ] TBE自定义算子支持
  - [ ] 内存管理（DDR/HBM）
- [ ] **NPU特定优化**
  - [ ] AI Core利用率优化
  - [ ] 数据流优化
  - [ ] 多核调度

## 技术栈

### 核心依赖
- **LLVM/MLIR**: 17.0+
- **C++标准**: C++17
- **构建系统**: CMake 3.20+
- **编译器**: GCC 9+ / Clang 12+

### 可选依赖
- **ONNX**: 模型导入
- **OpenMP/TBB**: 并行执行
- **GoogleTest**: 单元测试
- **LLVM FileCheck**: 集成测试

## 学习路线

### 阶段一：MLIR基础（建议学习时长：2-3周）
1. **第1-2天**: MLIR概念理解
   - IR结构（Operation, Block, Region）
   - SSA形式
   - Dialect机制
2. **第3-5天**: 自定义Dialect
   - TableGen语法
   - Operation定义
   - Type和Attribute定义
3. **第6-10天**: Pass开发
   - Pattern Rewriting
   - Dialect Conversion
   - Pass Pipeline配置
4. **第11-15天**: 完整示例
   - 实现简单的AI算子Dialect
   - 实现基本的优化Pass
   - 降级到标准Dialect

### 阶段二：LLVM IR（建议学习时长：2-3周）
1. **第1-3天**: LLVM IR理解
   - IR语法
   - 指令集
   - 模块、函数、基本块
2. **第4-7天**: LLVM Pass
   - FunctionPass
   - ModulePass
   - 数据流分析
3. **第8-12天**: 优化技术
   - SSA优化
   - 循环优化
   - 内存优化
4. **第13-15天**: MLIR到LLVM降级
   - 降级流程实现
   - Runtime接口设计

### 阶段三：代码生成（建议学习时长：3-4周）
1. **第1-5天**: CodeGen基础
   - SelectionDAG
   - 指令选择
   - 寄存器分配
2. **第6-10天**: CPU后端
   - x86_64 Target
   - SIMD优化
   - 调用约定
3. **第11-15天**: Runtime实现
   - 内存管理
   - 并行执行
   - 库函数
4. **第16-20天**: 集成测试
   - 端到端测试
   - 性能测试
   - 调试工具

## 项目里程碑

### Milestone 1: 项目基础搭建（Week 1）
- 完成项目目录结构
- 完成CMake构建系统
- 完成LLVM/MLIR集成
- 完成Hello World示例

### Milestone 2: MLIR核心功能（Week 2-4）
- 完成AI Dialect定义
- 完成基础Pass实现
- 完成到Linalg的降级
- 完成单元测试覆盖

### Milestone 3: LLVM IR集成（Week 5-7）
- 完成到LLVM IR的完整降级
- 完成LLVM优化Pass集成
- 完成Runtime库基础版本
- 完成简单模型编译

### Milestone 4: CPU代码生成（Week 8-10）
- 完成x86_64代码生成
- 完成SIMD优化
- 完成性能测试框架
- 完成示例模型端到端运行

### Milestone 5: 文档与示例（Week 11-12）
- 完成技术文档
- 完成教程和示例
- 完成API文档
- 完成用户指南

## 测试策略

### 单元测试
- MLIR Dialect验证测试
- Pass功能测试
- 降级转换测试
- Runtime函数测试

### 集成测试
- MLIR -> LLVM IR端到端测试
- 编译流程测试
- 使用LLVM FileCheck验证IR输出

### 性能测试
- 常见AI算子性能基准
- 与参考实现对比（如ONNX Runtime）
- 性能回归测试

### 正确性测试
- 数值精度测试
- 模型推理正确性验证
- 边界条件测试

## 文档体系

### 开发文档
- 架构设计文档
- API参考文档
- Pass开发指南
- 后端扩展指南

### 学习文档
- MLIR入门教程
- LLVM IR基础
- 优化Pass编写
- 代码生成原理

### 用户文档
- 快速开始指南
- 编译器使用手册
- 常见问题FAQ
- 故障排查指南

## 项目输出

### 代码产出
- ACompiler编译器工具链
- Runtime运行时库
- 示例代码和测试用例
- 开发工具和脚本

### 文档产出
- 完整技术文档
- 学习教程
- API文档
- 设计文档

### 知识积累
- 编译器技术理解
- LLVM/MLIR实践经验
- AI编译优化技术
- 工程实践能力

## 扩展规划

### 功能扩展
- 更多AI框架前端支持（TensorFlow, PyTorch）
- 量化支持（INT8, Mixed Precision）
- 动态Shape支持
- 更多后端（GPU, NPU, DSA）

### 优化扩展
- Auto-tuning支持
- 图级优化
- 算子库自动生成
- 编译缓存机制

### 工具扩展
- 可视化工具
- 性能分析器
- 调试器增强
- IDE集成

## 参考资源

### 官方文档
- LLVM官方文档: https://llvm.org/docs/
- MLIR官方文档: https://mlir.llvm.org/
- LLVM Language Reference: https://llvm.org/docs/LangRef.html

### 学习资源
- MLIR Toy Tutorial
- LLVM Kaleidoscope Tutorial
- "Getting Started with LLVM Core Libraries"
- "MLIR: A Compiler Infrastructure for the End of Moore's Law"

### 开源项目参考
- TVM: AI编译器参考
- XLA: TensorFlow编译器
- IREE: MLIR E2E编译器
- Torch-MLIR: PyTorch前端

---

*最后更新: 2026-02-16*
