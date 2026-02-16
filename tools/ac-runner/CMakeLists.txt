add_llvm_executable(ac-runner
  main.cpp
)

target_link_libraries(ac-runner PRIVATE
  ACDialectACHigh
  ACDialectACMid
  ACCodeGen
  ACSupport
  MLIRIR
  MLIRSupport
  MLIRExecutionEngine
  LLVMOrcJIT
)
