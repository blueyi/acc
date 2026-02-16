//===- Logger.h - ACompiler Logging System -----------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_SUPPORT_LOGGER_H
#define ACOMPILER_SUPPORT_LOGGER_H

#include "llvm/Support/raw_ostream.h"
#include <string>

namespace acompiler {

enum class LogLevel { Debug, Info, Warning, Error };

class Logger {
public:
  static Logger &instance();

  void setLevel(LogLevel level) { level_ = level; }
  void log(LogLevel level, const std::string &message);

  void debug(const std::string &msg) { log(LogLevel::Debug, msg); }
  void info(const std::string &msg) { log(LogLevel::Info, msg); }
  void warning(const std::string &msg) { log(LogLevel::Warning, msg); }
  void error(const std::string &msg) { log(LogLevel::Error, msg); }

private:
  LogLevel level_ = LogLevel::Info;
  Logger() = default;
};

} // namespace acompiler

#endif // ACOMPILER_SUPPORT_LOGGER_H
