"""lit configuration for ACompiler tests."""

import os
import lit.formats
import lit.util

config.name = "ACompiler"
config.test_format = lit.formats.ShTest(not llvm_config.use_lit_shell)
config.suffixes = [".mlir", ".ll", ".ac"]
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.acompiler_obj_root, "test")

# Add tool substitutions
tools = ["ac-opt", "ac-translate", "ac-runner", "acompiler"]
tool_dirs = [config.acompiler_tools_dir, config.llvm_tools_dir]
llvm_config.add_tool_substitutions(tools, tool_dirs)
