//
// Created by iksiygrik on 16/11/17.
//

#include "thread_pool.h"

__declspec(thread) thread_pool *thread_pool::current_pool = nullptr;

thread_pool::thread_pool(size_t numThreads = 1) {
	isTerminated = false;

	threads.reserve(numThreads);
	for (int i = 0; i < (int)numThreads; ++i) {
		threads.emplace_back(&thread_pool::thread_loop, this);
	}
}

thread_pool::~thread_pool() {
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

void thread_pool::execute(std::function<void()> const &thread_func) {
	{
		std::unique_lock<std::mutex> lockList(function_mutex);
		functions.emplace(std::function<void()>(thread_func));
	}
	funcList.notify_one();
}

void thread_pool::thread_loop() {
	current_pool = this;
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
			}
			catch (...) {

			}

		}
	}
}