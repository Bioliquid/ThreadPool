#include "thread_pool.h"
#include "future.h"

template<class T>
using future_ptr = std::shared_ptr<future<T>>;

void thread1() {
	std::cout << "thread 1\n";
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
	std::cout << "still thread 1\n";
}

void thread2() {
	std::cout << "thread 2\n";
}

int main() {
	ThreadPool pool(2);

	pool.execute(thread1);
	/*pool.execute([]() {
		cout << "thread 2\n";
	});*/
	pool.execute(thread2);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}