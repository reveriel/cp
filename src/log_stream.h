#pragma once

#include <sstream>
namespace cp {

// when write to LogStream, write to internal buffer
class LogStream {
public:
  LogStream() : buffer_(1024, '\0'), ss_(buffer_) {}

  LogStream &operator=(const LogStream &) = delete;
  LogStream(const LogStream &) = delete;

  // if the buffer is full, stop wrting
  template <typename T> LogStream &operator<<(const T &pf) {
    if (ss_.str().size() < buffer_.size()) {
      ss_ << pf;
    }
    return *this;
  }

private:
  std::string buffer_;
  std::ostringstream ss_;
};


namespace detail
{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

// A fix-sized buffer on the stack
template <int SIZE>
class FixedBuffer {
  public:
  FixedBuffer() : cur_(data_) {
  }
  ~FixedBuffer(){

  }

  void append(const char * buf, size_t len) {
    if (remaining() > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char *data() const { return data_; }

  char *current() { return cur_; }

  int remaining() const { return static_cast<int>(end() - cur_); }

  int avail() const { return static_cast<int>(end() - cur_); }

  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }

  void bzero() { memset(data_, 0, sizeof(data_)); }

  std::string toString() const { return std::string(data_, length()); }

  std::string toString(int len) const { return std::string(data_, len); }

  const char *end() const { return data_ + sizeof(data_); }

  int length() const { return static_cast<int>(cur_ - data_); }

private:
  char data_[SIZE];
  char * cur_;
};

}


} // namespace cp
