add_library(ACompilerInit STATIC
  InitACompiler.cpp
  Dialect/AComp/ACompDialect.cpp
)

target_include_directories(ACompilerInit
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(ACompilerInit
  PUBLIC
    MLIRIR
    MLIRSupport
)

