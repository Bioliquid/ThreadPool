#ifndef PROMISE
#define PROMISE

#include <iostream>
#include "shared_state.h"
#include "future.h"

template<typename R>
class promise {
private:
	shared_state<R> *state_ptr;
public:
	promise(promise && other) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr = std::move(other.state_ptr);
	}
	promise &operator= (promise && other) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	future<R> getFuture() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		return future<R>(state_ptr);
	}

	promise() {
		state_ptr = new shared_state<R>();
	}

	void set(const R & value) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->data = value;
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}

	void set(const R && value) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->data = std::move(value);
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}

	void setException(const std::exception_ptr & e) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr->error = e;
	}
};

template<typename R>
class promise<R&>{
private:
	shared_state<R&> *state_ptr;
public:
	promise(promise && other) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr = std::move(other.state_ptr);
	}
	promise &operator= (promise && other) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	future<R&> getFuture() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		return future<R&>(state_ptr);
	}

	promise() {
		state_ptr = new shared_state<R&>();
	}

	void set(R & value) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->data = &value;
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}
	void setException(const std::exception_ptr & e) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr->error = e;
	}
};

template<>
class promise<void>{
private:
	shared_state<void> *state_ptr;
public:
	promise(promise && other) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr = std::move(other.state_ptr);
	}
	promise &operator= (promise && other) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	future<void> getFuture() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		return future<void>(state_ptr);
	}

	promise() {
		state_ptr = new shared_state<void>();
	}

	void set() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}

	void setException(const std::exception_ptr & e) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr->error = e;
	}
};

#endif //PROMISE