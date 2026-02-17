add_llvm_library(ACSupport
  Logger.cpp
  Timer.cpp
  ErrorHandling.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/Support

  LINK_LIBS
  LLVMSupport
)
