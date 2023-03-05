#pragma once

namespace cp {

class noncopyable {
public:
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;
};

} // namespace cp