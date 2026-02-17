add_llvm_executable(ac-runner main.cpp)
set_target_properties(ac-runner PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

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
