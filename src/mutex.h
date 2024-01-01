#pragma once
#include "thread_safety_analysis.h"
#include <mutex>
#include <thread>

namespace cp {

// add a thread id to the mutex
class CAPABILITY("mutex") Mutex {
private:
  std::mutex mu_;
  // the thread id that held the mutex
  std::optional<std::thread::id> tid_;

public:
  std::optional<std::thread::id> get_tid() { return tid_; }
  std::mutex &get() { return mu_; }

  Mutex() : mu_() {}

   void lock() ACQUIRE(mu_) {
    mu_.lock();
    // the thread id is only set when the mutex is locked
    tid_ = std::this_thread::get_id();
  }

  void unlock() RELEASE(mu_) {
    tid_.reset();
    mu_.unlock();
  }

  bool is_locked_by_this_thread() {
    return tid_ == std::this_thread::get_id();
  }

  bool try_lock() { return mu_.try_lock(); }
};

} // namespace cp
