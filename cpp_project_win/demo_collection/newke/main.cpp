#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

#define TEST

#ifdef HJ5FUNC
#include "HJ5.h"
#elif defined TEST

#endif

class A {
public:
	A(){}
	~A(){}
	int a = 0;
	int b = 1;
	virtual void func1() { cout << __FUNCTION__ << endl; }
private:
	int c = 2;
	void func2() { cout << __FUNCTION__ << endl; }
};

class B: public A {
public:
	B():A(){}
	~B(){}
	int a = 1;
	int b = 2;
	void func1() { cout << __FUNCTION__ << endl; }
private:
	int c = 3;
	void func2() { cout << __FUNCTION__ << endl; }

};

int main() {
#ifdef HJ5FUNC
	HJ5();
#elif defined TEST
	//父类指针不能访问子类private
	//A* b = new B();
	//b->func1();

	//按数组访问string
	//string in;
	//while (getline(cin, in)) {
	//	for (int i = 0; i < in.size(); i++) {
	//		in[i] = 't';
	//		cout << in[i] << endl;
	//	}
	//}

#endif
	return 0;
}
