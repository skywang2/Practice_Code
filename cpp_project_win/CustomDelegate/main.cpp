#include <iostream>
#include <functional>

//单播代理
//BindRaw();
//BindStatic();
//IsBound();
//Execute();
//ExecuteIfBound();

int AddNum(int a, int b)
{
	const int tmp = a + b;
	return tmp;
}

//函数指针，两种方式
typedef int (*FunMethod_t)(int, int);
using FunMethod_u = int(*)(int, int);


int main()
{
	FunMethod_t funcPtr = AddNum;
	std::cout << funcPtr(1, 2) << std::endl;


	system("pause");
	return 0;
}