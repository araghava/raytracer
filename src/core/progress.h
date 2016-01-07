#ifndef __PROGRESS_H_
#define __PROGRESS_H_

#include <mutex>
#include <string>

// Progress reporter with built-in lock.
class ProgressReporter {
public:
  ProgressReporter(const int num)
    : numReports(num), current(0), lastPct(0) {
    for (int i = 0; i < 100; i++) {
      progressStr += "_";
    }
  }

  void report() {
    std::lock_guard<decltype(mutex)> guard(mutex);
    if (current >= numReports) {
      return;
    }

    const int pct = (100 * current + numReports - 1) / numReports;
    if (pct != lastPct) {
      progressStr[lastPct] = '=';
      progressStr[pct] = '>';
      fprintf(stderr, "\r%s %d%%", progressStr.c_str(), pct);
    }

    lastPct = pct;
    current++;
  }

  void finalize() {
    fprintf(stderr, "\n");
  }

private:
  std::mutex mutex;
  std::string progressStr;

  const int numReports;
  int current;
  int lastPct;
};

#endif
