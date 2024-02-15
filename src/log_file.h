#pragma once

#include "file_util.h"
#include <memory>
#include <mutex>
#include <string>

namespace cp {
namespace fileutil {
class AppendFile;
}

// a log file that rolls over to a new file every `kRollPerSeconds_` seconds or
// `kRollSize_` bytes.
class LogFile {
  // no copy
  LogFile(const LogFile &) = delete;
  LogFile &operator=(const LogFile &) = delete;

public:
  // Input:
  // - `basename`: log file name base.
  // - `roll_size`: max bytes of log file
  // - `thread_safe`: if true, use mutex
  // - `flush_interval`: flush interval in seconds.
  // - `check_every_n`: check flush interval every n times of `append()`
  LogFile(const std::string &basename, size_t roll_size,
          bool thread_safe = true, int flush_interval = 3,
          int check_every_n = 1024);
  ~LogFile() {}

  void append(const char *logline, size_t len);
  void flush();

  // roll: create a new log file if a `kRollPerSeconds_` time has passed or
  // `roll_size_` bytes has been written. Return if rolled.
  bool rollFile();

private:
  void append_unlock(const char *logline, size_t len);

  // Return now's log file name, in this format:
  //   basename.yyyymmdd-hhmmss.hostname.pid.log
  static std::string getLogFileName(const std::string &basename, time_t *now);

  const std::string basename_;
  const size_t roll_size_;
  const int flush_interval_;
  const int check_every_n_;

  int count_;

  std::unique_ptr<std::mutex> mutex_; // if thread safe is true, use mutex
  time_t startOfPeriod_; // last roll time, round to `kRollPerSeconds_`
  time_t lastRoll_;
  time_t lastFlush_;
  // a buffered file stream for output
  std::unique_ptr<fileutil::AppendFile> file_;

  const static int kRollPerSeconds_ = 60 * 60 * 24; // 1 day
};

} // namespace cp
