#pragma once
#include <vector>
#include <list>
#include <functional>

class TestA;
typedef int (TestA::* twoParam)(int, int);//类成员函数的指针
using noParam = void (TestA::*)(void);//使用using形式的成员函数指针

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

//使用宏声明代理类型
#define DECLARE_DELEGATE(DelegateName)\
class My##DelegateName\
{\
public:\
	void BindRaw(TestA* UserClass, noParam FunPtr) { func = std::bind(FunPtr, UserClass); }\
	bool IsBound() { return func ? true : false; }\
	void Execute() { func(); }\
private:\
	std::function<void()> func;\
};

//使用宏+模板声明代理类型
//用模板定义函数指针，由于模板只能用在函数、类、结构体上，所以要用结构体把typedef包起来

template<typename ClassName, typename RetValType>//该声明是为了确保同名模板结构在使用可变参数时，结束变量递归处理
struct TFuncPtrType;

template<typename ClassName, typename RetValType, typename... ArgTypes>
struct TFuncPtrType<ClassName, RetValType(ArgTypes...)>
{
	typedef RetValType (ClassName::* FuncPtr)(ArgTypes...);
};

template<typename RetValType, typename... ParamTypes>
class TBaseDelegate
{
public:
	template<typename UserClass>
	void BindRaw(UserClass* obj,
		typename TFuncPtrType<UserClass, RetValType(ParamTypes...)>::FuncPtr pFunc,
		ParamTypes... Vars)
	{
		func = std::bind(pFunc, obj, Vars...);
	}

	bool IsBound()
	{
		return func ? true : false;
	}

	void Execute()
	{
		func();
	}
private:
	std::function<RetValType()> func;
};

//用继承的方式使用宏生成代理类型
#define DECLARE_DELEGATE_NOPARAM(DelegateName) class DelegateName : public TBaseDelegate<void> {};

