//===- ACDialect.cpp - ACompiler Dialect implementation ----------------===//
//
// ACompiler 自定义 MLIR Dialect 实现
//
//===----------------------------------------------------------------------===//

#include "ACDialect.h"
#include "mlir/IR/TypeID.h"

namespace mlir {
namespace ac {

void ACDialect::initialize() {
  // 在此注册 AC Dialect 的 Operations 和 Types
  // addOperations<>();
  // addTypes<>();
}

ACDialect::ACDialect(mlir::MLIRContext *context)
    : mlir::Dialect(getDialectNamespace(), context,
                    mlir::TypeID::get<ACDialect>()) {
  // 基本 Dialect 初始化
}

} // namespace ac
} // namespace mlir

DEFINE_EXPLICIT_TYPE_ID(mlir::ac::ACDialect)
