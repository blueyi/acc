add_library(acompilerCore STATIC
  Frontend/AST.cpp
  Frontend/Lexer.cpp
  Frontend/Parser.cpp
  Runtime/Tensor.cpp
  Runtime/Executor.cpp
)

target_include_directories(acompilerCore
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(acompilerCore PUBLIC cxx_std_17)
target_compile_definitions(acompilerCore PUBLIC ACOMPILER_HAS_LLVM=$<BOOL:${ACOMPILER_HAS_LLVM}>)

add_library(ACompiler::Core ALIAS acompilerCore)
