#ifndef SHAREDSTATE
#define SHAREDSTATE

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <iostream>

template<typename R>
class shared_state {
public:
	std::condition_variable cv;
	std::mutex mtx_;
	R data;
	std::atomic_bool ready_;
	std::exception_ptr error;

	shared_state() {
		ready_ = false;
	}
};

template<typename R>
class shared_state<R&> {
public:
	std::condition_variable cv;
	std::mutex mtx_;
	R *data;
	std::atomic_bool ready_;
	std::exception_ptr error;

	shared_state() {
		ready_ = false;
	}
};

template<>
class shared_state<void> {
public:
	std::condition_variable cv;
	std::mutex mtx_;
	std::atomic_bool ready_;
	std::exception_ptr error;

	shared_state() {
		ready_ = false;
	}
};

#endif //SHAREDSTATE