//===- ACDialect.h - ACompiler Dialect -------------------------*- C++ -*-===//
//
// ACompiler 自定义 MLIR Dialect 声明
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_DIALECT_AC_ACDIALECT_H
#define ACOMPILER_DIALECT_AC_ACDIALECT_H

#include "mlir/IR/Dialect.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/Types.h"

namespace mlir {
namespace ac {

/// ACompiler Dialect: 用于 AI 编译器高层 IR 表示
class ACDialect : public mlir::Dialect {
public:
  static llvm::StringRef getDialectNamespace() { return "ac"; }
  explicit ACDialect(mlir::MLIRContext *context);
};

} // namespace ac
} // namespace mlir

#endif // ACOMPILER_DIALECT_AC_ACDIALECT_H
