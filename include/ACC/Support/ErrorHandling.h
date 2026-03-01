//===- ErrorHandling.h - ACC Error Handling ----------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Error reporting and llvm::Error helpers for ACC. */

#ifndef ACC_SUPPORT_ERRORHANDLING_H
#define ACC_SUPPORT_ERRORHANDLING_H

#include "llvm/Support/Error.h"
#include <string>

namespace acc {

/// Report a fatal error and terminate the process.
/// \param message Error message to report.
[[noreturn]] void reportFatalError(const std::string &message);

/// Create an llvm::Error with the given message.
/// \param message Error description.
/// \return An llvm::Error holding the message.
llvm::Error makeError(const std::string &message);

} // namespace acc

#endif // ACC_SUPPORT_ERRORHANDLING_H
