#pragma once

#include "thread_safety_analysis.h"
#include <condition_variable>
#include <mutex>

#include <deque>
#include <cassert>

#include <iostream>

namespace cp {
template <typename T> class BlockingQueue {
public:
  BlockingQueue() = default;
  BlockingQueue(const BlockingQueue &) = delete;
  BlockingQueue &operator=(const BlockingQueue &) = delete;

  void put(const T &x) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(x);
    cv_not_empty_.notify_one();
  }

  void put(T &&x) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(std::move(x));
    cv_not_empty_.notify_one();
  }

  T take() {
    std::unique_lock<std::mutex> lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    try {
    while (queue_.empty()) {
      cv_not_empty_.wait(lock);
    }
    } catch (const std::system_error &e) {
      // print more info
      std::cout << "std::system_error: " << e.what() << std::endl;
    }

    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
  }

  bool empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  size_t size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

private:
  mutable std::mutex mutex_;
  std::condition_variable cv_not_empty_;
  std::deque<T> queue_;
};
} // namespace cp
