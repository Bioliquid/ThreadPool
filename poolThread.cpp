#include "poolThread.h"

ThreadPool::ThreadPool(size_t numThreads) {
	isTerminated = false;

	threads = vector<unique_ptr<thread> >(numThreads);
	for (unique_ptr<thread>& worker : threads) {
		worker.reset(new thread(&ThreadPool::ThreadLoop, this));
	}
}

ThreadPool::~ThreadPool() {
	unique_lock<mutex> lockList(function_mutex);
	isTerminated = true;
	funcList.notify_all();

	for (unique_ptr<thread>& worker : threads) {
		worker->join();
	}
}


void ThreadPool::execute(function<void()> const &thread_func) {
	unique_lock<mutex> lockList(function_mutex);
	functions.push_back(function<void()>(thread_func));
	funcList.notify_one();
}

function<void()> ThreadPool::Wait_for_next_func() {
	unique_lock<mutex> lockList(function_mutex);

	while (!isTerminated) {
		if (!functions.empty()) {
			function<void()> job = functions.front();
			functions.pop_front();
			return job;
		} else {
			funcList.wait(lockList);
		}
	}
}

void ThreadPool::ThreadLoop() {
	while (!isTerminated) {
		function<void()> job = Wait_for_next_func();
		job();
	}
}