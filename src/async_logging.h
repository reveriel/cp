#pragma once
#include "countdown_latch.h"
#include "log_stream.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
namespace cp {

class AsyncLogging {
  // nocopy
  AsyncLogging(const AsyncLogging &) = delete;
  AsyncLogging &operator=(const AsyncLogging &) = delete;

public:
  AsyncLogging(const std::string &basename, off_t rollSize,
               int flushInterval = 3)
      : flushInterval_(flushInterval), running_(false), basename_(basename),
        rolllSize_(rollSize), latch_(1), mutex_(), cond_(),
        currentBuffer_(new Buffer), nextBuffer_(new Buffer), buffers_() {
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);
  }
  ~AsyncLogging() {
    if (running_) {
      stop();
    }
  }

  void start() {
    running_ = true;
    thread_ = std::thread(std::bind(&AsyncLogging::threadFunc, this));
    latch_.wait();
  }

  void append(const char *logline, int len);

  void stop() {
    running_ = false;
    cond_.notify_one();
    thread_.join();
  }

private:
  void threadFunc();

  using Buffer = detail::FixedBuffer<detail::kLargeBuffer>;
  using BufferVec = std::vector<std::unique_ptr<Buffer>>;
  using BufferPtr = std::unique_ptr<Buffer>;

  int flushInterval_; // log flush interval in seconds
  std::atomic<bool> running_;
  const std::string basename_;
  const off_t rolllSize_;

  std::thread thread_;

  CountDownLatch latch_;
  std::mutex mutex_;
  std::condition_variable cond_;
  BufferPtr currentBuffer_ ; // GUARDED_BY(mutex_);
  BufferPtr nextBuffer_ ; //GUARDED_BY(mutex_);
  BufferVec buffers_; // GUARDED_BY(mutex_);
};

} // namespace cp
