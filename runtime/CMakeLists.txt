add_library(acompilerRuntime STATIC
  cpu/allocator.cpp
  cpu/kernels.cpp
)

target_include_directories(acompilerRuntime
  PUBLIC
    ${PROJECT_SOURCE_DIR}/runtime/include
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(acompilerRuntime PUBLIC cxx_std_17)

add_library(ACompiler::Runtime ALIAS acompilerRuntime)
