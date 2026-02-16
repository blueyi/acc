# Transform Pass TableGen generation
set(LLVM_TARGET_DEFINITIONS Passes.td)
mlir_tablegen(Passes.h.inc -gen-pass-decls -name ACompiler)
add_public_tablegen_target(ACompilerTransformsPassIncGen)
