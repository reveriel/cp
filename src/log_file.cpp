#include "log_file.h"
#include "file_util.h"
#include "process_info.h"
#include <time.h>

namespace cp {

LogFile::LogFile(const std::string &basename, size_t roll_size,
                 bool thread_safe, int flush_interval, int check_every_n)
    : basename_(basename), roll_size_(roll_size),
      flush_interval_(flush_interval), check_every_n_(check_every_n), count_(0),
      mutex_(thread_safe ? std::make_unique<std::mutex>() : nullptr),
      startOfPeriod_(0), lastRoll_(0), lastFlush_(0) {
  rollFile();
}

bool LogFile::rollFile() {
  time_t now = 0;
  std::string filename = getLogFileName(basename_, &now);

  time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

  if (now > lastRoll_) {
    lastRoll_ = now;
    lastFlush_ = now;
    startOfPeriod_ = start;
    file_.reset(new fileutil::AppendFile(filename));
    return true;
  }
  return false;
}

std::string LogFile::getLogFileName(const std::string &basename, time_t *now) {
  // use string append is better than stringstream if you know how how much
  // space will be used.
  std::string filename;
  filename.reserve(basename.size() + 64);
  filename = basename;

  char buf[32];
  struct tm tm;
  *now = time(NULL);
  gmtime_r(now, &tm);
  strftime(buf, sizeof(buf), ".%Y%m%d-%H%M%S", &tm);
  filename += buf;
  filename += ".";
  filename += process_info::hostname();
  filename += ".";
  filename += process_info::pid_str();
  filename += ".log";

  return filename;
}

void LogFile::append_unlock(const char *logline, size_t len) {
  file_->append(logline, len);
  if (file_->writtenBytes() > roll_size_) {
    rollFile();
  } else {
    ++count_;
    if (count_ >= check_every_n_) {
      count_ = 0;
      time_t now = 0;
      time(&now);
      if (now - lastFlush_ > flush_interval_) {
        lastFlush_ = now;
        file_->flush();
      }
    }
  }
}

void LogFile::append(const char *logline, size_t len) {
  if (mutex_) {
    std::lock_guard<std::mutex> lock(*mutex_);
    append_unlock(logline, len);
  } else {
    append_unlock(logline, len);
  }
}

void LogFile::flush() {
  if (mutex_) {
    std::lock_guard<std::mutex> lock(*mutex_);
    file_->flush();
  } else {
    file_->flush();
  }
}

} // namespace cp
