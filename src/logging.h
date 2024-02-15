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

  class SourceFile
  {
  public:
    template<int N>
    SourceFile(const char (&arr)[N])
      : data_(arr),
        size_(N-1)
    {
      const char* slash = strrchr(data_, '/'); // builtin function
      if (slash)
      {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename)
      : data_(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char * func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  static LogLevel logLevel();

  static void setLogLevel(LogLevel level);

  using OutputFunc = void (*)(const char* msg, int len);
  using FlushFunc = void (*)();

  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

  LogStream &stream() { return impl_.stream_; }

private:
  class Impl {
  public:
    Impl(LogLevel level, int old_errno, const SourceFile &file, int line);
    void formatTime();
    void finish();
    Timestamp time_;
    LogStream stream_;
    LogLevel level_;
    int line_;
    SourceFile basename_;
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

#define LOG_TRACE                                                              \
  if (cp::Logger::LogLevel() <= cp::Logger::TRACE)                             \
  cp::Logger(__FILE__, __LINE__, cp::Logger::TRACE, __func__).stream()

#define LOG_DEBUG                                                              \
  if (cp::Logger::LogLevel() <= cp::Logger::DEBUG)                             \
  cp::Logger(__FILE__, __LINE__, cp::Logger::DEBUG, __func__).stream()

#define LOG_INFO                                                               \
  if (cp::Logger::LogLevel() <= cp::Logger::INFO)                              \
  cp::Logger(__FILE__, __LINE__).stream()

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
