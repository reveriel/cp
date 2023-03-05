#include "thread.h"

#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>

namespace asd {
namespace detail {

pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }


class ThreadNameInitializer {
public:
  ThreadNameInitializer() {
    pthread_once(&ponce_, &ThreadNameInitializer::init);
  }

  static void init() {
    pthread_key_create(&key_, &ThreadNameInitializer::destructor);
  }

  static void destructor(void *x) {
    char *name = static_cast<char *>(x);
    delete[] name;
  }

  static pthread_once_t ponce_;
  static pthread_key_t key_;
};

struct ThreadData {
    using Func = Thread::Func;
    Func func_;
    std::string name_;
    pid_t *tid_;
    CountDownLatch *latch_;
    
    ThreadData(const Func &func, const std::string &name, pid_t *tid, CountDownLatch *latch)
        : func_(func), name_(name), tid_(tid), latch_(latch) {}
    
    void runInThread() {
        *tid_ = gettid();
        tid_ = nullptr;
        latch_->countDown();
        latch_ = nullptr;
    
        ThreadNameInitializer::destructor(pthread_getspecific(ThreadNameInitializer::key_));
        if (name_.empty()) {
        pthread_setspecific(ThreadNameInitializer::key_, nullptr);
        } else {
        char *name = new char[name_.size() + 1];
        memcpy(name, name_.c_str(), name_.size());
        name[name_.size()] = '\0';
        pthread_setspecific(ThreadNameInitializer::key_, name);
        }
    
        try {
        func_();
        Thread::currentThread()->setTid(0);
        } catch (const Exception &ex) {
        Thread::currentThread()->setTid(0);
        throw;
        } catch (const std::exception &ex) {
        Thread::currentThread()->setTid(0);
        throw;
        } catch (...) {
        Thread::currentThread()->setTid(0);
        throw;
        }
    }
    };

}

} // namespace detail

} // namespace asd
