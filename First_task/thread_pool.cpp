#include "thread_pool.h"

ThreadPool::ThreadPool(size_t numThreads = 1) {
	isTerminated = false;

	threads.reserve(numThreads);
<<<<<<< HEAD
	for (int i = 0; i < (int)numThreads; ++i) {
=======
	for (int i = 0; i < numThreads; ++i) {
>>>>>>> 4cb6259260a88fb90cff4a65b9e8e1371ce1d24e
		threads.emplace_back(&ThreadPool::ThreadLoop, this);
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lockList(function_mutex);
		isTerminated = true;
	}
	funcList.notify_all();

	for (std::thread& worker : threads) {
		if (worker.joinable()) {
			worker.join();
		}
	}
}

void ThreadPool::execute(std::function<void()> const &thread_func) {
	{
		std::unique_lock<std::mutex> lockList(function_mutex);
		functions.emplace(std::function<void()>(thread_func));
	}
	funcList.notify_one();
}

void ThreadPool::ThreadLoop() {
	while (!isTerminated) {
		std::unique_lock<std::mutex> lockList(function_mutex);
		funcList.wait(lockList, [&]() {
			return !functions.empty() || isTerminated;
		});
		if (!functions.empty()) {
			std::function<void()> job = functions.front();
			functions.pop();
			lockList.unlock();
			try {
				job();
			} catch (...) {

			}
			
		}
	}
}
