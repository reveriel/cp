#pragma once
#include "thread_safety_analysis.h"
#include <mutex>
#include <thread>

namespace cp {

// add a thread id to the mutex
class Mutex {
private:
  std::mutex mu_;
  // the thread id that held the mutex
  std::thread::id m_tid;

public:
  Mutex() : mu_() {}
  void lock()  {
    mu_.lock();
    // the thread id is only set when the mutex is locked
    m_tid = std::this_thread::get_id();
  }

  void unlock() {
    m_tid = std::thread::id();
    mu_.unlock();
  }

  bool is_locked_by_this_thread() {
    return m_tid == std::this_thread::get_id();
  }

  bool try_lock() { return mu_.try_lock(); }
};

} // namespace cp
