# acompiler-opt - MLIR 优化工具
set(LLVM_LINK_COMPONENTS Support)

add_executable(acompiler-opt ACompilerOpt.cpp)
target_link_libraries(acompiler-opt
  PRIVATE
  ACDialect
  MLIRIR
  MLIRParser
  MLIRSupport
  LLVMSupport
)

# 安装
install(TARGETS acompiler-opt
  RUNTIME DESTINATION bin
)
