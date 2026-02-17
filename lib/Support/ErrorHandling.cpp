//===- ErrorHandling.cpp - ACC Error Handling -------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdlib>

namespace acc {

[[noreturn]] void reportFatalError(const std::string &message) {
  llvm::errs() << "ACC fatal error: " << message << "\n";
  std::abort();
}

llvm::Error makeError(const std::string &message) {
  return llvm::make_error<llvm::StringError>(message,
                                              llvm::inconvertibleErrorCode());
}

} // namespace acc
