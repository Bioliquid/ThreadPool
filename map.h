#pragma once

#include <thread>
#include <iostream>
#include <memory>
#include <functional>
#include "thread_pool.h"
#include "promise.h"
#include "future.h"

template<typename T, typename F>
future<typename std::result_of<F(T)>::type> Map(thread_pool * pool, future<T> && futur, const F & func) {
    using ret_type = typename std::result_of<F(T)>::type;
    std::shared_ptr<promise<ret_type>> p = std::shared_ptr<promise<ret_type>>(new promise<ret_type>());
    std::shared_ptr<future<T>> futur_ptr(new future<T>(std::move(futur)));
    if(pool != nullptr) {
        pool->execute([futur_ptr, &func, p] {
            p->set(func(futur_ptr->get()));
        });
    } else {
        std::thread([futur_ptr, &func, p]{
            p->set(func(futur_ptr->get()));
        }).detach();
    }
    return std::move(p->getFuture());
}


template<typename T, typename F>
future<typename std::result_of<F(T)>::type> Map(future<T> &&futur, const F &func) {
    thread_pool *pool;
    if(futur.getPool() != nullptr) {
        pool = futur.getPool();
    } else if(thread_pool::current_pool != nullptr){
        pool = thread_pool::current_pool;
    } else {
        pool = nullptr;
    }
    return std::move(Map(pool, std::move(futur), func));
}