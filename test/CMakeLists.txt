# ACompiler 测试
# 第一阶段：使用 CTest 运行简单测试
# 后续可接入 LIT 完整测试框架

# 基本测试：验证工具可执行
add_test(NAME acompiler-opt-help
  COMMAND $<TARGET_FILE:acompiler-opt> --help)
set_tests_properties(acompiler-opt-help PROPERTIES
  LABELS "acompiler;tools")

add_test(NAME acompiler-compile-help
  COMMAND $<TARGET_FILE:acompiler-compile> --help)
set_tests_properties(acompiler-compile-help PROPERTIES
  LABELS "acompiler;tools")
