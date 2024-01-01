#pragma once

#include "log_stream.h"
#include "timestamp.h"

#include <string>
namespace cp {

class Logger {
public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  Logger(std::string file, int line, LogLevel level);

  LogStream& stream() { return impl_.stream_; }

private:
  class Impl {
  public:
    Timestamp time_;

    LogStream stream_;

    LogLevel level_;

    int line_;

    std::string source_file_;
  };

  Impl impl_;
};

// global current log level
extern Logger::LogLevel g_logLevel;

template <int LEVEL> class LogLevelHelper {
public:
  explicit LogLevelHelper(Logger::LogLevel level) : level_(level) {}

  operator Logger::LogLevel() const { return level_; }

private:
  Logger::LogLevel level_;
};

#define LOG_TRACE cp::Logger(__FILE__, __LINE__, cp::Logger::TRACE).stream()
#define LOG_DEBUG cp::Logger(__FILE__, __LINE__, cp::Logger::DEBUG).stream()
#define LOG_INFO cp::Logger(__FILE__, __LINE__, cp::Logger::INFO).stream()
#define LOG_WARN cp::Logger(__FILE__, __LINE__, cp::Logger::WARN).stream()
#define LOG_ERROR cp::Logger(__FILE__, __LINE__, cp::Logger::ERROR).stream()
#define LOG_FATAL cp::Logger(__FILE__, __LINE__, cp::Logger::FATAL).stream()

template <typename T, int LEVEL>
LogLevelHelper<LEVEL> operator<<(LogLevelHelper<LEVEL> &log, const T &value) {
  LogStream os;
  os << value;
  return log;
}

} // namespace cp
