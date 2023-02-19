#include <iostream>
#include <functional>
#include "FDelegateTwoParams.hpp"

//��������
//BindRaw();
//BindStatic();
//IsBound();
//Execute();
//ExecuteIfBound();

int AddNum(int a, int b)
{
	const int tmp = a + b;
	std::cout << "[" << __FUNCTION__ << "], " << tmp << std::endl;
	return tmp;
}

//����ָ�룬���ַ�ʽ
typedef int (*FunMethod_t)(int, int);
using FunMethod_u = int(*)(int, int);

//����һ��������
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
	//��ȫ�ֺ���
	{
		FDelegateTwoParams delegate;
		delegate.BindGlobalFunc(&AddNum, 1, 2);
		if (delegate.IsBound())
		{
			delegate.Execute();
		}
	}

	//�ಥ�����������Ա����
	{
		FDelegateTwoParams delegate;
		TestA a;
		delegate.AddRaw(&a, &TestA::AddNum, 2, 3);
		delegate.AddRaw(&a, &TestA::SubNum, 11, 2);
		delegate.BroadCast();
	}

	system("pause");
	return 0;
}