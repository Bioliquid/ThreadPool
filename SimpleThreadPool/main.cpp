#include "threadPool.h"

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

void thread2() {
	cout << "thread 2\n";
}

int main() {
	ThreadPool pool(10);

	pool.execute(thread1);
	pool.runAsync(thread2);
	
	this_thread::sleep_for(chrono::milliseconds(1));
	return 0;
}
