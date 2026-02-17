//===- AST.h - ACC Abstract Syntax Tree -------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Defines the AST node types for the ACC DSL.
//
//===----------------------------------------------------------------------===//

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
  std::vector<int64_t> shape;
  std::string elementType; // "f32", "f16", "i8", etc.
};

/// Represents a layer declaration (e.g., layer conv1 = Conv2D(...)).
class LayerDeclAST : public ExprAST {
public:
  std::string name;
  std::string opType;
  std::vector<std::unique_ptr<ExprAST>> args;
  // Named parameters (e.g., filters=64, kernel=3)
  std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> namedArgs;
};

/// Represents a model declaration.
class ModelDeclAST : public ExprAST {
public:
  std::string name;
  std::vector<std::pair<std::string, TensorType>> inputs;
  std::vector<std::unique_ptr<LayerDeclAST>> layers;
  std::unique_ptr<ExprAST> output;
};

/// Represents a compile declaration with target and optimization options.
class CompileDeclAST : public ExprAST {
public:
  std::string modelName;
  std::string target;
  // Optimization options
  bool fuseOps = false;
  bool vectorize = false;
  std::vector<int64_t> tilingSizes;
};

/// Represents a complete source file / compilation unit.
class ModuleAST {
public:
  std::vector<std::unique_ptr<ModelDeclAST>> models;
  std::vector<std::unique_ptr<CompileDeclAST>> compileDecls;
};

} // namespace acc

#endif // ACC_FRONTEND_AST_H
