#pragma once

#include <string>
#include <iostream>

using namespace std;

#define PRINT std::cout<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"<<std::endl

class People
{
public:
	People() { PRINT; }
	virtual ~People() { PRINT; }//创建虚析构函数

	virtual void func() { PRINT; }
	void test13() { func(); }
};

class Man : public People
{
public:
	Man() { PRINT; }
	//Man(int i) { PRINT; }
	Man(int& i) { PRINT; }
	Man(int&& i) { PRINT; }
	~Man() { PRINT; }

	void func() override { PRINT; }
};

