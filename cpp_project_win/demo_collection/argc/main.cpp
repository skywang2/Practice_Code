#include <iostream>
#include <string>
using namespace std;

struct A{
	int a;
	int b;
};

int main(int argc, char* argv[]) {
	for (auto p = argv; p != argv + argc; ++p)
	{
		cout << *p << endl;
	}

	A ff[3] = { 0 };
	auto p1 = ff;
	auto p2 = ff + 1;//起始地址随数组元素增大

	//if (cin) {
	//	cout << "1234" << endl;
	//	cerr << "error test" << endl;
	//	clog << "log test" << endl;
	//}

	string out;
	while (cin >> out) cout << out << endl;

	A a;
	A &b = a;

	int ptr[10] = { 0 };
	new(ptr) int[10] {1, 2, 3, 4};//placement new，在指定位置创建对象

	return 0;
}
