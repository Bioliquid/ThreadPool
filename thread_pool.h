#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include "future.h"
#include "promise.h"

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
    static thread_local thread_pool* current_pool;

    template <typename T, typename F>
    void parallel(T a, T b, F f) {
        std::vector<future<void>> cv;
        for (T c = a; c != b; ++c) {
            if(c+1 == b) {
                *c = f(*c);
            } else {
                std::shared_ptr<promise<void>> p(new promise<void>);
                cv.push_back(std::move(p->getFuture()));
                execute([c, p, &f](){
                    *c = f(*c);
                    p->set();
                });
            }
        }
        for(size_t i = 0; i < cv.size(); i++) {
            cv[i].wait();
        }
    }
};