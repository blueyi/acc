"""lit configuration for ACC tests."""

import os
import lit.formats
import lit.util

config.name = "ACC"
# llvm_config is injected when loaded via build/test/lit.site.cfg.py; fallback when run with pip's lit
use_lit_shell = getattr(llvm_config, "use_lit_shell", False) if "llvm_config" in dir() else False
config.test_format = lit.formats.ShTest(not use_lit_shell)
config.suffixes = [".mlir", ".ll", ".ac"]
# input.mlir is expected-output sample, not a RUN test
config.excludes = ["onnx-import-add/input.mlir"]
config.test_source_root = os.path.dirname(__file__)
# acc_obj_root / acc_tools_dir set by site config when using build/test; fallbacks for standalone
if not getattr(config, "acc_obj_root", None):
    config.acc_obj_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "build"))
if not getattr(config, "acc_tools_dir", None):
    config.acc_tools_dir = os.path.join(config.acc_obj_root, "bin")
if not getattr(config, "llvm_tools_dir", None):
    config.llvm_tools_dir = os.environ.get("LLVM_TOOLS_DIR", "")
config.test_exec_root = os.path.join(config.acc_obj_root, "test")

# Add tool substitutions (acc, tools, and FileCheck from LLVM)
tools = ["ac-opt", "ac-translate", "ac-runner", "acc", "FileCheck"]
tool_dirs = [config.acc_tools_dir, config.llvm_tools_dir]
if "llvm_config" in dir():
    llvm_config.add_tool_substitutions(tools, tool_dirs)
else:
    for tool in tools:
        if tool == "FileCheck" and config.llvm_tools_dir:
            config.substitutions.append((tool, os.path.join(config.llvm_tools_dir, tool)))
        else:
            config.substitutions.append((tool, os.path.join(config.acc_tools_dir, tool)))
