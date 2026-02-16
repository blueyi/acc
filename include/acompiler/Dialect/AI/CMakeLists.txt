# AI Dialect TableGen
set(LLVM_TARGET_DEFINITIONS AI.td)

mlir_tablegen(AIDialect.h.inc -gen-dialect-decls -dialect=ai)
mlir_tablegen(AIDialect.cpp.inc -gen-dialect-defs -dialect=ai)
mlir_tablegen(AIOps.h.inc -gen-op-decls)
mlir_tablegen(AIOps.cpp.inc -gen-op-defs)
mlir_tablegen(AITypes.h.inc -gen-typedef-decls)
mlir_tablegen(AITypes.cpp.inc -gen-typedef-defs)
mlir_tablegen(AIAttrs.h.inc -gen-attrdef-decls)
mlir_tablegen(AIAttrs.cpp.inc -gen-attrdef-defs)

add_public_tablegen_target(MLIRAIIncGen)
add_dependencies(mlir-headers MLIRAIIncGen)
