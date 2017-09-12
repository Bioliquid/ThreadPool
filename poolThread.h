#ifndef POOLTHREAD
#define POOLTHREAD

#include <thread>
#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
using namespace std;

class ThreadPool {
private:
	void ThreadLoop();
	function<void()> Wait_for_next_func();

	vector<unique_ptr<thread> > threads;
	list<function<void()> > functions;

	mutex function_mutex;
	condition_variable funcList;
	atomic<bool> isTerminated;
public:
	ThreadPool(size_t numThreads);
	virtual ~ThreadPool();

	void execute(function<void()> const&);

	template<class _FN, class... _ARGS>
	void runAsync(_FN _fn, _ARGS... _args) {
		execute(bind(_fn, _args...));
	}
};

#endif