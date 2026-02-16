add_executable(acompiler-opt
  acompiler-opt.cpp
)

target_link_libraries(acompiler-opt
  PRIVATE
    ACompilerInit
    MLIROptLib
    MLIRInitAllDialects
    MLIRInitAllPasses
)

