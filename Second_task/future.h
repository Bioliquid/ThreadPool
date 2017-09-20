#ifndef FUTURE
#define FUTURE

template<class T>
class future {
public:
	bool ready;
	T data;

	future() {
		ready = false;
	}
	T get() {
		return data;
	}
	void wait() {
		while (!ready);
	}
	bool isReady() {
		return ready;
	}
};

#endif