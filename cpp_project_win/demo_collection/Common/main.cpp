#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include "common.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::thread;

#define READ_CHAR 0
#define VIRTUAL_DEF 1
#define TEMPLATE 0
#define THREAD_TEST 0

CRITICAL_SECTION cs = { 0 };

bool FourCharToInt32(int* output, char* input)
{
	return true;
}

bool ReadFromChar(char* input, size_t size)
{
	int temp = 0;
	temp = (int)input;
	return true;
}

template<typename T>
const T &mymax(const T &a, const T &b)
{
	return (a > b) ? a : b;
}

void func_temp(const string &in)
{
	//cout << "input:" << in << endl;
	char output[512] = { 0 };
	for (int i = 0; i < 30; i++)
	{
		EnterCriticalSection(&cs);
		//cout << "j:" << i;
		sprintf(output, "[%s:%d], pid:%ld, tid:%ld", __FUNCTION__, __LINE__, GetCurrentProcessId(), GetCurrentThreadId());
		cout << output << endl;
		LeaveCriticalSection(&cs);
		Sleep(50);
	}
}

int main(int argc, char* argv[])
{
#if READ_CHAR
	int testInput = 0x12345678;
	ReadFromChar((char*)testInput, sizeof(int));
#elif VIRTUAL_DEF
	cout << "=====A=====" << endl;
	A_child aChildObj;
	A* aObj = &aChildObj;
	aObj->print();

	cout << "=====B_child=====" << endl;
	B_child bChildObj;
	B* bObj = &bChildObj;
	bObj->print();

	cout << "=====B_child_child=====" << endl;
	B* bObj2 = new B_child_child();
	bObj2->print();
	delete bObj2;

	cout << "=====D=====" << endl;
	D dObj;
	dObj.print();
#elif TEMPLATE
	int t1 = 1, t2 = 2;
	string a1("111");
	string a2("222");

	cout << mymax(t1, t2) << endl;
	cout << mymax(a1, a2) << endl;
#elif THREAD_TEST
	InitializeCriticalSection(&cs);
	thread child(func_temp, "xxxxx");
	if (true == child.joinable())
		child.detach();

	char output[512] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		EnterCriticalSection(&cs);
		//cout << "i:" << i;
		sprintf(output, "[%s:%d], pid:%ld, tid:%ld", __FUNCTION__, __LINE__, GetCurrentProcessId(), GetCurrentThreadId());
		cout << output << endl;
		LeaveCriticalSection(&cs);
		Sleep(10);
	}

	DeleteCriticalSection(&cs);
#elif !defined(XXX)
	int a=0;
#endif
	return 0;
}
