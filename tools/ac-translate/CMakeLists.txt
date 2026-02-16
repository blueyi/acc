add_llvm_executable(ac-translate
  main.cpp
)

target_link_libraries(ac-translate PRIVATE
  ACDialectACHigh
  ACDialectACMid
  MLIRIR
  MLIRSupport
  MLIRTranslateLib
  MLIRLLVMToLLVMIRTranslation
  MLIRTargetLLVMIRExport
)
