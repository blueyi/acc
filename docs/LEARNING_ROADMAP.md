# ACompiler 学习路线图

本文档提供了学习ACompiler和编译器技术的详细路线图。

## 📚 学习阶段概览

```
阶段一：MLIR基础        阶段二：LLVM IR        阶段三：代码生成
  (2-3周)                (2-3周)               (3-4周)
     ↓                      ↓                     ↓
  Dialect定义          LLVM IR语法           指令选择
  Operation定义        Pass开发              寄存器分配
  Pass开发             优化技术              指令调度
  Dialect降级          降级实现              Runtime库
```

## 第一阶段：MLIR基础（2-3周）

### 🎯 学习目标
- 理解MLIR核心概念
- 掌握自定义Dialect开发
- 学会编写优化Pass
- 实现Dialect降级

### 📖 学习内容

#### Week 1: MLIR概念理解

**Day 1-2: MLIR基础**
- [ ] MLIR简介和动机
- [ ] IR结构（Operation, Block, Region）
- [ ] SSA形式
- [ ] Dialect机制
- [ ] 类型系统

**资源**:
- 📄 [docs/tutorials/01_mlir_basics.md](tutorials/01_mlir_basics.md)
- 🔗 [MLIR Language Reference](https://mlir.llvm.org/docs/LangRef/)
- 🎥 MLIR Overview视频

**实践**:
```bash
# 运行MLIR示例
cd examples/mlir
acompiler-opt simple_ops.mlir
```

**Day 3-5: 自定义Dialect**
- [ ] TableGen语法
- [ ] Operation定义
- [ ] Type和Attribute定义
- [ ] Dialect注册

**资源**:
- 📄 [docs/tutorials/02_custom_dialect.md](tutorials/02_custom_dialect.md)
- 🔗 [Defining Dialects](https://mlir.llvm.org/docs/DefiningDialects/)
- 💻 查看`include/acompiler/Dialect/AI/`源码

**实践**:
- 为AI Dialect添加新算子（如Sigmoid, Tanh）
- 编写算子的验证逻辑
- 添加测试用例

**Day 6-7: 复习和实践**
- [ ] 实现一个完整的简单Dialect
- [ ] 编写测试用例
- [ ] 阅读标准Dialect源码（arith, func）

#### Week 2: Pass开发

**Day 8-10: Pattern Rewriting**
- [ ] RewritePattern基础
- [ ] Pattern匹配机制
- [ ] OpRewritePattern
- [ ] GreedyPatternRewriter

**资源**:
- 📄 [docs/tutorials/03_pass_development.md](tutorials/03_pass_development.md)
- 🔗 [Pattern Rewriting](https://mlir.llvm.org/docs/PatternRewriter/)
- 💻 查看`lib/Transforms/FusionPasses/ConvReLUFusion.cpp`

**实践**:
- 实现MatMul+Add融合Pass
- 实现简单的常量折叠Pass

**Day 11-12: Pass Pipeline**
- [ ] Pass管理器
- [ ] Pass依赖关系
- [ ] Pass Pipeline配置
- [ ] Pass统计信息

**实践**:
- 配置多个Pass的执行顺序
- 分析Pass的性能影响

**Day 13-14: 高级Pass技术**
- [ ] 数据流分析
- [ ] 分析Pass vs 变换Pass
- [ ] Pass调试技巧

**实践**:
- 实现形状推导Pass
- 实现死代码消除Pass

#### Week 3: Dialect降级

**Day 15-17: Dialect Conversion**
- [ ] ConversionPattern
- [ ] TypeConverter
- [ ] 降级目标（Target）
- [ ] 合法性检查

**资源**:
- 📄 [docs/tutorials/04_dialect_lowering.md](tutorials/04_dialect_lowering.md)
- 🔗 [Dialect Conversion](https://mlir.llvm.org/docs/DialectConversion/)

**实践**:
- 实现AI Dialect到Linalg的降级
- 测试降级的正确性

**Day 18-20: 完整示例**
- [ ] 实现完整的降级流程：AI → Linalg → Affine → SCF
- [ ] 优化Pass集成
- [ ] 端到端测试

**实践**:
- 编译一个简单的AI模型
- 验证每个降级阶段的IR

### ✅ 阶段评估

完成以下任务以验证学习成果：

1. **Dialect项目**: 实现一个完整的简单Dialect（至少3个操作）
2. **Fusion Pass**: 实现一个算子融合Pass
3. **降级Pass**: 实现从自定义Dialect到标准Dialect的降级
4. **测试覆盖**: 所有代码都有对应的测试用例

---

## 第二阶段：LLVM IR与优化（2-3周）

### 🎯 学习目标
- 理解LLVM IR结构
- 掌握LLVM Pass开发
- 学习优化技术
- 实现MLIR到LLVM IR的降级

### 📖 学习内容

#### Week 4: LLVM IR基础

**Day 21-23: LLVM IR语法**
- [ ] LLVM IR结构（Module, Function, BasicBlock）
- [ ] 指令集
- [ ] 类型系统
- [ ] SSA形式在LLVM中的应用

**资源**:
- 📄 [docs/tutorials/05_llvm_ir_basics.md](tutorials/05_llvm_ir_basics.md)
- 🔗 [LLVM Language Reference](https://llvm.org/docs/LangRef.html)
- 🔗 [LLVM Programmer's Manual](https://llvm.org/docs/ProgrammersManual.html)

**实践**:
```bash
# 手写简单的LLVM IR
clang -S -emit-llvm simple.c -o simple.ll
# 阅读和理解生成的IR
```

**Day 24-26: LLVM Pass框架**
- [ ] Pass类型（FunctionPass, ModulePass等）
- [ ] Pass注册和执行
- [ ] Pass依赖
- [ ] Pass管理器

**资源**:
- 🔗 [Writing an LLVM Pass](https://llvm.org/docs/WritingAnLLVMPass.html)

**实践**:
- 编写简单的FunctionPass（如指令计数）
- 编写ModulePass（如函数统计）

**Day 27-28: 分析框架**
- [ ] DominatorTree
- [ ] LoopInfo
- [ ] AliasAnalysis
- [ ] MemorySSA

#### Week 5: LLVM优化

**Day 29-31: 优化技术**
- [ ] 常量折叠和传播
- [ ] 死代码消除
- [ ] 公共子表达式消除
- [ ] 循环不变式外提

**资源**:
- 📄 [docs/tutorials/06_llvm_optimization.md](tutorials/06_llvm_optimization.md)
- 🔗 [LLVM Optimization Passes](https://llvm.org/docs/Passes.html)

**实践**:
- 使用opt工具运行各种优化Pass
- 观察优化前后IR的变化

**Day 32-34: 循环优化**
- [ ] 循环展开（Loop Unrolling）
- [ ] 循环向量化（Loop Vectorization）
- [ ] 循环融合
- [ ] 多面体优化基础

**实践**:
- 编写可向量化的代码
- 使用Polly进行多面体优化

**Day 35: 复习和实践**

#### Week 6: MLIR到LLVM降级

**Day 36-38: 降级实现**
- [ ] MLIR LLVM Dialect
- [ ] 类型转换（Tensor → MemRef → LLVM Ptr）
- [ ] 控制流转换
- [ ] 函数调用约定

**实践**:
- 实现SCF到CFG的转换
- 实现MemRef到LLVM的降级

**Day 39-41: Runtime接口**
- [ ] C ABI设计
- [ ] Runtime库接口
- [ ] 内存管理
- [ ] 错误处理

**实践**:
- 设计Runtime API
- 实现基础Runtime函数

**Day 42: 端到端测试**
- [ ] MLIR → LLVM IR完整流程
- [ ] 生成可执行代码
- [ ] 正确性验证

### ✅ 阶段评估

完成以下任务：

1. **LLVM Pass**: 实现至少2个有意义的LLVM优化Pass
2. **降级实现**: 完成MLIR到LLVM IR的完整降级
3. **Runtime**: 实现基础Runtime库
4. **端到端**: 编译并运行一个简单的AI模型

---

## 第三阶段：代码生成（3-4周）

### 🎯 学习目标
- 理解代码生成流程
- 掌握指令选择和寄存器分配
- 实现CPU后端
- 开发Runtime库

### 📖 学习内容

#### Week 7: CodeGen基础

**Day 43-47: 代码生成概述**
- [ ] CodeGen流程
- [ ] SelectionDAG
- [ ] 指令选择（Instruction Selection）
- [ ] 合法化（Legalization）

**资源**:
- 📄 [docs/tutorials/07_codegen_basics.md](tutorials/07_codegen_basics.md)
- 🔗 [LLVM Code Generator](https://llvm.org/docs/CodeGenerator.html)
- 🔗 [TableGen Backend](https://llvm.org/docs/TableGen/)

**实践**:
- 研究x86后端的实现
- 使用llc生成汇编代码

#### Week 8: CPU后端

**Day 48-52: x86_64后端**
- [ ] x86架构基础
- [ ] 指令选择模式
- [ ] 寄存器分配
- [ ] 调用约定

**资源**:
- 📄 [docs/tutorials/08_cpu_backend.md](tutorials/08_cpu_backend.md)
- 🔗 [x86 Architecture](https://www.intel.com/content/www/us/en/developer/overview.html)

**实践**:
- 为AI算子选择x86指令
- 实现简单的寄存器分配

**Day 53-56: SIMD优化**
- [ ] SSE/AVX指令集
- [ ] 向量化
- [ ] 数据对齐
- [ ] 性能调优

**实践**:
- 使用SIMD优化矩阵乘法
- 使用SIMD优化卷积

#### Week 9-10: Runtime库

**Day 57-63: Runtime实现**
- [ ] 内存管理
- [ ] 张量操作
- [ ] 算子实现（Conv, MatMul等）
- [ ] 并行执行（OpenMP/TBB）

**实践**:
- 实现完整的CPU Runtime
- 优化算子性能

**Day 64-70: 集成和测试**
- [ ] 端到端集成
- [ ] 性能测试
- [ ] 正确性验证
- [ ] 与参考实现对比

### ✅ 阶段评估

完成以下任务：

1. **CPU后端**: 实现完整的x86_64代码生成
2. **Runtime**: 实现高性能Runtime库
3. **SIMD优化**: 至少优化3个核心算子
4. **端到端**: 编译并高效运行ResNet-18等模型

---

## 📊 学习进度跟踪

使用以下清单跟踪学习进度：

### MLIR基础
- [ ] 完成MLIR基础教程
- [ ] 实现自定义Dialect
- [ ] 实现优化Pass
- [ ] 实现Dialect降级

### LLVM IR
- [ ] 掌握LLVM IR语法
- [ ] 实现LLVM Pass
- [ ] 完成MLIR到LLVM降级
- [ ] 实现Runtime接口

### 代码生成
- [ ] 理解CodeGen流程
- [ ] 实现CPU后端
- [ ] SIMD优化
- [ ] 完整Runtime库

### 项目实践
- [ ] 编译简单模型（单层网络）
- [ ] 编译中等模型（ResNet-18）
- [ ] 性能测试和优化
- [ ] 文档编写

## 🛠️ 实践项目建议

### 初级项目（Week 1-3）
1. **简单Dialect**: 实现算术运算Dialect
2. **Basic Pass**: 实现常量折叠Pass
3. **测试框架**: 建立完整的测试体系

### 中级项目（Week 4-6）
1. **优化Pass集**: 实现多个优化Pass
2. **降级流程**: 完整的多层降级
3. **LLVM集成**: 与LLVM后端集成

### 高级项目（Week 7-10）
1. **完整后端**: x86_64代码生成
2. **高性能Runtime**: 优化的Runtime库
3. **模型编译**: 端到端模型编译
4. **性能分析**: 详细的性能分析和优化

## 📚 推荐阅读

### 书籍
1. "Getting Started with LLVM Core Libraries" - Bruno Cardoso Lopes
2. "Engineering a Compiler" - Keith Cooper
3. "Modern Compiler Implementation in C" - Andrew Appel
4. "Computer Architecture: A Quantitative Approach" - Hennessy & Patterson

### 论文
1. "MLIR: A Compiler Infrastructure for the End of Moore's Law"
2. "Polyhedral Compilation as a Design Pattern for Compilers"
3. "Tensor Comprehensions: Framework-Agnostic High-Performance Machine Learning Abstractions"

### 在线资源
- [LLVM官方文档](https://llvm.org/docs/)
- [MLIR官方文档](https://mlir.llvm.org/)
- [Compiler Explorer](https://godbolt.org/)
- [LLVM Weekly](https://llvmweekly.org/)

## 💡 学习建议

1. **实践为主**: 理论学习后立即动手实践
2. **阅读源码**: 多阅读LLVM/MLIR和其他编译器的源码
3. **循序渐进**: 从简单示例开始，逐步增加复杂度
4. **测试驱动**: 为所有代码编写测试
5. **性能意识**: 始终关注性能优化
6. **社区交流**: 参与LLVM/MLIR社区讨论

## 🎓 进阶方向

完成基础学习后，可以向以下方向深入：

### GPU编译
- [ ] CUDA后端实现
- [ ] PTX代码生成
- [ ] GPU特定优化

### NPU编译
- [ ] Ascend后端实现
- [ ] NPU架构理解
- [ ] 专用指令集

### 高级优化
- [ ] 自动调优（Auto-tuning）
- [ ] 图级优化
- [ ] 量化感知编译
- [ ] 动态Shape处理

### 工具开发
- [ ] 可视化工具
- [ ] 调试器
- [ ] 性能分析器
- [ ] IDE集成

---

**祝学习愉快！如有问题，请在GitHub Issues中讨论。**
