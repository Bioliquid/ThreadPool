#ifndef FUTURE
#define FUTURE

#include "shared_state.h"

template<typename R>
class Promise;
template<typename R>
class Promise<R&>;
template<>
class Promise<void>;


template<typename R>
class Future {
private:

	friend class Promise<R>;

	shared_state<R> *state_ptr = 0;

	explicit Future(shared_state<R> *ptr) {
		state_ptr = ptr;
	}
public:
	Future(Future && other) {
		state_ptr = std::move(other.state_ptr);
	}
	Future& operator=(Future && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	Future(Future const &) = delete;
	Future &operator= (Future const &) = delete;
	Future() = default;

	R Get() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->error){
			std::rethrow_exception(state_ptr->error);
		}
		wait();
		return state_ptr->data;
	}

	bool IsReady() const {
		return state_ptr->ready_;
	}

	void wait() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->ready_) {
			return;
		}
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		while (!state_ptr->ready_) {
			state_ptr->cv.wait(lock);
		}
	}
};

template<typename R>
class Future<R&> {
private:

	friend class Promise<R&>;

	shared_state<R&> *state_ptr = 0;

	explicit Future(shared_state<R&> *ptr) {
		state_ptr = ptr;
	}
public:
	Future(Future && other) {
		state_ptr = std::move(other.state_ptr);
	}
	Future& operator=(Future && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	Future(Future const &) = delete;
	Future &operator= (Future const &) = delete;
	Future() = default;

	R & Get() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->error){
			std::rethrow_exception(state_ptr->error);
		}
		wait();
		return *state_ptr->data;
	}

	bool IsReady() const {
		return state_ptr->ready_;
	}

	void wait() const {
		if (state_ptr->ready_) {
			return;
		}
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		while (!state_ptr->ready_) {
			state_ptr->cv.wait(lock);
		}
	}
};

template<>
class Future<void> {
private:

	friend class Promise<void>;

	shared_state<void> *state_ptr = 0;

	explicit Future(shared_state<void> *ptr) {
		state_ptr = ptr;
	}
public:
	Future(Future && other) {
		state_ptr = std::move(other.state_ptr);
	}
	Future& operator=(Future && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	Future(Future const &) = delete;
	Future &operator= (Future const &) = delete;
	Future() = default;

	void Get() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->error){
			std::rethrow_exception(state_ptr->error);
		}
		wait();
	}

	bool IsReady() const {
		return state_ptr->ready_;
	}

	void wait() const {
		if (state_ptr->ready_) {
			return;
		}
		std::unique_lock<std::mutex> lock(state_ptr->mtx_);
		while (!state_ptr->ready_) {
			state_ptr->cv.wait(lock);
		}
	}
};

#endif //Future