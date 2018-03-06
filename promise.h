#pragma once

#include "shared_state.h"
#include "future.h"

template<typename R>
class promise {
private:
    std::shared_ptr<shared_state<R>> state_ptr;
public:

    promise() : state_ptr(new shared_state<R>()) {}
    promise(promise<R> && p) {
        state_ptr = std::move(p.state_ptr);
    }
    ~promise() {
        state_ptr->promiseDestroyed = true;
        state_ptr->cv.notify_all();
    }

    future<R> getFuture() {
        return future<R>(state_ptr);
    }

    void set(const R & value) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting value multiple times");
        }
        state_ptr->data = value;
        state_ptr->ready_ = true;
        state_ptr->cv.notify_one();
    }

    void set(R && value) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting value multiple times");
        }
        state_ptr->data = std::move(value);
        state_ptr->ready_ = true;
        state_ptr->cv.notify_one();
    }
    void setError(const std::exception_ptr & ptr) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting error multiple times");
        }
        state_ptr->isException = true;
        state_ptr->exc = ptr;
        state_ptr->ready_ = true;
        state_ptr->cv.notify_all();
    }
};

template<typename R>
class promise<R&>{
private:
    std::shared_ptr<shared_state<R&>> state_ptr;
public:
    promise() : state_ptr(new shared_state<R&>()) {}
    promise(promise<R&> && p) {
        state_ptr = std::move(p.state_ptr);
    }
    ~promise() {
        state_ptr->promiseDestroyed = true;
        state_ptr->cv.notify_all();
    }

    future<R> getFuture() {
        return future<R>(state_ptr);
    }

    void set(const R & value) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting value multiple times");
        }
        state_ptr->data = value;
        state_ptr->ready_ = true;
        state_ptr->cv.notify_one();
    }

    void set(R && value) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting value multiple times");
        }
        state_ptr->data = std::move(value);
        state_ptr->ready_ = true;
        state_ptr->cv.notify_one();
    }
    void setError(const std::exception_ptr & ptr) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting error multiple times");
        }
        state_ptr->isException = true;
        state_ptr->exc = ptr;
        state_ptr->ready_ = true;
        state_ptr->cv.notify_all();
    }
};

template<>
class promise<void>{
private:
    std::shared_ptr<shared_state<void>> state_ptr;
public:
    promise() : state_ptr(new shared_state<void>()) {}
    promise(promise<void> && p) {
        state_ptr = p.state_ptr;
    }
    ~promise() {
        state_ptr->promiseDestroyed = true;
        state_ptr->cv.notify_all();
    }

    future<void> getFuture() {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        return future<void>(state_ptr);
    }

    void set() {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting value multiple times");
        }
        state_ptr->ready_ = true;
        state_ptr->cv.notify_one();
    }
    void setError(const std::exception_ptr & ptr) {
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        if (state_ptr->ready_) {
            throw std::runtime_error("setting error multiple times");
        }
        state_ptr->isException = true;
        state_ptr->exc = ptr;
        state_ptr->ready_ = true;
        state_ptr->cv.notify_all();
    }
};