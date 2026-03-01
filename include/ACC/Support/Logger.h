//===- Logger.h - ACC Logging System -----------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Logging facility for ACC. */

#ifndef ACC_SUPPORT_LOGGER_H
#define ACC_SUPPORT_LOGGER_H

#include "llvm/Support/raw_ostream.h"
#include <string>

namespace acc {

/// Log severity levels.
enum class LogLevel { Debug, Info, Warning, Error };

/// Singleton logger with configurable level and convenience methods.
class Logger {
public:
  /// \return The global logger instance.
  static Logger &instance();

  /// Set minimum log level.
  /// \param level Messages below this level are not emitted.
  void setLevel(LogLevel level) { level_ = level; }
  /// Log a message at the given level.
  /// \param level Severity.
  /// \param message Message text.
  void log(LogLevel level, const std::string &message);

  /// Log at Debug level.
  void debug(const std::string &msg) { log(LogLevel::Debug, msg); }
  /// Log at Info level.
  void info(const std::string &msg) { log(LogLevel::Info, msg); }
  /// Log at Warning level.
  void warning(const std::string &msg) { log(LogLevel::Warning, msg); }
  /// Log at Error level.
  void error(const std::string &msg) { log(LogLevel::Error, msg); }

private:
  LogLevel level_ = LogLevel::Info;
  Logger() = default;
};

} // namespace acc

#endif // ACC_SUPPORT_LOGGER_H
