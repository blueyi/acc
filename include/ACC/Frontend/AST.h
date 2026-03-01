//===- AST.h - ACC Abstract Syntax Tree -------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Defines the AST node types for the ACC DSL.
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief AST node types for the ACC DSL (Frontend). */

#ifndef ACC_FRONTEND_AST_H
#define ACC_FRONTEND_AST_H

#include <memory>
#include <string>
#include <vector>

namespace acc {

/// Base class for all AST expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() = default;
};

/// Represents a tensor type specification (e.g., tensor<1, 3, 224, 224, f32>).
struct TensorType {
  std::vector<int64_t> shape;  ///< Shape dimensions.
  std::string elementType;     ///< Element type string, e.g. "f32", "f16", "i8".
};

/// Represents a layer declaration (e.g., layer conv1 = Conv2D(...)).
class LayerDeclAST : public ExprAST {
public:
  std::string name;       ///< Layer name.
  std::string opType;    ///< Operator type, e.g. "Conv2D", "ReLU".
  std::vector<std::unique_ptr<ExprAST>> args;  ///< Positional arguments.
  std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> namedArgs;  ///< Named parameters (e.g. filters=64).
};

/// Represents a model declaration.
class ModelDeclAST : public ExprAST {
public:
  std::string name;  ///< Model name.
  std::vector<std::pair<std::string, TensorType>> inputs;  ///< Input name and type.
  std::vector<std::unique_ptr<LayerDeclAST>> layers;  ///< Layer list.
  std::unique_ptr<ExprAST> output;  ///< Output expression.
};

/// Represents a compile declaration with target and optimization options.
class CompileDeclAST : public ExprAST {
public:
  std::string modelName;   ///< Name of the model to compile.
  std::string target;     ///< Target backend, e.g. "cpu", "cuda".
  bool fuseOps = false;   ///< Whether to enable operator fusion.
  bool vectorize = false; ///< Whether to enable vectorization.
  std::vector<int64_t> tilingSizes;  ///< Tiling sizes for optimization.
};

/// Represents a complete source file / compilation unit.
class ModuleAST {
public:
  std::vector<std::unique_ptr<ModelDeclAST>> models;       ///< Model declarations.
  std::vector<std::unique_ptr<CompileDeclAST>> compileDecls;  ///< Compile directives.
};

} // namespace acc

#endif // ACC_FRONTEND_AST_H
