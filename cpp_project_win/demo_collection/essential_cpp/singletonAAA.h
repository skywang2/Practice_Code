#pragma once
#include "singleton.h"

class SingletonAAA : public Singleton<SingletonAAA>
{
	friend class Singleton<SingletonAAA>;
public:
	virtual void Show(){ std::cout << "SingletonAAA" << std::endl; }
private:
	SingletonAAA() = default;
	SingletonAAA(const SingletonAAA&) = delete;
	SingletonAAA& operator=(const SingletonAAA&) = delete;
};
