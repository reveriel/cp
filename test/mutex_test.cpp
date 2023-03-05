
#include "src/mutex.h"
#include <chrono>
#include <iostream>
#include <vector>

// global vars
cp::Mutex g_mutex;
int g_count = 0;
const int kCOUNT = 1000 * 1000;

std::vector<int> g_vec;

int foo() {
  std::lock_guard<cp::Mutex> lock(g_mutex);
  if (!g_mutex.is_locked_by_this_thread()) {
    std::cerr << "FAIL: mutex is not locked by this thread" << std::endl;
    return -1;
  }
  ++g_count;
  return 0;
}

int main() {
  std::cout << "size of mutex: " << sizeof(std::mutex) << std::endl;

  assert(foo() == 0);
  assert(g_count == 1);

  const int MAX_THREAD = 12;
  g_vec.reserve(MAX_THREAD * kCOUNT);

  std::chrono::time_point<std::chrono::system_clock> start;
  start = std::chrono::system_clock::now();
  for (int i = 0; i < kCOUNT; i++) {
    g_vec.push_back(i);
  }

  std::cout << "single thread without lock : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now() - start)
                   .count()
            << std::endl;

  start = std::chrono::system_clock::now();
  for (int n_thur = 1; n_thur < MAX_THREAD; n_thur++) {
    std::vector<std::thread> threads;
    g_vec.clear();
    for (int i = 0; i < n_thur; i++) {
      threads.push_back(std::thread([]() {
        for (int i = 0; i < kCOUNT; i++) {
          std::lock_guard<cp::Mutex> lock(g_mutex);
          g_vec.push_back(i);
        }
      }));
    }

    for (auto &t : threads) {
      t.join();
    }
    std::cout << "multi thread with lock : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now() - start)
                     .count()
              << std::endl;
  }
  // tested on my laptop, 2.6 GHz 6-Core Intel Core i7

  // 1: size of mutex: 64
  // 1: single thread without lock : 34
  // 1: multi thread with lock : 76
  // 1: multi thread with lock : 282
  // 1: multi thread with lock : 739
  // 1: multi thread with lock : 1239
  // 1: multi thread with lock : 1987
  // 1: multi thread with lock : 2850
  // 1: multi thread with lock : 3918
  // 1: multi thread with lock : 5027
  // 1: multi thread with lock : 6348
  // 1: multi thread with lock : 8226
  // 1: multi thread with lock : 10262
}
