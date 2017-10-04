#include "future.h"
#include "promise.h"
#include "shared_state.h"
#include <iostream>
#include <thread>

void f1() {
	promise<int> p;
	int a = 5;
	p.set(a);
	a = 10;
	std::cout << p.getFuture().get() << std::endl;
}

int main() {
	std::thread thread1(f1);
	thread1.join();
	system("pause");
}