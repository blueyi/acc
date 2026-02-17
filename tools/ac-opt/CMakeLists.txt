add_llvm_executable(ac-opt main.cpp)
set_target_properties(ac-opt PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(ac-opt PRIVATE
  ACDialectACHigh
  ACDialectACMid
  ACTransforms
  ACConversionACHighToACMid
  ACConversionACMidToLinalg
  ACConversionACMidToLLVM
  MLIRFuncDialect
  MLIROptLib
  MLIRIR
  MLIRPass
  MLIRSupport
)
