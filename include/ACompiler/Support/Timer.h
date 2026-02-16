//===- Timer.h - ACompiler Compilation Timer ---------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_SUPPORT_TIMER_H
#define ACOMPILER_SUPPORT_TIMER_H

#include <chrono>
#include <string>
#include <vector>

namespace acompiler {

/// A simple timer for measuring compilation phase durations.
class Timer {
public:
  void start(const std::string &phaseName);
  void stop();
  void report() const;

private:
  struct TimerEntry {
    std::string name;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    double durationMs() const;
  };

  std::vector<TimerEntry> entries_;
  int currentEntry_ = -1;
};

} // namespace acompiler

#endif // ACOMPILER_SUPPORT_TIMER_H
