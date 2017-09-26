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

void thread4() {
	throw 1;
}

void thread5() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "still thread 5\n";
	std::cout << "still thread 5\n";
}

int thread3(int a, int b) {
	return a + b;
}

int main() {
	ThreadPool pool(3);
	future_ptr<int> f = pool.runAsync(thread3, 2, 3);
	future_ptr<int> f1 = pool.runAsync(thread3, 3, 3);

	std::cout << "waiting for threads" << std::endl;

	f1->wait();
	std::cout << f1->get() << std::endl;
	f->wait();
	std::cout << f->get() << std::endl;
	system("pause");
}