# Ascend NPU Runtime — TODO

## Overview

This module will implement Huawei Ascend NPU Runtime support for executing ACC-compiled code on Ascend AI processors.

## Planned Features

- Ascend Device memory management
- AI Core / Vector Core task scheduling
- ACL (Ascend Computing Language) interface wrapper
- CANN toolchain integration
- Custom TBE operator compilation support

## Dependencies

- CANN >= 6.0
- Ascend 910/310 series NPU

## References

- [CANN Official Documentation](https://www.hiascend.com/document)
- [Ascend C Programming Guide](https://www.hiascend.com/document/detail/zh/canncommercial/700/inferapplicationdev/aclcppdevg/aclcppdevg_000004.html)

## Status

**Not yet implemented** — Development will start after GPU backend is mostly complete.
