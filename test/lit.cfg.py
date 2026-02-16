# LIT 测试配置文件
# 用于 ACompiler 的 FileCheck 测试
# 需在构建时通过 configure_lit_site_cfg 生成完整配置
#
# 使用方式：
#   cd build && ninja check-acompiler
# 或：lit -v test/
#
# 环境变量 ACOMPILER_OPT / ACOMPILER_COMPILE 可指定工具路径

import os
import lit.formats

config.name = 'ACompiler'
config.test_format = lit.formats.ShTest(execute_external=True)
config.suffixes = ['.mlir', '.ll', '.td', '.py']
config.test_source_root = os.path.dirname(__file__)

# 工具路径（由 CMake 或环境变量提供）
config.acompiler_opt = os.environ.get('ACOMPILER_OPT', 'acompiler-opt')
config.acompiler_compile = os.environ.get('ACOMPILER_COMPILE', 'acompiler-compile')
config.substitutions.append(('%acompiler-opt', config.acompiler_opt))
config.substitutions.append(('%acompiler-compile', config.acompiler_compile))
