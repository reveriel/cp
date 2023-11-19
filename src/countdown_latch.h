#pragma once
#include "thread_safety_analysis.h"
#include <condition_variable>

namespace cp {

// usage:
//
// 1. sync end. one thread waits on a CountDownLatch(int count)
//
// each theard calls countDown() when it finishes.
// the master thread calls wait() to wait for all threads to finish
//
// 2. sync start. many threads wait on a CountDownLatch(1)
//
// each thread calls wait().
// the master threads calls countDown() to signal other threads to start.
//

class CountDownLatch {
public:
  explicit CountDownLatch(int count);
  CountDownLatch(const CountDownLatch &) = delete;
  CountDownLatch &operator=(const CountDownLatch &) = delete;

  // suspends the calling thread until the latch has counted down to zero
  void wait();

  // decrements the count
  void countDown();
  int getCount() const;

private:
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  int count_;
};
} // namespace cp
