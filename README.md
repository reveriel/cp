# cp

A Commentted (Copied) rPc framework of [muduo](https://github.com/muduokun/muduo).


# develooped on mac

Enable coredump on macOS is complicated ( I never success). Linux is recommended.


# 1 basic

## 1.1 condition variable

condition_variable is used with std::mutex, to block one or more threads util a shared variable (the condition) is satisfied, and notifies the condition variable

usage:

the thread that intends to modify the shared variable must:

1. acquire a mutex
2. modify the shared variable
   2.1 unlock the mutex
3. call notify_one or notify_all on cv ( (can be done after releasing the lock))

标准居然说 "can be done after releasing the lock" 这种不确定的说法。。

这种要求调用方严格做这么多步骤, 真的不算好设计，很容易用错啊，而且记不住.


the therad that intends to wait on a condition variable must:

1. acquire the mutex
2. while (condition is not satisfied)
    2.1. call wait, wait_for, or wait_until, on cv. (atomically release the mutex, suspend until notified or timeout, reacquire the mutex)
or.

1. use the predicated overload of wait

这里等待方调用了 lock， 却没有显式地调用 unlock, 需要注意。

wait 依赖 unique_lock, 以及 unlock 不是传入的参数，也不灵活。


```cpp
std::mutex m;
std::condition_variable cv
std::string data;
bool ready = false;
bool processed = false;

void worker_thread() {
    // wait util main() sends data
    std::unique_lock lk(m); // this line locks the mutex
    cv.wait(lk, []{ return ready;});

    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

int main()
{
    std::thread worker(worker_thread);

    data = "Example data";
    {
        std::lock_guard lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    // wait for the worker
    {
        std::unique_lock lk(m);
        cv.wait(lk, [] {return processed; });
    }
    std::cout << "Back in main(). data = " << data << '\n';
    worker.join();
}
```

Note:

1. unlock before notify, why? because the doc says so.

I've tried not following the doc, but it still works. Maybe I'll know why after I implement my pthread.

为什么这么麻烦？ cv 这个 api 感觉还是太底层太复杂了.

# 1.2 blocking queue

Easy, one condition variable

good chance to use what you just learned.

# 1.3 countdown latch

Easy. one condidtion variable

It's important to see the test code.

# 1.4. logging

the first a application

