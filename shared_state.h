#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename R>
class shared_state {
public:
    std::condition_variable cv;
    std::mutex mtx_;
    R data;
    bool isException;
    std::exception_ptr exc;
    std::atomic<bool> ready_, promiseDestroyed;

    shared_state() {
        ready_ = promiseDestroyed = isException = false;
    }
};

template<typename R>
class shared_state<R&> {
public:
    std::condition_variable cv;
    std::mutex mtx_;
    R *data;
    std::exception_ptr exc;
    bool isException;
    std::atomic<bool> ready_, promiseDestroyed;

    shared_state() {
        ready_ = promiseDestroyed = isException = false;
    }
};

template<>
class shared_state<void> {
public:
    std::condition_variable cv;
    std::mutex mtx_;
    std::exception_ptr exc;
    bool isException;
    std::atomic<bool> ready_, promiseDestroyed;

    shared_state() {
        ready_ = promiseDestroyed = isException = false;
    }
};