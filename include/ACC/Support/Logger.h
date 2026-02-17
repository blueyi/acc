//===- Logger.h - ACC Logging System -----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_SUPPORT_LOGGER_H
#define ACC_SUPPORT_LOGGER_H

#include "llvm/Support/raw_ostream.h"
#include <string>

namespace acc {

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

} // namespace acc

#endif // ACC_SUPPORT_LOGGER_H
