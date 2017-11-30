#include <iostream>
#include <chrono>
#include "promise.h"
#include "future.h"
#include "map.h"

int main() {
	//for (int i = 0; i < 1000; ++i) {
	thread_pool pool(2);
	promise<int> p;
	p.setPool(&pool);

	future<long> f = Map(std::move(p.getFuture()), [](int ii) {
		return (long)(ii + 1);
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	pool.execute([&p]()  {
		p.set(0);
	});

	std::cerr << f.get() << std::endl;
	//}
	system("pause");
}