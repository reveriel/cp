#include "../src/blocking_queue.h"
#include "../src/countdown_latch.h"
#include "../src/timestamp.h"

#include <chrono>
#include <mutex>

#include <thread>

#include <iostream>

void bench(int num_thread, int num_count) {
  cp::CountDownLatch latch(1);

  // master threads put to queue_start, with timestamp
  cp::BlockingQueue<cp::Timestamp> queue_start;

  // worker threads consume from queue_start, put to queue_end, with delay
  // worker threads stop when got cp::Timestamp::invalid()
  cp::BlockingQueue<cp::TimeDifference> queue_end;

  for (int i = 0; i < num_thread; i++) {
    std::thread([&latch, &queue_start, &queue_end]() {
      latch.wait();
      std::cout << "thread " << std::this_thread::get_id() << " started"
                << std::endl;

      cp::Timestamp start(queue_start.take());
      while (start.valid()) {
        queue_end.put(cp::Timestamp::now() - start);
        start = queue_start.take();
      }
    }).detach();
  }

  cp::TimeDifference sum;
  latch.countDown();
  for (int i = 0; i < num_count; i++) {
    queue_start.put(cp::Timestamp::now());
    sum += queue_end.take();
  }

  std::cout << "num_thread: " << num_thread << ", num_count: " << num_count
            << ", avg: " << sum.toMilliseconds() / num_count << " ms"
            << std::endl;

  // stop worker threads
  for (int i = 0; i < num_thread; i++) {
    queue_start.put(cp::Timestamp::invalid());
  }
}

int main() {
  for (int i = 1; i < 100; i++) {
    bench(i, 1000 * 1000);
  }
}
