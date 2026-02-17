# ACompiler 项目实施检查清单

本文档提供完整的项目实施检查清单，用于追踪项目进度。

---

## 📋 总体进度

- [ ] **Phase 1**: MLIR基础与架构 (Week 1-4)
- [ ] **Phase 2**: LLVM IR优化 (Week 5-7)
- [ ] **Phase 3**: 代码生成与Runtime (Week 8-11)
- [ ] **Phase 4**: 多后端扩展 (Week 12+, 规划中)

当前阶段: **Phase 1** | 完成度: **30%**

---

## 第一阶段：MLIR基础（Week 1-4）

### Week 1: 项目工程搭建

#### 环境配置
- [ ] 安装LLVM/MLIR 17.0+
  - [ ] 下载源码
  - [ ] 配置CMake
  - [ ] 编译LLVM/MLIR (约30-60分钟)
  - [ ] 验证安装 (`mlir-opt --version`)
- [ ] 配置ACompiler构建系统
  - [ ] 设置LLVM_DIR和MLIR_DIR
  - [ ] 配置CMakeLists.txt
  - [ ] 首次编译成功
  - [ ] 生成compile_commands.json
- [ ] IDE配置
  - [ ] VS Code/CLion配置
  - [ ] clangd LSP配置
  - [ ] 代码跳转和补全工作

#### 测试框架
- [ ] 集成GoogleTest
  - [ ] FetchContent配置
  - [ ] 示例测试编译
  - [ ] `ninja check-acompiler`运行成功
- [ ] FileCheck集成
  - [ ] lit测试配置
  - [ ] 第一个.mlir测试通过

#### CI/CD（可选）
- [ ] GitHub Actions配置
  - [ ] 自动编译工作流
  - [ ] 自动测试工作流
  - [ ] 代码覆盖率报告

**验收标准**:
- ✅ 项目可以成功编译
- ✅ 至少一个测试通过
- ✅ IDE代码补全工作

---

### Week 2: MLIR Dialect开发

#### AI Dialect基础
- [ ] Dialect定义
  - [x] AI.td基础结构
  - [x] Dialect命名空间
  - [ ] Dialect注册和初始化
- [ ] 核心算子实现 (目标: 15+)
  - [x] Conv2D操作
  - [x] MatMul操作  
  - [x] ReLU操作
  - [x] MaxPool2D操作
  - [x] BatchNorm操作
  - [ ] AvgPool2D操作
  - [ ] GlobalAvgPool操作
  - [ ] Add/Mul操作
  - [ ] LayerNorm操作
  - [ ] GELU激活
  - [ ] Sigmoid激活
  - [ ] Tanh激活
  - [ ] Reshape操作
  - [ ] Transpose操作
  - [ ] Softmax操作

#### 验证逻辑
- [ ] 所有算子有verify()实现
  - [ ] 输入维度检查
  - [ ] 属性合法性检查
  - [ ] 类型兼容性检查
- [ ] 错误消息清晰易懂
- [ ] 边界条件测试

#### 类型系统
- [ ] 自定义张量类型
  - [ ] TensorType定义
  - [ ] 形状推导支持
  - [ ] 动态形状支持
- [ ] 量化类型（可选）
  - [ ] QuantizedType定义
  - [ ] INT8类型支持

#### 形状推导
- [ ] InferTypeOpInterface实现
  - [ ] Conv2D形状推导
  - [ ] MatMul形状推导
  - [ ] Pool形状推导
- [ ] 形状推导测试通过

**验收标准**:
- ✅ 至少15个算子定义完整
- ✅ 所有算子通过verify测试
- ✅ 形状推导工作正常
- ✅ 测试覆盖率 >70%

---

### Week 3: MLIR Pass开发

#### 融合Pass
- [ ] ConvReLU融合
  - [x] Pattern定义
  - [x] 匹配逻辑
  - [ ] 重写逻辑
  - [ ] FileCheck测试
- [ ] ConvBatchNormReLU融合
  - [ ] Pattern定义
  - [ ] BatchNorm参数折叠
  - [ ] 测试验证
- [ ] MatMulAdd融合（GEMM）
  - [ ] Pattern定义
  - [ ] GEMM操作定义
  - [ ] 测试
- [ ] 通用融合框架
  - [ ] BinaryFusionPattern模板
  - [ ] 可复用的融合工具

#### 优化Pass
- [ ] 常量折叠
  - [x] Pass框架
  - [ ] AddOp折叠
  - [ ] MulOp折叠
  - [ ] 其他算子折叠
- [ ] 死代码消除
  - [ ] 未使用值检测
  - [ ] 删除逻辑
  - [ ] 测试
- [ ] 公共子表达式消除（CSE）
  - [ ] 表达式哈希
  - [ ] 重复检测
  - [ ] 替换逻辑

#### Pass Pipeline
- [ ] Pass注册
  - [ ] Passes.td定义
  - [ ] Pass构造函数
- [ ] Pass依赖关系
  - [ ] 依赖声明
  - [ ] 执行顺序
- [ ] Pipeline配置
  - [ ] 预定义Pipeline
  - [ ] 命令行选项

**验收标准**:
- ✅ 至少5个Pass实现
- ✅ 所有Pass有FileCheck测试
- ✅ Pass可以正确组合使用
- ✅ 性能有可测量的提升

---

### Week 4: ONNX前端

#### 导入器框架
- [ ] ONNXImporter类
  - [ ] 模型加载
  - [ ] 图遍历
  - [ ] 符号表管理
- [ ] 类型转换
  - [ ] ONNX类型 -> MLIR类型
  - [ ] 形状推导
- [ ] 属性提取
  - [ ] 整数数组属性
  - [ ] 浮点属性
  - [ ] 字符串属性

#### 算子转换 (目标: 20+)
- [ ] 卷积类
  - [ ] Conv
  - [ ] ConvTranspose
- [ ] 线性
  - [ ] MatMul
  - [ ] Gemm
- [ ] 激活
  - [ ] Relu, Sigmoid, Tanh
  - [ ] Gelu, Silu
- [ ] 池化
  - [ ] MaxPool, AvgPool
  - [ ] GlobalAveragePool
- [ ] 归一化
  - [ ] BatchNormalization
  - [ ] LayerNormalization
- [ ] Element-wise
  - [ ] Add, Sub, Mul, Div
- [ ] Reduce
  - [ ] ReduceMean, ReduceSum
- [ ] Shape
  - [ ] Reshape, Transpose
  - [ ] Flatten, Squeeze, Unsqueeze

#### 工具集成
- [ ] acompiler-translate工具
  - [ ] 命令行参数解析
  - [ ] ONNX导入选项
  - [ ] 输出格式选择
- [ ] 错误处理
  - [ ] 不支持算子警告
  - [ ] 转换失败报告
  - [ ] 调试信息

#### 模型测试
- [ ] 简单模型
  - [ ] 单层网络
  - [ ] MLP
- [ ] CNN模型
  - [ ] ResNet-18
  - [ ] MobileNetV2
- [ ] Transformer模型
  - [ ] BERT-tiny（可选）

**验收标准**:
- ✅ 支持20+个ONNX算子
- ✅ ResNet-18成功导入
- ✅ 导入的MLIR通过验证
- ✅ 可以查看完整的IR

---

## 第二阶段：LLVM IR优化（Week 5-7）

### Week 5: MLIR到LLVM降级

#### 多层降级
- [ ] AIToLinalg
  - [ ] Conv2D降级
  - [ ] MatMul降级
  - [ ] Pool降级
  - [ ] Element-wise降级
- [ ] LinalgToLoops/Affine
  - [ ] 循环生成
  - [ ] 边界条件
- [ ] LoopsToSCF
  - [ ] 结构化控制流
- [ ] SCFToCF
  - [ ] 控制流图
- [ ] CFToLLVM
  - [ ] LLVM Dialect生成

#### Runtime接口
- [ ] C API设计
  - [ ] Tensor管理
  - [ ] Runtime创建/销毁
  - [ ] 模型执行
- [ ] LLVM IR生成
  - [ ] 函数声明
  - [ ] 调用指令
  - [ ] ABI兼容性

#### 测试验证
- [ ] 每层降级有测试
- [ ] 端到端降级测试
- [ ] 生成的LLVM IR可编译

**验收标准**:
- ✅ 完整降级流程工作
- ✅ 生成的IR通过`llc`编译
- ✅ Runtime API文档完整

---

### Week 6: LLVM优化

#### 循环优化
- [ ] Loop Unrolling
  - [ ] 展开因子配置
  - [ ] 部分展开
- [ ] Loop Vectorization
  - [ ] SIMD指令生成
  - [ ] 向量宽度选择
- [ ] Loop Fusion
  - [ ] 相邻循环融合
  - [ ] 依赖分析

#### 内存优化
- [ ] Linalg Tiling
  - [ ] Tile size配置
  - [ ] 多级Tiling
- [ ] Buffer优化
  - [ ] 临时buffer消除
  - [ ] Buffer复用

#### 优化Pipeline
- [ ] O0: 无优化
- [ ] O1: 基础优化
- [ ] O2: 标准优化
- [ ] O3: 激进优化

**验收标准**:
- ✅ 集成10+个LLVM Pass
- ✅ 向量化成功应用
- ✅ 优化后性能提升30%+

---

### Week 7: 指令映射

#### SIMD支持
- [ ] SSE指令
  - [ ] 单精度浮点
  - [ ] 128-bit向量
- [ ] AVX/AVX2指令
  - [ ] 256-bit向量
  - [ ] FMA指令
- [ ] AVX-512（可选）
  - [ ] 512-bit向量

#### 代价模型
- [ ] 指令延迟表
- [ ] 吞吐量模型
- [ ] 选择策略

**验收标准**:
- ✅ 卷积/矩阵乘使用SIMD
- ✅ 汇编包含AVX指令
- ✅ SIMD带来2-4x加速

---

## 第三阶段：代码生成（Week 8-11）

### Week 8-9: CPU后端

#### 编译流程
- [ ] CompilerDriver实现
  - [ ] 命令行工具
  - [ ] Pipeline配置
  - [ ] 错误处理
- [ ] 代码生成
  - [ ] LLVM IR -> 目标代码
  - [ ] 对象文件生成
  - [ ] 共享库生成

#### Runtime库
- [ ] 基础结构
  - [ ] Runtime类
  - [ ] Tensor类
  - [ ] 内存池
- [ ] 线程池
  - [ ] OpenMP集成
  - [ ] 或自定义线程池

**验收标准**:
- ✅ 端到端编译成功
- ✅ 生成的.so可加载
- ✅ Runtime库可用

---

### Week 10-11: 性能优化

#### 算子优化
- [ ] Conv2D
  - [ ] im2col + GEMM
  - [ ] Winograd（可选）
  - [ ] Direct卷积
- [ ] MatMul
  - [ ] Blocking/Tiling
  - [ ] SIMD优化
  - [ ] BLAS集成（可选）

#### 性能测试
- [ ] 基准测试框架
  - [ ] 时间测量
  - [ ] GFLOPS计算
  - [ ] 统计分析
- [ ] 单算子基准
  - [ ] Conv2D各种配置
  - [ ] MatMul各种大小
- [ ] 端到端基准
  - [ ] ResNet-18
  - [ ] MobileNetV2
- [ ] 与参考对比
  - [ ] ONNX Runtime
  - [ ] MKL（可选）

#### Profiling
- [ ] Linux perf
  - [ ] CPU cycles
  - [ ] Cache misses
  - [ ] 热点函数
- [ ] Intel VTune（可选）
  - [ ] 微架构分析
  - [ ] 内存访问模式

**验收标准**:
- ✅ Conv2D达到目标性能（60%+参考）
- ✅ ResNet-18推理成功
- ✅ 性能分析报告完整
- ✅ 至少优化3个瓶颈

---

## 第四阶段：多后端（Week 12+）

### CUDA后端（TODO）
- [ ] GPU Dialect集成
- [ ] Kernel生成
- [ ] Shared Memory优化
- [ ] Tensor Core支持
- [ ] cuDNN集成

### Ascend后端（TODO）
- [ ] CANN SDK集成
- [ ] Ascend Dialect
- [ ] TBE算子映射
- [ ] OM模型生成

**当前状态**: 规划中，未开始实施

---

## 📊 质量指标

### 代码质量
- [ ] 编译警告数: 0
- [ ] Clang-tidy警告: <10
- [ ] 代码覆盖率: >80%
- [ ] 文档覆盖率: 100%（公共API）

### 性能指标
- [ ] Conv2D: >60% ONNX Runtime
- [ ] MatMul: >65% MKL/OpenBLAS
- [ ] ResNet-18: >65% ONNX Runtime
- [ ] 编译时间: <30s (ResNet-18)

### 测试指标
- [ ] 单元测试: >100个
- [ ] 集成测试: >20个
- [ ] 端到端测试: >5个
- [ ] 所有测试通过率: 100%

---

## 📝 文档完成度

### 技术文档
- [x] PROJECT_PLAN.md
- [x] DIRECTORY_STRUCTURE.md
- [x] IMPLEMENTATION_GUIDE.md
- [x] CHECKLIST.md (本文件)
- [ ] API_REFERENCE.md
- [x] LEARNING_ROADMAP.md

### 教程文档
- [x] 01_mlir_basics.md
- [x] 02_custom_dialect.md
- [ ] 03_pass_development.md
- [ ] 04_dialect_lowering.md
- [ ] 05_llvm_ir_basics.md
- [ ] 06_llvm_optimization.md
- [ ] 07_codegen_basics.md
- [ ] 08_cpu_backend.md

### 设计文档
- [x] architecture.md
- [ ] pass_pipeline.md
- [ ] runtime_design.md
- [ ] multi_backend.md

### 用户文档
- [x] README.md
- [ ] QUICKSTART.md
- [ ] FAQ.md
- [ ] TROUBLESHOOTING.md

---

## 🎯 里程碑追踪

| 里程碑 | 目标日期 | 状态 | 完成度 |
|--------|---------|------|--------|
| M1: 项目搭建 | Week 1 | 🚧 进行中 | 80% |
| M2: MLIR核心 | Week 4 | 📅 计划中 | 30% |
| M3: LLVM集成 | Week 7 | 📅 计划中 | 10% |
| M4: CPU代码生成 | Week 10 | 📅 计划中 | 5% |
| M5: 文档完善 | Week 12 | 📅 计划中 | 40% |

---

## 💡 使用方法

1. **追踪进度**: 定期更新检查清单
2. **识别瓶颈**: 标记卡住的任务
3. **调整计划**: 根据实际进度调整
4. **庆祝里程碑**: 完成阶段后review

**更新频率**: 建议每周更新一次

---

**最后更新**: 2026-02-16  
**当前阶段**: Phase 1 - Week 1  
**下一个里程碑**: M1 - 项目基础搭建

