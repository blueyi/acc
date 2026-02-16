# Ascend NPU Runtime — TODO

## 概述

此模块将实现华为 Ascend NPU 的 Runtime 支持，用于在昇腾 AI 处理器上执行 ACompiler 编译后的代码。

## 计划包含

- Ascend Device 内存管理
- AI Core / Vector Core 任务调度
- ACL（Ascend Computing Language）接口封装
- CANN 工具链集成
- 自定义 TBE 算子编译支持

## 依赖

- CANN >= 6.0
- Ascend 910/310 系列 NPU

## 参考

- [CANN 官方文档](https://www.hiascend.com/document)
- [Ascend C 编程指南](https://www.hiascend.com/document/detail/zh/canncommercial/700/inferapplicationdev/aclcppdevg/aclcppdevg_000004.html)

## 状态

**尚未实现** — 等待 GPU 后端基本完成后启动开发。
