//===- Timer.h - ACC Compilation Timer ---------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

#ifndef ACC_SUPPORT_TIMER_H
#define ACC_SUPPORT_TIMER_H

#include <chrono>
#include <string>
#include <vector>

namespace acc {

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

} // namespace acc

#endif // ACC_SUPPORT_TIMER_H
