#include "logging.h"

namespace cp {
thread_local char t_errnobuf[512];
thread_local char t_time[64];
thread_local time_t t_lastSecond;

// init log level from environment variable
Logger::LogLevel initLogLevel() {
  if (::getenv("CP_LOG_TRACE"))
    return Logger::TRACE;
  else if (::getenv("CP_LOG_DEBUG"))
    return Logger::DEBUG;
  else
    return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char *LogLevelName[Logger::NUM_LOG_LEVELS] = {
    "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL ",
};

// helper class for known string length at compile time
class T {
public:
  T(const char *str, unsigned len) : str_(str), len_(len) {}
  const char *str_;
  const unsigned len_;
};

inline LogStream &operator<<(LogStream &s, T v) {
  s.append(v.str_, v.len_);
  return s;
}

} // namespace cp
