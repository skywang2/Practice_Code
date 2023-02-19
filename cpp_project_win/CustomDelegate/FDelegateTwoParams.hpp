#pragma once
#include <vector>
#include <list>
#include <functional>

class TestA;
typedef int (TestA::* twoParam)(int, int);//类成员函数的指针
using pFunNoParam = void (TestA::*)();//使用using形式的成员函数指针

typedef int (*Func_Ret_TwoParams)(int, int);//全局函数的指针

//直接定义代理类型，不具有拓展性
class FDelegateTwoParams
{
public:
	void BindGlobalFunc(Func_Ret_TwoParams pFunc, int a, int b)
	{
		func = std::bind(pFunc, a, b);
	}

	void AddRaw(TestA* UserClass, twoParam FunPtr, int a, int b)
	{
		funcs.push_back(std::bind(FunPtr, UserClass, a, b));
	}

	bool IsBound()
	{
		return func ? true : false;
	}

	void Execute()
	{
		func();
	}

	void ExecuteIfBound()
	{
		if (IsBound())
		{
			Execute();
		}
	}

	void BroadCast()
	{
		for (auto f : funcs)
		{
			f();
		}
	}
private:
	std::function<int()> func;
	std::list<std::function<int()>> funcs;
};

//使用宏生成代理类型

//使用宏+模板生成代理类型


