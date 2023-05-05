#pragma once
#include <iostream>
#include <string>
#include <mutex>

template<typename T>
class Singleton
{
public:
	static T* GetInstance();
	virtual void Show(){ std::cout << "Singleton" << std::endl; }
protected:
	Singleton(){}
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
private:
	static T* instance;
	static std::mutex m_mtx;
};

template<typename T>
T* Singleton<T>::instance = nullptr;

template<typename T>
std::mutex Singleton<T>::m_mtx;

template<typename T>
T* Singleton<T>::GetInstance()
{
	if (!instance)
	{
		std::lock_guard<std::mutex> lk(m_mtx);
		if (!instance) instance = new T();
	}
	return instance;
}
