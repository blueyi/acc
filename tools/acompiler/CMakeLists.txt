add_llvm_executable(acompiler
  main.cpp
)

target_link_libraries(acompiler PRIVATE
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
