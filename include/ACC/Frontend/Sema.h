//===- Sema.h - ACC Semantic Analysis ----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_FRONTEND_SEMA_H
#define ACC_FRONTEND_SEMA_H

#include "ACC/Frontend/AST.h"

namespace acc {

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

} // namespace acc

#endif // ACC_FRONTEND_SEMA_H
