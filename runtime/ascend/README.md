# Ascend Runtime（TODO）

**状态**: 规划中，未实现

## 概述

Ascend Runtime为ACompiler提供华为Ascend NPU后端支持，基于CANN（Compute Architecture for Neural Networks）框架实现高性能AI推理。

## 规划特性

### 1. 基础架构
- [ ] ACL Runtime API封装
- [ ] NPU内存管理（DDR/HBM）
- [ ] 算子调度和执行
- [ ] 多卡支持

### 2. CANN集成
- [ ] ACL（Ascend Computing Language）集成
- [ ] AscendCL Stream管理
- [ ] Graph执行引擎
- [ ] Event同步机制

### 3. 算子支持
- [ ] TBE（Tensor Boost Engine）算子库
- [ ] 内置算子调用
- [ ] 自定义TBE算子
- [ ] 算子融合

### 4. IR生成
- [ ] Ascend IR（AIR）生成
- [ ] 算子映射和转换
- [ ] 图优化
- [ ] 内存优化

## 技术架构

```
应用程序
    ↓
Ascend Runtime API
    ↓
┌──────────────┬───────────────┬──────────────┐
│ 内存管理      │ 算子调度       │ 流管理        │
└──────────────┴───────────────┴──────────────┘
    ↓              ↓               ↓
┌──────────────┬───────────────┬──────────────┐
│ ACL Memory   │ TBE算子库      │ Graph Engine │
└──────────────┴───────────────┴──────────────┘
    ↓
CANN Framework (ACL)
    ↓
Ascend NPU (AI Core)
```

## 编译流程

```
MLIR (AI Dialect)
    ↓
Linalg Dialect
    ↓
Ascend Dialect (自定义)
    ↓
Ascend IR (AIR)
    ↓
OM模型 (Offline Model)
    ↓
NPU执行
```

## CANN框架架构

### ACL层次
- **Application**: 应用层
- **ACL API**: 统一编程接口
- **Runtime**: 运行时管理
- **Driver**: 驱动层

### 关键组件
- **Graph**: 计算图管理
- **Operator**: 算子库
- **Memory**: 内存管理
- **Stream**: 流管理
- **Event**: 事件同步

## 实现计划

### Phase 1: CANN集成（2-3周）
1. CANN SDK环境配置
2. ACL API封装
3. 基础内存管理
4. 设备管理和初始化

### Phase 2: 算子实现（3-4周）
1. 内置算子映射
2. TBE算子调用
3. 自定义算子开发
4. 算子融合支持

### Phase 3: IR生成（2-3周）
1. Ascend Dialect定义
2. MLIR到Ascend IR转换
3. 图优化Pass
4. OM模型生成

### Phase 4: 性能优化（2-3周）
1. 内存优化（DDR/HBM）
2. 多核并行调度
3. 数据流优化
4. 算子自动调优

## Ascend IR示例

### 算子定义（TBE）

```python
# tbe_ops/custom_conv2d.py
from te import tvm
from topi import generic

@tvm.register_func
def custom_conv2d(input_shape, weight_shape, stride, padding):
    """自定义Conv2D TBE算子"""
    # TBE算子实现
    # ...
    return output
```

### ACL调用示例

```cpp
// ascend_runtime.cpp
#include "acl/acl.h"

class AscendRuntime {
public:
    AscendRuntime(int device_id) {
        // 初始化ACL
        aclInit(nullptr);
        aclrtSetDevice(device_id);
        aclrtCreateContext(&context_, device_id);
        aclrtCreateStream(&stream_);
    }
    
    void execute(const Model& model, 
                 std::vector<Tensor*> inputs,
                 std::vector<Tensor*> outputs) {
        // 加载OM模型
        uint32_t model_id;
        aclmdlLoadFromFile(model.path.c_str(), &model_id);
        
        // 创建数据集
        aclmdlDataset* input_dataset = CreateDataset(inputs);
        aclmdlDataset* output_dataset = CreateDataset(outputs);
        
        // 执行推理
        aclmdlExecute(model_id, input_dataset, output_dataset);
        
        // 同步
        aclrtSynchronizeStream(stream_);
    }
    
private:
    aclrtContext context_;
    aclrtStream stream_;
};
```

## Runtime API设计

```c
// Ascend Runtime API

// 初始化
acompiler_runtime_t* acompiler_create_ascend_runtime(int device_id);
void acompiler_destroy_ascend_runtime(acompiler_runtime_t* runtime);

// 内存管理
acompiler_tensor_t* acompiler_ascend_alloc_tensor(
    acompiler_runtime_t* runtime,
    const int* shape, int ndim, 
    acompiler_dtype_t dtype,
    acompiler_mem_type_t mem_type); // DDR or HBM

void acompiler_ascend_free_tensor(acompiler_tensor_t* tensor);

// 数据传输
void acompiler_ascend_memcpy_h2d(void* dst, const void* src, size_t size);
void acompiler_ascend_memcpy_d2h(void* dst, const void* src, size_t size);

// 模型加载
acompiler_model_t* acompiler_ascend_load_model(
    acompiler_runtime_t* runtime,
    const char* om_path);

void acompiler_ascend_unload_model(acompiler_model_t* model);

// 执行
void acompiler_ascend_execute(
    acompiler_runtime_t* runtime,
    acompiler_model_t* model,
    acompiler_tensor_t** inputs, int num_inputs,
    acompiler_tensor_t** outputs, int num_outputs);

// 异步执行
acompiler_stream_t* acompiler_ascend_create_stream(
    acompiler_runtime_t* runtime);

void acompiler_ascend_execute_async(
    acompiler_runtime_t* runtime,
    acompiler_model_t* model,
    acompiler_tensor_t** inputs, int num_inputs,
    acompiler_tensor_t** outputs, int num_outputs,
    acompiler_stream_t* stream);

void acompiler_ascend_synchronize_stream(acompiler_stream_t* stream);
```

## 算子映射

### 卷积算子

```
MLIR (ai.conv2d)
    ↓
Ascend算子: aclnnConvolution
    ↓
TBE Kernel: Conv2D
    ↓
AI Core执行
```

### 矩阵乘法

```
MLIR (ai.matmul)
    ↓
Ascend算子: aclnnMatmul
    ↓
TBE Kernel: BatchMatMul
    ↓
Cube单元执行（矩阵计算单元）
```

## NPU硬件架构

### Ascend 910
- **AI Core**: 矩阵计算、向量计算
- **Cube**: 矩阵计算单元
- **Vector**: 向量计算单元
- **Scalar**: 标量计算单元

### 内存层次
- **HBM**: High Bandwidth Memory（高带宽内存）
- **DDR**: Double Data Rate（板载内存）
- **L2 Cache**: 二级缓存
- **L1 Buffer**: 一级缓存（AI Core内部）

## 优化策略

### 1. 内存优化
- HBM/DDR合理分配
- 减少主机-设备传输
- 内存复用

### 2. 算子优化
- 算子融合（减少中间结果）
- Tile优化（提高缓存命中率）
- 向量化（充分利用Vector单元）

### 3. 并行优化
- 多AI Core并行
- 流水线并行
- 数据并行

### 4. 图优化
- 常量折叠
- 算子重排
- 内存布局优化

## 性能目标

- **卷积**: 充分利用Cube单元，达到理论峰值的60%以上
- **矩阵乘法**: 达到理论TFLOPS的70%以上
- **端到端**: 与MindSpore性能差距在30%以内

## 测试计划

### 功能测试
- [ ] 算子正确性测试
- [ ] 内存管理测试
- [ ] 多卡并行测试
- [ ] 异常处理测试

### 性能测试
- [ ] 单算子性能基准
- [ ] 端到端模型性能
- [ ] 与MindSpore对比
- [ ] 不同NPU型号测试（910/910B）

### 兼容性测试
- [ ] CANN版本兼容性
- [ ] 驱动版本兼容性
- [ ] 固件版本兼容性

## 依赖要求

- **CANN Toolkit**: 5.0+
- **Driver**: 对应版本
- **Firmware**: 对应版本
- **OS**: Ubuntu 18.04/20.04, CentOS 7/8
- **Python**: 3.7+（TBE开发）

## 开发环境

```bash
# 安装CANN Toolkit
wget https://ascend-repo.obs.cn-east-2.myhuaweicloud.com/...
chmod +x Ascend-cann-toolkit_xxx_linux-*.run
./Ascend-cann-toolkit_xxx_linux-*.run --install

# 设置环境变量
source /usr/local/Ascend/ascend-toolkit/set_env.sh

# 验证安装
npu-smi info
```

## 工具链

### ATC（Ascend Tensor Compiler）
模型转换工具，生成OM模型：
```bash
atc --model=model.onnx \
    --framework=5 \
    --output=model \
    --soc_version=Ascend910
```

### MSProf
性能分析工具：
```bash
msprof --application="./app" \
       --output=./profiling_data
```

## 参考资源

- [CANN官方文档](https://www.hiascend.com/document)
- [ACL API参考](https://www.hiascend.com/document/detail/zh/canncommercial/63RC1/apiref/aplref/index.html)
- [TBE开发指南](https://www.hiascend.com/document/detail/zh/canncommercial/63RC1/devguide/tbedevg/tbedevg_000001.html)
- [MindSpore](https://www.mindspore.cn/)（参考实现）

## 注意事项

1. **硬件要求**: 需要Ascend 310/910系列NPU硬件
2. **软件许可**: 需要华为CANN软件许可
3. **技术支持**: 建议联系华为技术支持获取SDK和文档
4. **社区资源**: Ascend社区论坛和GitHub仓库

---

**注意**: 本目录内容为规划文档，实际实现需要Ascend硬件和CANN SDK支持，将在第四阶段进行。如需开发，请先确保具备相应的硬件资源和软件许可。
