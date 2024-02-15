#pragma once

#include "noncopyable.h"
#include <stddef.h>
#include <string>

namespace cp {

namespace detail {

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

// A fix-sized buffer
template <int SIZE> class FixedBuffer {
public:
  FixedBuffer() : cur_(data_) {
  }
  ~FixedBuffer() = default;

  // append `len` bytes to buffer, if buffer is full, stop this appending
  void append(const char *buf, size_t len) {
    if (static_cast<size_t>(avail()) > len) {
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
  char *cur_;
};

} // namespace detail

// When wrting to LogStream, write to the internal buffer.
// If the buffer is full, this write operation will be ignored.
class LogStream : public noncopyable {
public:
  using Buffer = detail::FixedBuffer<detail::kSmallBuffer>;
  LogStream() = default;
  //
  LogStream &operator<<(bool v);
  LogStream &operator<<(short);
  LogStream &operator<<(unsigned short);
  LogStream &operator<<(int);
  LogStream &operator<<(unsigned int);
  LogStream &operator<<(long);
  LogStream &operator<<(unsigned long);
  LogStream &operator<<(long long);
  LogStream &operator<<(unsigned long long);
  LogStream &operator<<(float v);
  LogStream &operator<<(double v);
  LogStream &operator<<(char v);
  LogStream &operator<<(const char *str);
  LogStream &operator<<(const unsigned char *str);
  LogStream &operator<<(const std::string &str);
  LogStream &operator<<(const Buffer &buffer);


  // append `len` bytes to buffer, if buffer is full, stop this appending
  void append(const char *data, size_t len) { buffer_.append(data, len); }

  // return the internal buffer
  const Buffer &buffer() const { return buffer_; }

  void reset() { buffer_.reset(); }

private:
  Buffer buffer_;

  static const int kMaxNumericSize = 48;
  static void staticCheck();
  template <typename T> void formatInteger(T);
};

} // namespace cp
