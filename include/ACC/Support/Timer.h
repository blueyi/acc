//===- Timer.h - ACC Compilation Timer ---------------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//

/** \file
 * \brief Compilation phase timer for ACC. */

#ifndef ACC_SUPPORT_TIMER_H
#define ACC_SUPPORT_TIMER_H

#include <chrono>
#include <string>
#include <vector>

namespace acc {

/// Timer for measuring compilation phase durations.
class Timer {
public:
  /// Start a named phase (stops any running phase).
  /// \param phaseName Name of the phase for reporting.
  void start(const std::string &phaseName);
  /// Stop the current phase and record its duration.
  void stop();
  /// Print a summary of all phase durations.
  void report() const;

private:
  /// One recorded phase (name + start/end times).
  struct TimerEntry {
    std::string name;  ///< Phase name.
    std::chrono::steady_clock::time_point start;  ///< Start time.
    std::chrono::steady_clock::time_point end;    ///< End time.
    /// \return Duration in milliseconds.
    double durationMs() const;
  };

  std::vector<TimerEntry> entries_;
  int currentEntry_ = -1;
};

} // namespace acc

#endif // ACC_SUPPORT_TIMER_H
