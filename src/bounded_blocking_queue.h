
#include "../src/blocking_queue.h"
#include "../src/countdown_latch.h"
#include "../src/timestamp.h"

#include <mutex>

#include <iostream>
#include <cassert>

namespace cp {

template <typename T> class BoundedBlockingQueue {
public:
  BoundedBlockingQueue(int capacity) : capacity_(capacity) {
    assert(capacity > 0);
  }

  void put(const T &x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.size() == capacity_) {
      cv_not_full_.wait(lock);
    }
    assert(queue_.size() < capacity_);
    queue_.push_back(x);
    cv_not_empty_.notify_one();
  }

  void put(T &&x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.size() == capacity_) {
      cv_not_full_.wait(lock);
    }
    assert(queue_.size() < capacity_);
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
    cv_not_full_.notify_one();
    return front;
  }

  size_t size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

  bool empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  bool full() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size() == capacity_;
  }

  size_t capacity() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return capacity_;
  }

private:
  mutable std::mutex mutex_;
  std::condition_variable cv_not_empty_;
  std::condition_variable cv_not_full_;
  std::deque<T> queue_;
  const size_t capacity_;
};

} // namespace cp
