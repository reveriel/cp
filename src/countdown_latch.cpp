#include "countdown_latch.h"

#include <cassert>

namespace cp {

CountDownLatch::CountDownLatch(int count) : count_(count) {
  assert(count >= 0);
}

void CountDownLatch::wait() {
  std::unique_lock<std::mutex> lock(mutex_);
  // while (count_ > 0) {
  //   condition_.wait(lock);
  // }
  condition_.wait(lock, [&]() { return count_ == 0; });
}

void CountDownLatch::countDown() {
  std::unique_lock<std::mutex> lock(mutex_);
  --count_;
  if (count_ == 0) {
    condition_.notify_all();
  }
}

int CountDownLatch::getCount() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return count_;
}

} // namespace cp
