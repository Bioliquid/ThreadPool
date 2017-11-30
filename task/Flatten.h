#include "future.h"
#include "promise.h"

template<typename T>
struct nested_type;


template<typename T>
struct nested_type<future<T>> {
	typedef T type;
};

template<typename T>
struct nested_type<future<future<T>>> {
	typedef typename nested_type<future<T>>::type type;
};

template<typename T>
future<T> FlattenImpl(const future<T> & f) {
	return std::move(f);
}

template<typename T>
future<typename nested_type<T>::type> FlattenImpl(const future<future<T>> & f) {
	return FlattenImpl(f.Get());
}

template<typename T>
future<typename nested_type<T>::type> Flatten(const future<future<T>> & f) {
	//using return_type = typename nested_type_Getter<future<T>>::type_t;
	std::shared_ptr<promise<typename nested_type<future<T>>::type>> p(new promise<typename nested_type<future<T>>::type>());
	std::thread([p, &f](){
		p->Set(std::move(FlattenImpl(f).Get()));
	}).detach();
	return std::move(p->Getfuture());
}

template<typename T>
future<T> Flatten(const future<T> & f) {
	std::shared_ptr<promise<T>> p(new promise<T>());
	std::thread([p, &f](){
		p->Set(std::move(f.Get()));
	}).detach();
	return std::move(p->Getfuture());
}

template<template<typename, typename...> class C, typename T>
future<C<T>> Flatten(C<future<T>> const & col) {
	std::shared_ptr<promise<C<T>>> p(new promise<C<T>>());
	std::thread([p, &col](){
		C<T> output = C<T>();
		for (const future<T> & f : col) {
			output.insert(std::end(output), f.Get());
		}
		p->Set(output);
	}).detach();
	return std::move(p->Getfuture());
}