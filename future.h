#pragma once

#include "shared_state.h"

template<typename R>
class promise;
template<typename R>
class promise<R&>;
template<>
class promise<void>;


template<typename R>
class future {
private:

    friend class promise<R>;

    std::shared_ptr<shared_state<R>> state_ptr;

    explicit future(std::shared_ptr<shared_state<R>> s) {
        state_ptr = s;
    }
public:
    future() = default;

    future(future<R> && f) {
        state_ptr = std::move(f.state_ptr);
    }

    future& operator=(future<R> && f) {
        state_ptr = std::move(f.state_ptr);
        return *this;
    }

    R get() const {
        wait();
        if (state_ptr->isException) {
            std::rethrow_exception(state_ptr->exc);
        }
        return std::move(state_ptr->data);
    }

    bool isReady() const {
        return state_ptr->ready_;
    }

    void wait() const {
        if (state_ptr->ready_) {
            return;
        }
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        while (!state_ptr->ready_ && !state_ptr->promiseDestroyed) {
            state_ptr->cv.wait(lock);
        }
        if (state_ptr->promiseDestroyed && !state_ptr->ready_) {
            throw std::runtime_error("Promise destroyed");
        }
    }
};

template<typename R>
class future<R&> {
private:

    friend class promise<R&>;

    std::shared_ptr<shared_state<R&>> state_ptr = 0;

    explicit future(std::shared_ptr<shared_state<R&>> s) {
        state_ptr = s;
    }
public:
    future() = default;

    future(future<R&> && f) {
        state_ptr = std::move(f.state_ptr);
    }

    future& operator=(future<R&> && f) {
        state_ptr = std::move(f.state_ptr);
        return *this;
    }

    R get() const {
        wait();
        if (state_ptr->isException) {
            std::rethrow_exception(state_ptr->exc);
        }
        return state_ptr->data;
    }

    bool isReady() const {
        return state_ptr->ready_;
    }

    void wait() const {
        if (state_ptr->ready_) {
            return;
        }
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        while (!state_ptr->ready_ && !state_ptr->promiseDestroyed) {
            state_ptr->cv.wait(lock);
        }
        if (state_ptr->promiseDestroyed && !state_ptr->ready_) {
            throw std::runtime_error("Promise destroyed");
        }
    }
};

template<>
class future<void> {
private:

    friend class promise<void>;

    std::shared_ptr<shared_state<void>> state_ptr;

    explicit future(std::shared_ptr<shared_state<void>> s) {
        state_ptr = s;
    }
public:
    future() = default;

    future(future<void> && f) {
        state_ptr = std::move(f.state_ptr);
    }

    future& operator=(future<void> && f) {
        state_ptr = std::move(f.state_ptr);
        return *this;
    }

    void get() const {
        wait();
        if (state_ptr->isException) {
            std::rethrow_exception(state_ptr->exc);
        }
    }

    bool isReady() const {
        return state_ptr->ready_;
    }

    void wait() const {
        if (state_ptr->ready_) {
            return;
        }
        std::unique_lock<std::mutex> lock(state_ptr->mtx_);
        while (!state_ptr->ready_ && !state_ptr->promiseDestroyed) {
            state_ptr->cv.wait(lock);
        }
        if (state_ptr->promiseDestroyed && !state_ptr->ready_) {
            throw std::runtime_error("Promise destroyed");
        }
    }
};