add_llvm_executable(acc main.cpp)
set_target_properties(acc PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(acc PRIVATE
  ACFrontend
  ACDialectACHigh
  ACDialectACMid
  ACTransforms
  ACConversionACHighToACMid
  ACConversionACMidToLinalg
  ACConversionACMidToLLVM
  ACIROptimizer
  ACCodeGen
  ACSupport
  MLIRIR
  MLIRPass
  MLIRSupport
  LLVMSupport
)
