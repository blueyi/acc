//===- ErrorHandling.h - ACC Error Handling ----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_SUPPORT_ERRORHANDLING_H
#define ACC_SUPPORT_ERRORHANDLING_H

#include "llvm/Support/Error.h"
#include <string>

namespace acc {

/// Report a fatal error and terminate.
[[noreturn]] void reportFatalError(const std::string &message);

/// Create an llvm::Error with the given message.
llvm::Error makeError(const std::string &message);

} // namespace acc

#endif // ACC_SUPPORT_ERRORHANDLING_H
