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
#include "promise.h"

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

	template<class F, class... Args>
	std::shared_ptr<future<typename std::result_of<F(Args...)>::type>> runAsync(F&& _fn, Args&&... _args) {
		using r = std::result_of<F(Args...)>::type;
		std::function<r()> ex_func = std::bind(_fn, _args...);
		std::shared_ptr<promise<r>> ft(new promise<r>);
		std::function<void()> func = [=]() {
			ft->set(ex_func());
		};
		execute(func);
		return ft->getFuture();
	}
};

#endif
