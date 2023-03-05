#pragma once

#include <functional>
#include <string>

class Thread {

public:
  using Func = std::function<void()>;

  explicit Thread(Func func, const std::string &name = std::string());
  ~Thread();

  void start();
  int join();
  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  const std::string &name() const { return name_; }

private:
  bool started_;
  bool joined_;
  pthread_t pthreadId_;
  pid_t tid_;
  Func func_;
  std::string name_;

  static void *startThread(void *thread);
  void runInThread();
};