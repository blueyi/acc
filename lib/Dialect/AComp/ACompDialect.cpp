#include "acompiler/Dialect/AComp/ACompDialect.h"

#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OpImplementation.h"

using namespace mlir;

namespace acompiler {

ACompDialect::ACompDialect(MLIRContext *context)
    : Dialect(getDialectNamespace(), context, TypeID::get<ACompDialect>()) {
  // Starter dialect: ops/types will be added incrementally.
}

} // namespace acompiler

