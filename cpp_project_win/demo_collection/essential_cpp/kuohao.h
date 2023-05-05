#pragma once
#include <iostream>

using namespace std;

//function object函数对象
class MyClass
{
public:
	MyClass();
	~MyClass();

	void operator()(int x) {
		cout << "x:" << x << endl;
	}

private:

};

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}
