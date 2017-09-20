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
};

#endif
