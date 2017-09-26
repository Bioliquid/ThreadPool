#ifndef FUTURE
#define FUTURE

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

template<class T>
class future {
public:
	bool ready;
	T data;
	std::mutex futex;
	std::condition_variable condutex;

	future() {
		ready = false;
	}
	future(const future<T>& new_future) {
		ready = new_future.ready;
		data = new_future.data;
		futex = new_future.futex;
		condutex = new_future.condutex;
	}
	future(future<T>&& new_future) {
		ready = std::move(new_future.ready);
		data = std::move(new_future.data);
		futex = std::move(new_future.futex);
		condutex = std::move(new_future.condutex);
	}
	T get() {
		return data;
	}
	void wait() {
		std::unique_lock<std::mutex> lockList(futex);
		condutex.wait(lockList, [&]() {
			return ready;
		});
	}
	bool isReady() {
		return ready;
	}
};

#endif