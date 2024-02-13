#pragma once

namespace cp {

class noncopyable {
public:
  noncopyable() = default;
  ~noncopyable() = default;
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;
};

} // namespace cp