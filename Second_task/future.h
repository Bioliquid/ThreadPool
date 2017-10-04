#ifndef FUTURE
#define FUTURE

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

	shared_state<R> *state_ptr = 0;

	explicit future(shared_state<R> *ptr) {
		state_ptr = ptr;
	}
public:
	future(future && other) {
		state_ptr = std::move(other.state_ptr);
	}
	future& operator=(future && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	future(future const &) = delete;
	future &operator= (future const &) = delete;
	future();

	R get() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->error){
			std::rethrow_exception(state_ptr->error);
		}
		wait();
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
		while (!state_ptr->ready_) {
			state_ptr->cv.wait(lock);
		}
	}
};

template<typename R>
class future<R&> {
private:

	friend class promise<R&>;

	shared_state<R&> *state_ptr = 0;

	explicit future(shared_state<R&> *ptr) {
		state_ptr = ptr;
	}
public:
	future(future && other) {
		state_ptr = std::move(other.state_ptr);
	}
	future& operator=(future && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	future(future const &) = delete;
	future &operator= (future const &) = delete;
	future();

	R & get() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->error){
			std::rethrow_exception(state_ptr->error);
		}
		wait();
		return *state_ptr->data;
	}

	bool isReady() const {
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
class future<void> {
private:

	friend class promise<void>;

	shared_state<void> *state_ptr = 0;

	explicit future(shared_state<void> *ptr) {
		state_ptr = ptr;
	}
public:
	future(future && other) {
		state_ptr = std::move(other.state_ptr);
	}
	future& operator=(future && other) {
		if (this != &other) {
			state_ptr = std::move(other.state_ptr);
		}
		return *this;
	}

	future(future const &) = delete;
	future &operator= (future const &) = delete;
	future();

	void get() const {
		if (state_ptr == 0){
			throw std::runtime_error("value is not set and promise doesn't exist");
		}
		if (state_ptr->error){
			std::rethrow_exception(state_ptr->error);
		}
		wait();
	}

	bool isReady() const {
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

#endif //FUTURE