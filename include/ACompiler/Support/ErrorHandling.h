//===- ErrorHandling.h - ACompiler Error Handling ----------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_SUPPORT_ERRORHANDLING_H
#define ACOMPILER_SUPPORT_ERRORHANDLING_H

#include "llvm/Support/Error.h"
#include <string>

namespace acompiler {

/// Report a fatal error and terminate.
[[noreturn]] void reportFatalError(const std::string &message);

/// Create an llvm::Error with the given message.
llvm::Error makeError(const std::string &message);

} // namespace acompiler

#endif // ACOMPILER_SUPPORT_ERRORHANDLING_H
