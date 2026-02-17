add_llvm_executable(acc
  main.cpp
)

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
