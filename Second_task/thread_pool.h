#ifndef POOLTHREAD
#define POOLTHREAD

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include "future.h"

class ThreadPool {
private:
	void ThreadLoop();

	std::vector<std::thread> threads;
	std::queue<std::function<void()> > functions;

	std::mutex function_mutex;
	std::condition_variable funcList;
	std::atomic<bool> isTerminated;
public:
	ThreadPool(size_t);
	virtual ~ThreadPool();
	void execute(std::function<void()> const&);

	template<class r, class fn, class... args>
	std::shared_ptr<future<r>> runAsync(std::shared_ptr<future<r>> &f, fn _fn, args... _args) {
		std::function<r()> ex_func = std::bind(_fn, _args...);
		std::shared_ptr<future<r>> ft(new future<r>());
		std::function<void()> func = [=]() {
			ft->data = ex_func();
			ft->ready = true;
		};
		execute(func);
		f = ft;
		return ft;
	}
};

#endif
