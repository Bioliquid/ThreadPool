#include "Flatten.h"

template<typename ...Args, std::size_t... I>
auto flatten_impl_tuple(const std::tuple<Args...> &tup, std::index_sequence<I...>) {
	return std::make_tuple(flatten_impl(std::get<I>(tup))...);
}

template<typename ...TParam, typename Indices = std::make_index_sequence<sizeof...(TParam)>>
auto flattenTuple(std::tuple<TParam...> tuple) {
	using name = typename nested_type_promise<std::tuple<TParam...>>::t_t;
	std::shared_ptr<Promise<name>> ptr(new Promise<name>);
	std::thread([ptr, &tuple]() {
		auto temp = flatten_impl_tuple(tuple, Indices{});
		ptr->set(temp);
	}).detach();
	return std::move(ptr->getFuture());
}