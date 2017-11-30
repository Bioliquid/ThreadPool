//
// Created by iksiygrik on 15/11/17.
//

#ifndef PLUSIKI_SHARED_STATE_H
#define PLUSIKI_SHARED_STATE_H

#ifndef SHAREDSTATE
#define SHAREDSTATE

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "thread_pool.h"

template<typename R>
class shared_state {
public:
	std::condition_variable cv;
	std::mutex mtx_;
	R data;
	bool isException;
	std::exception_ptr exc;
	thread_pool *pool = nullptr;
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
	thread_pool *pool = nullptr;
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
	thread_pool *pool = nullptr;
	bool isException;
	std::atomic<bool> ready_, promiseDestroyed;

	shared_state() {
		ready_ = promiseDestroyed = isException = false;
	}
};

#endif //SHAREDSTATE

#endif //PLUSIKI_SHARED_STATE_H
