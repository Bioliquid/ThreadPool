#include "poolThread.h"

void thread1() {
	cout << "thread 1\n";
	cout << "still thread 1\n";
	cout << "still thread 1\n";
	cout << "still thread 1\n";
	cout << "still thread 1\n";
	cout << "still thread 1\n";
	this_thread::sleep_for(chrono::milliseconds(100));
	cout << "still thread 1\n";
	cout << "still thread 1\n";
	cout << "still thread 1\n";
	cout << "still thread 1\n";
}

void thread2(int a) {
	cout << "thread 2\n";
	cout << a << endl;
}

int main() {
	auto start = std::chrono::system_clock::now();
	ThreadPool pool(10);

	pool.execute(thread1);
	pool.runAsync(thread2, 2, 2);

	auto end = std::chrono::system_clock::now();
	chrono::duration<double> diff = end - start;
	
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << diff.count() << endl;
	return 0;
}