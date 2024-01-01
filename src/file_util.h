#pragma once
#include <string>

namespace cp {

namespace fileutil {

// AppendFile is a thin wrapper of FILE, which has a 64KiB buffer,
// larger than the default 8KiB file buffer in Linux.
// Not thread-safe.
class AppendFile {
  // no copy
  AppendFile(const AppendFile &) = delete;
  AppendFile &operator=(const AppendFile &) = delete;

public:
  explicit AppendFile(const std::string &filename);

  ~AppendFile();

  void append(const char *logline, size_t len);

  // [12.20.2 Flushing
  // Buffers](https://www.gnu.org/software/libc/manual/html_node/Flushing-Buffers.html)
  //
  // Flushing output on a buffered stream means transmitting all accumulated
  // characters to the file. There are many circumstances when buffered output
  // on a stream is flushed automatically:
  //
  // When you try to do output and the output buffer is full.
  // When the stream is closed. See Closing Streams.
  // When the program terminates by calling exit. See Normal Termination.
  // When a newline is written, if the stream is line buffered.
  // Whenever an input operation on any stream actually reads data from its
  // file. If you want to flush the buffered output at another time, call
  // fflush, which is declared in the header file stdio.h.
  void flush();

  size_t writtenBytes() const { return writtenBytes_; }

private:
  size_t write(const char *logline, size_t len);

  FILE *fp_;

  char buffer_[64 * 1024]; // 64KB

  size_t writtenBytes_;
};

} // namespace fileutil

} // namespace cp
