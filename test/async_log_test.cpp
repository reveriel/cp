#include "timestamp.h"
#include "logging.h"
#include "async_logging.h"

#include <cstring>
#include <thread>

using namespace cp;

void bench() {

  int cnt = 0;
  const int BATCH = 1000;

  std::string long_str(3000, 'X');


  for (int t = 0; t < 30; ++t) {
    cp::Timestamp start = cp::Timestamp::now();

    for (int i = 0; i < BATCH; i++) {
      LOG_INFO << "Hello 0123456789"
               << " abcdefghijklmnopqrstuvwxyz " << cnt;
      ++cnt;
    }
    cp::Timestamp end = cp::Timestamp::now();
    printf("used %f\n", (end - start).toMilliseconds());
  }
}

int main() {

  std::string log_file = "a.log";

  size_t roll_size = 1024 * 1024 * 10; // 10 MB
  cp::AsyncLogging log(log_file, roll_size);
  log.start();

  bench();
}