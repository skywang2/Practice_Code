#pragma once
#include <string>
#include <mutex>

std::mutex mux;

class Singleton {
public:
	//static Singleton* GetInstance();
	static Singleton* GetInstance(int p1, std::string p2);
	static void Destory(Singleton*);

private:
	Singleton(){};
	~Singleton(){};
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static Singleton* logger;//实例
	int param1;
	std::string param2;
};

Singleton* Singleton::logger = nullptr;//注意：static私有变量要在class外定义

//构造单例
Singleton* Singleton::GetInstance(int p1, std::string p2) {
	if (!logger) {
		mux.lock();
		if (!logger) {
			logger = new Singleton();
			logger->param1 = p1;
			logger->param2 = p2;
		}
		mux.unlock();
	}
	return logger;
}

//Singleton* Singleton::GetInstance() {
//	if (!logger) {
//		mux.lock();
//		if (!logger) {
//			logger = new Singleton();
//		}
//		mux.unlock();
//	}
//	return logger;
//}

//析构单例
void Singleton::Destory(Singleton*) {
	if (logger) {
		mux.lock();
		if (logger) delete logger;
		mux.unlock();
	}
}
