#include <iostream>
#define MYDLL_EXPORT
#include "MyDll.h"

void MyDllFunc1()
{
	std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "], using __declspec(dllexport)" << std::endl;
}

void MyDllFunc2()
{
	std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "], not using __declspec(dllexport)" << std::endl;
}
