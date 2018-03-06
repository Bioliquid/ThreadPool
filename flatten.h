#pragma once

#include <tuple>
#include "future.h"
#include "promise.h"

template<typename T>
struct nested_type {
    typedef T type;
};

template<typename T>
struct nested_type<future<T>> {
    typedef T type;
};

template<typename T>
struct nested_type<future<future<T>>> {
    typedef typename nested_type<T>::type type;
};

template<typename T>
T FlattenImpl(T t) {
    return t;
}

template<typename T>
T FlattenImpl(const future<T> & f) {
    return std::move(f.get());
}

template<typename T>
auto FlattenImpl(const future<future<T>> & f) {
    return std::move(FlattenImpl(std::move(f.get())));
}

template<typename T>
future<typename nested_type<T>::type> Flatten(const future<future<T>> & f) {
    //using return_type = typename nested_type_Getter<future<T>>::type_t;
    std::shared_ptr<promise<typename nested_type<future<T>>::type>> p(new promise<typename nested_type<future<T>>::type>());
    std::thread([p, &f](){
        p->set(std::move(FlattenImpl(f).get()));
    }).detach();
    return std::move(p->getFuture());
}

template<typename T>
future<T> Flatten(const future<T> & f) {
    std::shared_ptr<promise<T>> p(new promise<T>());
    std::thread([p, &f](){
        p->set(std::move(f.get()));
    }).detach();
    return std::move(p->getFuture());
}

template<template<typename, typename...> class C, typename T>
future<C<T>> Flatten(C<future<T>> const & col) {
    std::shared_ptr<promise<C<T>>> p(new promise<C<T>>());
    std::thread([p, &col](){
        C<T> output = C<T>();
        for (const future<T> & f : col) {
            output.insert(std::end(output), f.get());
        }
        p->set(output);
    }).detach();
    return std::move(p->getFuture());
}


template<typename ...Args>
struct nested_type<std::tuple<Args...>> {
    typedef std::tuple<typename nested_type<Args>::type...> type;
};

template<typename ...Args, std::size_t... I>
auto flatten_impl_tuple(const std::tuple<Args...> &tup, std::index_sequence<I...>) {
    return std::make_tuple(FlattenImpl(std::get<I>(tup))...);
}

template<typename ...Args, typename Indices = std::make_index_sequence<sizeof...(Args)>>
auto flattenTuple(std::tuple<Args...> tupl) {
    using type_ = typename nested_type<std::tuple<Args...>>::type;
    std::shared_ptr<promise<type_>> promise_ptr(new promise<type_>);
    std::thread([promise_ptr, &tupl]() {
        auto temp = flatten_impl_tuple(tupl, Indices{});
        promise_ptr->set(temp);
    }).detach();
    return std::move(promise_ptr->getFuture());
}