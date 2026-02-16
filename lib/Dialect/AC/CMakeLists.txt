# AC Dialect - ACompiler 自定义 MLIR Dialect
add_library(ACDialect
  ACDialect.cpp
)
target_include_directories(ACDialect
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ACDialect
  PUBLIC MLIRIR MLIRSupport
)
