//===- Sema.h - ACompiler Semantic Analysis ----------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_FRONTEND_SEMA_H
#define ACOMPILER_FRONTEND_SEMA_H

#include "ACompiler/Frontend/AST.h"

namespace acompiler {

/// Semantic analyzer: performs type checking, name resolution, and validation.
class Sema {
public:
  /// Analyze and validate a parsed module AST.
  /// Returns true if the module is semantically valid.
  bool analyze(ModuleAST &module);

private:
  bool validateModel(ModelDeclAST &model);
  bool validateLayer(LayerDeclAST &layer);
  bool checkTypes(const TensorType &expected, const TensorType &actual);
};

} // namespace acompiler

#endif // ACOMPILER_FRONTEND_SEMA_H
