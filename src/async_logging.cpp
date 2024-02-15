#include "async_logging.h"
#include "log_file.h"
#include "timestamp.h"
#include <cassert>

namespace cp {

void AsyncLogging::append(const char *logline, int len) {

  std::lock_guard lg(mutex_);
  if (currentBuffer_->avail() > len) {
    // if the buffer is not full, write to it
    currentBuffer_->append(logline, len);
  } else {
    // if the buffer is full, write to next buffer
    buffers_.push_back(std::move(currentBuffer_));

    if (nextBuffer_) {
      currentBuffer_ = std::move(nextBuffer_);
    } else {
      // if nextBuffer_ is null, create a new one
      currentBuffer_.reset(new Buffer);
    }
    // now currentBuffer_ is not full
    currentBuffer_->append(logline, len);
    cond_.notify_one();
  }
}

void AsyncLogging::threadFunc() {
  assert(running_);
  latch_.countDown(); // wait until constructor
  LogFile output(basename_, rolllSize_, false);
  auto newBuffer1 = std::make_unique<Buffer>();
  auto newBuffer2 = std::make_unique<Buffer>();
  newBuffer1->bzero();
  newBuffer2->bzero();
  BufferVec buffers_to_write;
  buffers_to_write.reserve(16);
  while (running_) {

    assert(newBuffer1 && newBuffer1->length() == 0);
    assert(newBuffer2 && newBuffer2->length() == 0);
    assert(buffers_to_write.empty());
    {
      std::unique_lock lock(mutex_);
      if (buffers_.empty()) {
        cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
      }
      buffers_.push_back(std::move(currentBuffer_));
      currentBuffer_ = std::move(newBuffer1);
      buffers_to_write.swap(buffers_);
      if (!nextBuffer_) {
        nextBuffer_ = std::move(newBuffer2);
      }
    }

    assert(!buffers_to_write.empty());

    if (buffers_to_write.size() > 25) {
      char buf[256];
      // too many buffers
      snprintf(buf, sizeof buf,
               "Dropped log messages at %s, %zd larger buffers\n",
               Timestamp::now().toFormattedString().c_str(),
               buffers_to_write.size() - 2);
      fputs(buf, stderr);
      output.append(buf, static_cast<int>(strlen(buf)));
      buffers_to_write.erase(buffers_to_write.begin() + 2,
                             buffers_to_write.end());
    }

    for (const auto &buffer : buffers_to_write) {
      output.append(buffer->data(), buffer->length());
    }
    if (buffers_to_write.size() > 2) {
      buffers_to_write.resize(2);
    }
    if (!newBuffer1) {
      assert(!newBuffer2);
      newBuffer1 = std::move(buffers_to_write[0]);
      newBuffer2 = std::move(buffers_to_write[1]);
      buffers_to_write.clear();
      assert(newBuffer1 && newBuffer1->length() == 0);
      assert(newBuffer2 && newBuffer2->length() == 0);
      newBuffer1->bzero();
      newBuffer2->bzero();
    }

    output.flush();
  }
  output.flush();
}

} // namespace cp
