#pragma once
#include <vector>
#include <list>
#include <functional>

class TestA;
typedef int (TestA::* twoParam)(int, int);//���Ա������ָ��
using pFunNoParam = void (TestA::*)();//ʹ��using��ʽ�ĳ�Ա����ָ��

typedef int (*Func_Ret_TwoParams)(int, int);//ȫ�ֺ�����ָ��

//ֱ�Ӷ���������ͣ���������չ��
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

//ʹ�ú����ɴ�������

//ʹ�ú�+ģ�����ɴ�������

