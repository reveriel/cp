#pragma once

#include "thread_safety_analysis.h"
#include <condition_variable>
#include <mutex>

#include <deque>

namespace cp {
template <typename T> class BlockingQueue {
public:
  BlockingQueue() = default;
  BlockingQueue(const BlockingQueue &) = delete;
  BlockingQueue &operator=(const BlockingQueue &) = delete;

  void put(const T &x) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push_back(x);
    cv_not_empty_.notify_one();
  }

  void put(T &&x) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push_back(std::move(x));
    cv_not_empty_.notify_one();
  }

  T take() {
    std::unique_lock<std::mutex> lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty()) {
      cv_not_empty_.wait(lock);
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
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
