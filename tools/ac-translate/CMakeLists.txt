add_llvm_executable(ac-translate main.cpp ONNXImportRegistration.cpp)
set_target_properties(ac-translate PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(ac-translate PRIVATE
  ACFrontend
  ACDialectACHigh
  ACDialectACMid
  MLIRIR
  MLIRSupport
  MLIRTranslateLib
  MLIRLLVMToLLVMIRTranslation
  MLIRTargetLLVMIRExport
)
