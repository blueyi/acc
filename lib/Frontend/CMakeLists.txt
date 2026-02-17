add_mlir_library(ACFrontend
  Lexer.cpp
  Token.cpp
  Parser.cpp
  AST.cpp
  Sema.cpp
  MLIRGen.cpp
  ONNXImporter.cpp
  TritonImporter.cpp
  TorchImporter.cpp

  ADDITIONAL_HEADER_DIRS
  ${PROJECT_SOURCE_DIR}/include/ACC/Frontend

  LINK_LIBS PUBLIC
  ACDialectACHigh
  MLIRIR
  MLIRSupport
)
