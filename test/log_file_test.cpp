#include "log_file.h"
#include "logging.h"
#include <iostream>

std::unique_ptr<cp::LogFile> g_log;

void outputFunc(const char *msg, int len) { g_log->append(msg, len); }

void flushFunc() { g_log->flush(); }

int main() {

  g_log = std::make_unique<cp::LogFile>("log_file_test", 1000 * 1000);

  cp::Logger::setOutput(outputFunc);
  cp::Logger::setFlush(flushFunc);
  cp::Logger::setLogLevel(cp::Logger::LogLevel::INFO);

  for (int i = 0; i < 1000; ++i) {
    LOG_INFO << "Hello 0123456789 abcdefghijklmnopqrstuvwxyz " << i;
  }
}
