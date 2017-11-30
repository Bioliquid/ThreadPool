//
// Created by iksiygrik on 16/11/17.
//

#ifndef PLUSIKI_THREAD_POOL_H
#define PLUSIKI_THREAD_POOL_H

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

class thread_pool {
private:
	void thread_loop();

	std::vector<std::thread> threads;
	std::queue<std::function<void()> > functions;

	std::mutex function_mutex;
	std::condition_variable funcList;
	std::atomic<bool> isTerminated;
public:
	thread_pool(size_t);
	virtual ~thread_pool();
	void execute(std::function<void()> const&);
	static __declspec(thread) thread_pool* current_pool;
};

#endif //PLUSIKI_THREAD_POOL_H
