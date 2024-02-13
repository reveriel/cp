#pragma once
#include "countdown_latch.h"
#include "log_stream.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
namespace cp {


// thread safe, asynchronous logging class
// each appender thread has a `currentBuffer_` and a `nextBuffer_`, when
// `currentBuffer_` is full, it will be swapped with `nextBuffer_` and notify
// the logging thread to write to file.
//  When append(),
// `buffers_` is a vector of buffers that are full and need to be written to
// file.
// `flushInterval_` is the interval in seconds that the logging thread will
// flush the buffers to file.
// A backgrouhd thread is created to write the buffers to file.
class AsyncLogging {
  // nocopy
  AsyncLogging(const AsyncLogging &) = delete;
  AsyncLogging &operator=(const AsyncLogging &) = delete;

public:
  // log file name will be `basename` + "." + pid + "." + hostname + ".log"
 // Input:
  // - `rollSize`: max bytes of log file
  // - `flushInterval`: flush interval in seconds.
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

  // 4MB buffer
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
  // buffers that are full and need to be written to file
  BufferVec buffers_; // GUARDED_BY(mutex_);
};

} // namespace cp
