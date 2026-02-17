//===- Logger.cpp - ACC Logging System --------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#include "ACC/Support/Logger.h"
#include "llvm/Support/raw_ostream.h"

namespace acc {

Logger &Logger::instance() {
  static Logger logger;
  return logger;
}

void Logger::log(LogLevel level, const std::string &message) {
  if (level < level_)
    return;

  const char *prefix = "";
  switch (level) {
  case LogLevel::Debug:   prefix = "[DEBUG] ";   break;
  case LogLevel::Info:    prefix = "[INFO]  ";   break;
  case LogLevel::Warning: prefix = "[WARN]  ";   break;
  case LogLevel::Error:   prefix = "[ERROR] ";   break;
  }

  llvm::errs() << prefix << message << "\n";
}

} // namespace acc
