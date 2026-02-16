//===- ErrorHandling.cpp - ACompiler Error Handling -------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdlib>

namespace acompiler {

[[noreturn]] void reportFatalError(const std::string &message) {
  llvm::errs() << "ACompiler fatal error: " << message << "\n";
  std::abort();
}

llvm::Error makeError(const std::string &message) {
  return llvm::make_error<llvm::StringError>(message,
                                              llvm::inconvertibleErrorCode());
}

} // namespace acompiler
