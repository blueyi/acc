import os

import lit.formats
import lit.util

config.name = "ACompiler"
config.test_format = lit.formats.ShTest(execute_external=True)

config.suffixes = [".mlir"]
config.excludes = ["CMakeLists.txt", "lit.cfg.py", "lit.site.cfg.py.in"]

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.acompiler_obj_root, "test")

acompiler_opt = lit.util.which("acompiler-opt", config.acompiler_tools_dir)
if acompiler_opt is None:
  acompiler_opt = os.path.join(config.acompiler_tools_dir, "acompiler-opt")
config.substitutions.append(("%acompiler_opt", acompiler_opt))

