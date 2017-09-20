#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include "future.h"

template<typename T>
class promise {
	std::shared_ptr<future<T>> fut;
public:
	promise() : fut(new future<T>) {}
	promise(promise<T> && p) {
		fut = p.fut;
	}
	std::shared_ptr<future<T>> getFuture() {
		return fut;
	}
	void set(const T& os_func) {
		std::unique_lock<std::mutex> l(fut->futex);
		fut->data = os_func;
		fut->ready = true;
		fut->condutex.notify_all();
	}
	void set(T&& os_func) {
		std::unique_lock<std::mutex> l(fut->futex);
		fut->data = os_func;
		fut->ready = true;
		fut->condutex.notify_all();
	}
};