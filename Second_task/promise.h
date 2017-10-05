#ifndef PROMISE
#define PROMISE

#include <iostream>
#include "shared_state.h"
#include "future.h"

template<typename R>
class Promise {
private:
	shared_state<R> *state_ptr;
public:
	Promise(Promise && other) {
		state_ptr = std::move(other.state_ptr);
	}
	Promise &operator= (Promise && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	Future<R> GetFuture() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		return Future<R>(state_ptr);
	}

	Promise() {
		state_ptr = new shared_state<R>();
	}

	void Set(const R & value) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->data = value;
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}

	void Set(const R && value) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->data = std::move(value);
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}

	void SetException(const std::exception_ptr & e) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr->error = e;
	}
};

template<typename R>
class Promise<R&>{
private:
	shared_state<R&> *state_ptr;
public:
	Promise(Promise && other) {
		state_ptr = std::move(other.state_ptr);
	}
	Promise &operator= (Promise && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	Future<R&> GetFuture() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		return Future<R&>(state_ptr);
	}

	Promise() {
		state_ptr = new shared_state<R&>();
	}

	void Set(R & value) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already set");
		}
		state_ptr->data = &value;
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}
	void SetException(const std::exception_ptr & e) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr->error = e;
	}
};

template<>
class Promise<void>{
private:
	shared_state<void> *state_ptr;
public:
	Promise(Promise && other) {
		state_ptr = std::move(other.state_ptr);
	}
	Promise &operator= (Promise && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	Future<void> GetFuture() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		return Future<void>(state_ptr);
	}

	Promise() {
		state_ptr = new shared_state<void>();
	}

	void Set() {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		if (state_ptr->ready_) {
			throw std::runtime_error("value already Set");
		}
		state_ptr->ready_ = true;
		state_ptr->cv.notify_one();
	}

	void SetException(const std::exception_ptr & e) {
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		state_ptr->error = e;
	}
};

#endif //Promise