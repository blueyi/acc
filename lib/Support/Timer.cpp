//===- Timer.cpp - ACompiler Compilation Timer ------------------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//

#include "ACompiler/Support/Timer.h"
#include "llvm/Support/raw_ostream.h"

namespace acompiler {

double Timer::TimerEntry::durationMs() const {
  return std::chrono::duration<double, std::milli>(end - start).count();
}

void Timer::start(const std::string &phaseName) {
  entries_.push_back({phaseName, std::chrono::steady_clock::now(), {}, 0.0});
  currentEntry_ = static_cast<int>(entries_.size()) - 1;
}

void Timer::stop() {
  if (currentEntry_ >= 0) {
    entries_[currentEntry_].end = std::chrono::steady_clock::now();
    currentEntry_ = -1;
  }
}

void Timer::report() const {
  llvm::errs() << "\n=== ACompiler Compilation Timing Report ===\n";
  double total = 0.0;
  for (const auto &entry : entries_) {
    double ms = entry.durationMs();
    total += ms;
    llvm::errs() << llvm::format("  %-30s %8.2f ms\n",
                                  entry.name.c_str(), ms);
  }
  llvm::errs() << llvm::format("  %-30s %8.2f ms\n", "TOTAL", total);
  llvm::errs() << "===========================================\n";
}

} // namespace acompiler
