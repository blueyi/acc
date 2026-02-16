add_llvm_executable(ac-opt
  main.cpp
)

target_link_libraries(ac-opt PRIVATE
  ACDialectACHigh
  ACDialectACMid
  ACTransforms
  ACConversionACHighToACMid
  ACConversionACMidToLinalg
  ACConversionACMidToLLVM
  MLIROptLib
  MLIRIR
  MLIRPass
  MLIRSupport
)
