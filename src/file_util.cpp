#include "file_util.h"
#include <cassert>

namespace cp {

namespace fileutil {

AppendFile::AppendFile(const std::string &filename) {

  fp_ = fopen(filename.c_str(), "ae"); // append, e for O_CLOEXEC, so that child
                                       // process can not see the file
  assert(fp_);
  setbuffer(fp_, buffer_, sizeof(buffer_));
  writtenBytes_ = 0;
}

AppendFile::~AppendFile() { fclose(fp_); }

void AppendFile::append(const char *logline, size_t len) {

  size_t n = write(logline, len); // n is the number of bytes actually written
  size_t remain = len - n;
  while (remain > 0) {
    size_t x = write(logline + n, remain);
    if (x == 0) {
      int err = ferror(fp_);
      if (err) {
        fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
      }
      break;
    }
    n += x;
    remain = len - n;
  }

  writtenBytes_ += n;
}

void AppendFile::flush() {

  int err = ::fflush(fp_);
  if (err) {
    fprintf(stderr, "AppendFile::flush() failed %s\n", strerror(err));
  }
}

size_t AppendFile::write(const char *logline, size_t len) {
  return ::fwrite(logline, sizeof(char), len, fp_);
}

} // namespace fileutil

} // namespace cp
