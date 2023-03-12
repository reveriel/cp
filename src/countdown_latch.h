#pragma once
#include "thread_safety_analysis.h"

namespace cp {

class CountDownLatch {
public:
  explicit CountDownLatch(int count);
  CountDownLatch(const CountDownLatch &) = delete;
  CountDownLatch &operator=(const CountDownLatch &) = delete;
  void wait();
  void countDown();
  int getCount() const;

private:
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  int count_;
};
} // namespace cp
