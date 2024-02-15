#include "timestamp.h"
#include "logging.h"
#include "async_logging.h"

#include <cstring>
#include <thread>

using namespace cp;

cp::AsyncLogging *G_logger = nullptr;

void asyncOutput(const char *msg, int len) { G_logger->append(msg, len); }

void bench(bool longLog) {
  cp::Logger::setOutput(asyncOutput);

  int cnt = 0;
  const int BATCH = 1000;

  std::string long_str(3000, 'X');
  std::string empty = " ";


  for (int t = 0; t < 30; ++t) {
    cp::Timestamp start = cp::Timestamp::now();

    for (int i = 0; i < BATCH; i++) {
      LOG_INFO << "Hello 0123456789"
               << " abcdefghijklmnopqrstuvwxyz "
               << (longLog ? long_str : empty)
               << cnt;
      ++cnt;
    }
    cp::Timestamp end = cp::Timestamp::now();
    printf("used %f\n", (end - start).toMilliseconds());
    // std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main() {

  std::string log_file = "async_log";

  size_t roll_size = 1024 * 1024 * 10; // 10 MB
  cp::AsyncLogging log(log_file, roll_size);
  log.start();
  G_logger = &log;

  bench(false);
}