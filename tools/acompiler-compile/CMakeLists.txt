# acompiler-compile - 端到端编译工具
set(LLVM_LINK_COMPONENTS Support)

add_executable(acompiler-compile ACompilerCompile.cpp)
target_link_libraries(acompiler-compile
  PRIVATE
  ACDialect
  MLIRIR
  MLIRSupport
  LLVMSupport
)

install(TARGETS acompiler-compile
  RUNTIME DESTINATION bin
)
