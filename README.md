# cp



# develooped on mac

enable coredump on mac

not surekj
```bash
sudo sysctl kern.coredump=1
sudo sysctl -w kern.corefile=core.%N.%P.%t
sudo launchctl limit core unlimited
```

# 1 basic

## 1.1 condition variable

condition_variable is used with std::mutex, to block one or more threads util a shared variable (the condition), and notifies the condition variable

usage:

the thread that intends to modify the shared variable must:

1. acquire a mutex
2. modify the shared variable
3. call notify_one or notify_all on cv

the therad that intends to wait on a condition variable must:

1. acquire the mutex
2. while (condition is not satisfied)
    2.1. call wait, wait_for, or wait_until, on cv. (atomically release the mutex, suspend until notified or timeout, reacquire the mutex)
or.

1. use the predicated overload of wait


```cpp
std::mutex m;
std::condition_variable cv
std::string data;
bool ready = false;
bool processed = false;

void worker_thread() {
    // wait util main() sends data
    std::unique_lock lk(m);
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

# 1.2 blocking queue

# 1.3 countdown latch


# 1.4 barrier




# 1. logging

depends on blocking queue


