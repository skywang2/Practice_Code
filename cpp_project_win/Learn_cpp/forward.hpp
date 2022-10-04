#pragma once
#include <string>
#include <iostream>

void TestProcess(const std::string& s)
{
	std::cout << "&" << std::endl;
}

void TestProcess(std::string&& s)
{
	std::cout << "&&" << std::endl;
}

//万能引用，只在模板函数和匿名函数（auto&&作为入参）情况下使用
//因为实例化要推导类型，所以写的是&&实际上可能是&左值引用
//因此要使用forward将param还原回正确的类型
template<typename T>
void MoreThanProcess(T&& param)
{
	//std::cout << __FUNCTION__ << std::endl;
	
	std::cout << "without std::forward" << std::endl;
	TestProcess(param);
	std::cout << "with std::forward" << std::endl;
	TestProcess(std::forward<T>(param));
}
