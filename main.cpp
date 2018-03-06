#include <iostream>
#include "promise.h"
#include "future.h"
#include "map.h"
#include "flattenTuple.h"

template<typename T>
void print_el(T t) {
    std::cout << t << ' ';
}

void print_els() {}

template<typename T, typename ...Args>
void print_els(T &&t, Args&&... args) {
    print_el(std::forward<T>(t));

    print_els(std::forward<Args>(args)...);
};

template<typename Tuple, size_t... Is>
constexpr auto print_impl(Tuple t, std::index_sequence<Is...>) {
    print_els(std::get<Is>(t)...);
}

template<typename Tuple>
constexpr auto print(Tuple t) {
    print_impl(t, std::make_index_sequence<std::tuple_size<Tuple>{}>{});
}

int main() {
    thread_pool pool(10);
    std::vector<int> v {1, 2, 3, 4, 5};
    pool.parallel(v.begin(), v.end(), [](int ii){ return ii*ii; });
    for (int &el : v) {
        std::cout << el << ' ';
    }
}