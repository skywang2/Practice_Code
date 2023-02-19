#include <iostream>
#include <functional>
#include "FDelegateTwoParams.hpp"

//单播代理
//BindRaw();
//BindStatic();
//IsBound();
//Execute();
//ExecuteIfBound();

//使用宏声明代理类型
DECLARE_DELEGATE(MacroDelegate)

DECLARE_DELEGATE_NOPARAM(NoParamDelegate)

//定义一个全局函数
int AddNum(int a, int b)
{
	const int tmp = a + b;
	std::cout << "[" << __FUNCTION__ << "], " << tmp << std::endl;
	return tmp;
}

//定义一个测试类
class TestA
{
public:
	void FunNoParam()
	{
		std::cout << __FUNCTION__ << std::endl;
	}

	int AddNum(int a, int b)
	{
		const int tmp = a + b;
		std::cout << "[" << __FUNCTION__ << "], " << tmp << std::endl;
		return tmp;
	}

	int SubNum(int a, int b)
	{
		const int tmp = a - b;
		std::cout << "[" << __FUNCTION__ << "], " << tmp << std::endl;
		return tmp;
	}
};

int main()
{
	//绑定全局函数
	{
		FDelegateTwoParams delegate;
		delegate.BindGlobalFunc(&AddNum, 1, 2);
		if (delegate.IsBound())
		{
			delegate.Execute();
		}
	}

	//多播，绑定两个类成员函数
	{
		FDelegateTwoParams delegate;
		TestA a;
		delegate.AddRaw(&a, &TestA::AddNum, 2, 3);
		delegate.AddRaw(&a, &TestA::SubNum, 11, 2);
		delegate.BroadCast();
	}

	//使用宏声明代理类型
	{
		MyMacroDelegate delegate;
		TestA a;
		delegate.BindRaw(&a, &TestA::FunNoParam);
		if (delegate.IsBound())
		{
			delegate.Execute();
		}
	}

	//使用宏+模板声明代理类型
	{
		NoParamDelegate delegate;
		TestA a;
		delegate.BindRaw(&a, &TestA::FunNoParam);
		if (delegate.IsBound())
		{
			delegate.Execute();
		}
	}

	system("pause");
	return 0;
}


