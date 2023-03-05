
#include <mutex>
#include <thread>

namespace cp {

// add a thread id to the mutex
class Mutex {
private:
  std::mutex m_mutex;
  std::thread::id m_tid;

public:
  Mutex() : m_mutex() {}
  void lock() {
    m_mutex.lock();
    // the thread id is only set when the mutex is locked
    m_tid = std::this_thread::get_id();
  }
  void unlock() {
    m_tid = std::thread::id();
    m_mutex.unlock();
  }

  bool is_locked_by_this_thread() {
    return m_tid == std::this_thread::get_id();
  }

  bool try_lock() { return m_mutex.try_lock(); }
};

} // namespace cp
