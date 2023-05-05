#include "stdafx.h"
//#include <time.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>//设置输出宽度、日期格式、货币格式等
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include <utility>
#include <regex>
#include <typeinfo>
#include <Windows.h>
#include <winternl.h>
#include "kuohao.h"
#include "qianxiangshengming.h"
#include "copyLinkList.h"
#include "bit_space.h"
#include "People.h"
#include "MyDll.h"
#include "friend.h"
#include "num_sequence.h"
#include "qt_virtual_signal.h"
#include "template_test.h"
#include "singleton.h"
#include "singletonAAA.h"
#include "Decorator.hpp"

using namespace std;

#define ELEM_SIZE 10;

/*class MyString
{
public:
	MyString(char* ch)
	{
		for (int i = 0; *(ch + i) != '\0'; i++)
		{
			data[i] = *(ch + i);
		}
	}
	~MyString();

	vector<char> data;
private:
};*/

namespace MyTemplate
{
	//打印最后一个元素
	template<typename T>
	ostream& print(ostream& os, const T& t)
	{
		return os << t;
	}
	//打印除最后一个元素以外的元素，每次处理一个入参，然后递归
	template<typename T, typename ... Args>
	ostream& print(ostream& os, const T& t, const Args&... rest)
	{
		os << t << ",";
		return print(os, rest...);//包扩展
	}
	//调用函数、包扩展
	template<typename T>
	T& SumOne(T& t)//
	{
		++t;
		return t;
	}
	template<typename ... Args>
	ostream& printSumOne(ostream& os, Args&... rest)
	{
		return print(os, SumOne(rest)...);//使用SumOne对rest中每个参数进行处理，可变参数的处理函数SumOne需要有合适的返回值
	}

	//emplace_back
	template<typename T>
	T& ChangeRightValue(T&& t)//
	{
		++t;
		return t;
	}

	template <typename T, typename... Args>
	void foo(const T &t, const Args& ... rest)
	{
		std::cout << __FUNCTION__ << ", sizeof...(Args):" << sizeof...(Args) << std::endl;
	}
}

template <typename F, typename T1, typename T2>
void flip1(F f, T1 t1, T2 t2)
{
	f(t2, t1);//假设f的一个参数为引用类型，那么t1或t2的值不会被改变，因为flip的参数不是引用类型
}

template<typename T>
void MyRef(T& a)
{
	a++;
}

template<typename T>
void SetIntger(T&& a)
{
	a++;
}

template<typename T>
void DoubleRef(T&& a)
{
	T t = a;
	MyRef(t);
	if (t == a)
	{
		std::cout << "t equal to a, t:" << t << ", a:" << a << std::endl;
	}
	else
	{
		std::cout << "t not equal to a, t:" << t << ", a:" << a << std::endl;
	}
}

template<typename It>
auto GetFirstElement(It beg, It end)->typename remove_reference<decltype(*beg)>::type
{
	return *beg;
}

template<typename It>
auto GetFirstElementRef(It beg, It end)->decltype(*beg)
{
	return *beg;
}

template<typename T>
T GetSum(const T& l, const T& r)
{
//	r++;不能修改const
	return l + r;
}

void PrintSum(int l, int r)
{
	std::cout << l + r << std::endl;
}

//享元模式
class User//外部数据
{
private:
	std::string name;
public:
	User(std::string n) : name(n) { return; }
	std::string GetName() { return name; }
};
class WebSite//享元，父类
{
public:
	virtual void Use(User& user) { return; };
};
class ConcreteWebSite : public WebSite
{
private:
	std::string name;//共享的内部数据
public:
	ConcreteWebSite(std::string n) : name(n) { return; }
	void Use(User& user) { std::cout << "website:" << name << ", user:" << user.GetName() << std::endl; }//使用外部数据
};
class WebSiteFactory//享元对象工厂
{
private:
	std::map<std::string, WebSite*> flyweights;
public:
	WebSite* GetWebSite(std::string key)
	{
		if (flyweights.find(key) == flyweights.end())
		{
			flyweights.insert(std::make_pair(key, new ConcreteWebSite(key)));
		}
		return flyweights[key];
	}
};


//中介者模式
class Colleague;
class ConcreteColleague1;
class ConcreteColleague2;
class Mediator//抽象中介者
{
public:
	virtual void transmit(const std::string& json, Colleague* const colleague) = 0;//转发数据
};
class Colleague
{
protected:
	Mediator* mediator;
public:
	Colleague(Mediator* m) { mediator = m; }
};
class ConcreteColleague1 : public Colleague
{
public:
	ConcreteColleague1(Mediator* m) : Colleague(m) {}
	void Send(const std::string& json) { mediator->transmit(json, this); }//发送数据
	void Receive(const std::string& json) { std::cout << __FUNCTION__ << ", receive:" << json << std::endl; }//处理接收数据
};
class ConcreteColleague2 : public Colleague
{
public:
	ConcreteColleague2(Mediator* m) : Colleague(m) {}
	void Send(const std::string& json) { mediator->transmit(json, this); }//发送数据
	void Receive(const std::string& json) { std::cout << __FUNCTION__ << ", receive:" << json << std::endl; }//处理接收数据
};
class ConcreteMediator : public Mediator
{
private:
	//为了让交互类对象不耦合，而将复杂度集中到中介者类
	ConcreteColleague1* colleague1;
	ConcreteColleague2* colleague2;
public:
	void SetColleague1(ConcreteColleague1* c) { colleague1 = c; }
	void SetColleague2(ConcreteColleague2* c) { colleague2 = c; }

	void transmit(const std::string& json, Colleague* const sender)//具体转发函数
	{
		if (sender == colleague1)//ConcreteColleague1虽然有前向定义，但任然要在使用前进行实现，例如在使用前调用include
		{
			colleague2->Receive(json);
		}
		else if (sender == colleague2)
		{
			colleague1->Receive(json);
		}
	}
};

//职责链模式
class HandlerChain
{
protected:
	HandlerChain* successor;
public:
	void SetSuccessor(HandlerChain* p) { successor = p; }//设置下一个处理节点
	virtual void HandleRequest(int request) = 0;
};
class Handler20 : public HandlerChain//处理[0,20)
{
public:
	void HandleRequest(int request)
	{
		if (request >= 0 && request < 20) { std::cout << __FUNCTION__ << " handle request:" << request << std::endl; }
		else { (successor) ? successor->HandleRequest(request) : nullptr; }
	}
};
class Handler60 : public HandlerChain//处理[20,60)
{
public:
	void HandleRequest(int request)
	{
		if (request >= 20 && request < 60) { std::cout << __FUNCTION__ << " handle request:" << request << std::endl; }
		else { (successor) ? successor->HandleRequest(request) : nullptr; }
	}
};
class Handler100 : public HandlerChain//处理[60,100]
{
public:
	void HandleRequest(int request)
	{
		if (request >= 60 && request <= 100) { std::cout << __FUNCTION__ << " handle request:" << request << std::endl; }
		else { (successor) ? successor->HandleRequest(request) : nullptr; }
	}
};

class Receiver//执行命令对象
{
public:
	void Process() { std::cout << "processing command" << std::endl; }//真正的处理函数
};
class Command
{
protected:
	Receiver* receiver;
public:
	Command(Receiver* r) : receiver(r) { return; }
	virtual void Execute() = 0;//命令类知道命令和执行对象的关联关系
};
class ConcreteCommand : public Command
{
public:
	ConcreteCommand(Receiver* r) : Command(r) { return; }
	void Execute() { receiver->Process(); }
};
class Invoker//传递命令对象
{
private:
	std::vector<Command*> cmds;
public:
	void SetCommand(Command* pCmd) { cmds.push_back(pCmd); }
	void CancelCommand(Command* pCmd) { cmds.erase(std::remove(cmds.begin(), cmds.end(), pCmd), cmds.end()); }
	void ExecuteCommand() { for (auto& cmd : cmds) { cmd->Execute(); } }
};

struct MyInt4
{
	INT32 i32;
	char c4[4];
};

namespace factory
{
	class Animal
	{
	public:
		void eat() {}
		void run() {}
	};

	class ThingA : public Animal
	{};

	class ThingB : public Animal
	{};

	class SimpleFactory
	{
	public:
		static Animal* Create(int type)
		{
			Animal* tmp = nullptr;
			switch (type)
			{
			case 0:
				tmp = new ThingA;
				break;
			case 1:
				tmp = new ThingB;
				break;
			default:
				break;
			}
			return tmp;
		}
	};

	class ThingAFactory : public ThingA
	{
	public:
		ThingA* Create()
		{
			return new ThingA;
		}
	};

	class ThingBFactory : public ThingB
	{
	public:
		ThingB* Create()
		{
			return new ThingB;
		}
	};
}

//代理模式
namespace proxy
{
	class Sender
	{
	public:
		virtual void Send() = 0;
	};

	class ClientClass : public Sender
	{
	public:
		void Send() { std::cout << typeid(*this).name() << ", " << __FUNCTION__ << std::endl; }
	};

	class ProxyClass : public Sender
	{
	public:
		ProxyClass() : m_client(nullptr) {}
		void Send() 
		{ 
			if (!m_client) { m_client = new ClientClass; }
			std::cout << typeid(*this).name() << ", " << __FUNCTION__ << std::endl; 
			m_client->Send();
			delete m_client;
		}
	private:
		ClientClass* m_client;
	};
}

vector<int> GetVec()
{
	return vector<int>{1, 3, 4, 5};
}

typedef NTSTATUS (NTAPI* QEURYINFORMATIONPROCESS)(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL);

class MemberNew
{
public:
	MemberNew(int a){ temp = new (std::nothrow) int(a); }
	int* temp;
};

void MyArrayByReference(int (&arr)[5])
{
	cout << "pass by reference arr:0x" << std::hex << arr << endl;
	cout << "pass by reference &arr:0x" << std::hex << &arr << endl;
}

void MyArrayByPointer(int arr[], size_t length)
{
	cout << "pass by pointer arr:0x" << std::hex << arr << endl;
	cout << "pass by pointer &arr:0x" << std::hex << &arr << endl;
}

template<typename T>
class MyPointer
{
	MyPointer() :_ptr(nullptr){ _ptr = new T; }
	MyPointer(T *ptr) :_ptr(ptr){ }
	~MyPointer() { delete _ptr; }
private:
	T _ptr;
};

//template<typename IteratorType, typename ElemType>//模板中声明一个迭代器类型，一个元素类型
//MyFind

//验证拷贝调用顺序
class A {
public:
	A() : m_ptr(new int(0)) {
		cout << "construct" << endl;
	}
	A(const A &a) : m_ptr(new int(*a.m_ptr)) { //深拷贝的拷贝构造函数
		cout << "copy construct" << endl;
	}
	~A() {
		cout << "delete" << endl;
		delete m_ptr;
	}
	void GetA();
	void GetFuncPointer();
private:
	int *m_ptr;
};

void A::GetA() {
	cout << __FUNCTION__ << endl;
	return;
}

void A::GetFuncPointer() {
	typedef void(A::*F1)();
	F1 f1 = &A::GetA;
	(this->*f1)();

	void(A::*f2)() = &A::GetA;

	//void(*f3)() = &A::GetA;
	return;
}


void TestGetA()
{
	//A a(GetA());

	//GetA();
}

void ThrowTest()
{
	throw 1234;
}

unique_ptr<string> demo(const char * s)
{
	unique_ptr<string> temp(new string(s));
	return temp;
}

void referenceString(int &a)
{
	return;
}
void constReferenceString(const int &a)
{
	return;
}

//使用stringstream实现一个类型转换的模板函数
template<class out_type, class in_value>
out_type convert(const in_value & t)
{
	stringstream stream;
	stream << t;//向流中传值
	out_type result;//这里存储转换结果
	stream >> result;//向result中写入值
	return result;
}

//extern template std::string convert(const int &t);//实例化声明
template std::string convert(const int &t);//显式实例化，实例化定义

//重载class的强制类型转换
class Convert_
{
public:
	Convert_(){ n = 0; }
	~Convert_(){}

	int n;
	operator int() { return n; }//重载强制类型转换
	Convert_& operator++(){ n++; return *this; }//重载前置++
	Convert_ operator++(int){ ++n; return *this; }//重载后置++

private:
};

//字符串分割，根据子串分割，若首字符为分隔符，结果第一项是空字符串
bool MySplit(const string& str, vector<string>& result, const string& delim=" ") {
	if (str.empty() || delim.empty())
	{
		return false;
	}
	result.clear();

	size_t startPos = 0;
	while (startPos < str.length())
	{
		size_t matchPos = str.find(delim, startPos);
		if (string::npos == matchPos)
		{
			result.push_back(str.substr(startPos));
			break;
		}
		result.push_back(str.substr(startPos, matchPos - startPos));
		startPos = matchPos + delim.length();
	}

	return true;
}

void array(int* value, size_t size) {
	for (int i = 0; i < (int)size; ++i)
		cout << "a = " << value[i] << endl;
}

//返回const引用
const string& constReference(string& n) {
	//return string("constReference");//不能返回局部变量的引用
	return n;
}

//func int to float
void funcItoF(float* value, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		cout << value << endl;
	}
}

int GetFileSize(char *filePath)
{
	int count = -1;
	std::fstream f;

	f.open(filePath, std::ios::in);
	if (f.is_open())
	{
		f.seekg(0, std::ios::end);
		count = f.tellg();
		f.close();
	}

	return count;
}

std::string ThrowTestFunc()
{
	std::string s = "This is a throw message.";
	return s;
}

void BubbleSort(int* src, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size -1 - i; j++)
		{
			if (src[j] > src[j + 1])
			{
				std::swap(src[j], src[j + 1]);
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	const int elem_size = 10;
	int data[elem_size] = { 1, 2, 3, 123, 3245, 47, 678, 456, 23, 5 };
	int data_back[elem_size] = { 0 };
	vector<int> ivec(data, data + elem_size);
	vector<int> ivec_back(elem_size);

	//传数组首地址
	array(data, elem_size);

	//调用function object（函数对象）
	std::for_each(ivec.begin(), ivec.end(), MyClass());

	//返回const引用
	string test("123");
	const string ref = constReference(test);

	//func int to float
	float intger[3] = { 1, 30.5, 0 };
	size_t size = sizeof(intger);
	funcItoF(intger, 3);

	//复制复杂链表
	//[[7, null], [13, 0], [11, 4], [10, 2], [1, 0]]
	Node a1(7), a2(13), a3(11), a4(10), a5(1);
	Node* head = &a1;
	a1.next = &a2;
	a2.next = &a3;
	a3.next = &a4;
	a4.next = &a5;
	a1.random = nullptr;
	a2.random = &a1;
	a3.random = &a5;
	a4.random = &a3;
	a5.random = &a1;

	//位域
	normal bit1;
	bitspace bit2;

	bit1.a = 2;
	bit1.b = 4;
	bit2.a = 1;
	bit2.b = 4;

	cout << "sizeof bit1:" << sizeof(bit1) << endl;
	cout << "sizeof bit2:" << sizeof(bit2) << endl;
	
	//拷贝复杂链表
	Node* copy = copyRandomList(head);

	//字符串分割
	string str("\n--123\n--456\n--789");
	string delim("\n--");
	vector<string> subs;
	//cout << "string::npos:" << string::npos << endl;
	MySplit(str, subs, delim);
	MySplit("123\n--456\n--789", subs, delim);
	MySplit("123\n--\n--456\n--789", subs, delim);

	//虚析构函数
	cout << "-----virtual ~People()-----" << endl;
	People* man = new Man();
	delete man;
	cout << "-----Man(int& i)-----" << endl;
	int i = 5;
	Man m1(i);
	cout << "-----Man(int&& i)-----" << endl;
	Man m2(move(i));
	cout << "----------" << endl;

	//string长度是否会记录\0
	string lenStr("12345");
	int len = lenStr.size();

	//使用convert模板函数进行转换
	double d;
	string salary;
	string s = "12.56";
	d = convert<double, string>(s);//d等于12.56
	salary = convert<string>(9000.0);//salary等于”9000”
	
	//重载class的强制类型转换符
	Convert_ mc;
	mc.n = 6;
	int mcOut = (int)mc;
	int pp1 = mc++.n;
	int pp2 = ++mc.n;

	//查看catch变量引用和非引用的地址区别
	string exp;
	exp = "hello world.";
	try
	{
		cout << "exp:" << &exp << endl;
		throw exp;
	}
	catch (string a)//非引用
	{
		cout << "not reference:" << &a << endl;
	}
	try
	{
		cout << "exp:" << &exp << endl;
		throw exp;
	}
	catch (string &a)//引用，catch会生成一个异常对象，以throw抛出对象为初始化参数，所以a和exp地址不同
	{
		cout << "reference:" << &a << endl;
	}
	try
	{
		cout << "exp:" << &exp << endl;
		throw &exp;
	}
	catch (string *a)//指针
	{
		cout << "ptr:" << a << endl;
	}

	//virtual func()
	Man m3;
	cout << "----------" << endl;
	m3.test13();

	//strtok()字符串分割
	char str2[80] = "This is - www.runoob.com - website";
	const char s2[2] = "-";
	char *token;

	token = strtok(str2, s2);/* 获取第一个子字符串 */
	while (token != NULL) {
		printf("%s\n", token);

		token = strtok(NULL, s2);
	}

	//memmove()和memcpy()对比
	char str3[9] = "abcd1234";
	char str4[32] = "xxxx";
	//char *str4 = str3 + 4;
	char str5[32] = "xxxx567890";
	memcpy(str4, str3, sizeof(str3));
	memcpy(str5, str3, sizeof(str3));
	//memccpy(str5, str3, 'c', sizeof(str3));//str3遇到‘c’时停止复制

	//std::move()
	vector<int> src = { 1, 2, 3, 4, 5 };
	int srcSize = src.size();
	int count = src.capacity();
	vector<int> dst(move(src));
	src.shrink_to_fit();

	const int size2 = 64;
	char t[size2];
	memset(t, 0, size2);
	memcpy(t, "1234567", 7);
	int tCount = sizeof(t);

	//try catch
	try
	{
		ThrowTest();
	}
	catch (int a)
	{
		cout << a << endl;
	}

	//dll
	SetDllDirectory(L"dll");//dll分目录存放，需要设置指定dll延迟加载，注意当前工作路径，IDE下运行会有问题
	//LoadLibrary(L"dll\\MyDll_D.dll");
	MyDllFunc1();
	//MyDllFunc2();//不使用_declspec关键字无法正确导出函数，在调用处会链接失败，无法解析的外部符号
	SetDllDirectory(NULL);

	//shared_ptr
	int shared = 10;
	shared_ptr<int> sptr = make_shared<int>(shared);

	//referenceString(333);//会失败
	constReferenceString(333);

	//看vector的范形式定义的左右边界
	int numArray[] = { 1, 2, 3, 4, 5, 6 };
	vector<int> nums(numArray, numArray + sizeof(numArray)/sizeof(int));
	vector<int>::iterator ittt = find(nums.begin(), nums.end(), 4);
	vector<int> numLeft(nums.begin(), ittt);
	vector<int> numRight(ittt, nums.end());
	*ittt;
	//同行定义多个对象
	vector<int> vint(1, 8);
	vector<int>::iterator
		it = vint.begin(),
		end = vint.end();

	//friend关键字
	FriClass fri(5);
	FriClass::iterator myFri = fri.begin();
	for (auto i = fri.begin(); i != fri.end(); ++i)
	{
		cout << *i << endl;
	}

	//unique_ptr&move
	unique_ptr<string> ps1, ps2;
	ps1 = demo("hello");
	ps2 = move(ps1);
	ps1 = demo("alexia");
	cout << *ps2 << *ps1 << endl;

	//查看拷贝构造和析构顺序
	TestGetA();

	//查看vector大小
	vector<int> vec_1 = { 1, 2, 3, 4 };
	vector<int> vec_2 = { 1 };
	size_t vec_size1 = sizeof(vec_1);
	size_t vec_size2 = sizeof(vec_2);

	num_sequence ns;
	num_sequence* pns = &ns;
	PtrType pm = &num_sequence::func1;
	(ns.*pm)(1);
	(pns->*pm)(1);

	//使用fstream把二维数组写入文件，测试打开文件错误时的state
	unsigned int fileArray01[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
	unsigned int fileArray02[2] = { 1, 2 };
	basic_fstream<unsigned int> file01("file01.bin", ios::in/*ios::out | ios::binary*/);
	//basic_fstream<unsigned int> file02("file02.bin", ios::in/*ios::out | ios::binary*/);
	//if (!file01.is_open() || !file02.is_open())
	//{
	//	file01.close();
	//	file02.close();
	//}
	//file01.write(fileArray02, 2);
	//file01.close();
	//file02.close();

	cout << "file01 state:" << file01.rdstate() << endl;
	file01.close();
	file01.clear();
	cout << "file01 state:" << file01.rdstate() << endl;
	ios_base::goodbit;
	file01.open("file02.bin", ios::in/*ios::out | ios::binary*/);
	cout << "file02 state:" << file01.rdstate() << endl;
	file01.close();
	cout << "file02 state:" << file01.rdstate() << endl;

	//查看本机char是否是signed char
	char pcSign = -1;
	signed char testB = -1;
	unsigned char testC = -1;
	printf("a:%d b:%d c:%d\n", pcSign, testB, testC);

	//验证虚函数是否会传值失败
	Camera_p* cam = new G5ComCamera();
	int z1 = 1, z2 = 2;
	dynamic_cast<G5ComCamera*>(cam)->test();

	//remove_reference简单使用
	int reference[4] = { 1, 2, 3, 4 };
	remove_reference<int&>::type b = reference[0];//b是int类型，不带引用
	//int &b = a[0];
	reference[0] = 5;
	std::move(b);//move仅提供强制类型转换

	//大小端序判断，使用联合体
	static union
	{
		char c[4];
		unsigned long l;
	}endian_test = { { 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.l)
	char ttttt = (char)endian_test.l;
	if ('l' == ENDIANNESS)
		cout << "little-endian" << endl;
	else
		cout << "big-endian" << endl;

	//构造智能指针
	shared_ptr<string> shaString;
	shared_ptr<list<int>> shaList;
	shared_ptr<int> shaInt(new int(42));
	shared_ptr<int> shaInt2 = make_shared<int>(42);

	shared_ptr<int> shaInt3(new int(42), [](int *p){delete p; });

	FILE* ptrFile = fopen("testfile.txt", "r");
	if (ptrFile)
	{
		//做一些操作
		fclose(ptrFile);
	}
	cout << "after fopen" << endl;

	//{
	//	shared_ptr<FILE> shaFile(fopen("testfile.txt", "r"), fclose);
	//	if (shaFile)
	//	{
	//		//做一些操作
	//	}
	//}

	try
	{
		int* pInt = shaInt.get();
	}
	catch (bad_alloc e)
	{
		cout << e.what() << endl;
	}

	unique_ptr<int> uniInt(new int(42));
	unique_ptr<int[]> uniInt2(new int[10]());//10个初始值为0的int
	unique_ptr<int> uniInt3(auto_ptr<int>(new int(42)));
	unique_ptr<int> uniInt4(uniInt.release());
	unique_ptr<int> uniInt5;
	uniInt5.reset(uniInt4.release());

	weak_ptr<int> weakInt(shaInt);
	if (weakInt.lock())//返回对应的shared_ptr，若expired()返回为true则返回一个空shared_ptr
	{
		//做一些操作
	}
	weakInt.use_count();//对应的shared_ptr的数量
	weakInt.expired();//对应的shared_ptr的数量为0则返回true

	//获取文件大小
	int filesize = GetFileSize("testfile.txt");
	
	//用throw抛出函数返回值
	try
	{
		throw ThrowTestFunc();
	}
	catch (std::string& s)
	{
		cout << s << endl;
	}

	//连续赋值
	int coma, comb;
	coma = comb = 1;
	const int conA = 1;
	int nonConB = conA;

	//优先队列，大顶堆
	priority_queue<int> myPriQueue;
	myPriQueue.push(1);
	myPriQueue.push(2);
	myPriQueue.push(3);
	cout << "top:" << myPriQueue.top() << endl;
	myPriQueue.pop();
	cout << "top:" << myPriQueue.top() << endl;

	//分配器
	std::allocator<std::string> myAlloc;
	auto const pSrc = myAlloc.allocate(5);
	auto qDst = pSrc;
	myAlloc.construct(qDst++);
	myAlloc.construct(qDst++, 10, 'c');
	myAlloc.construct(qDst++, "hi:");
	while (qDst != pSrc)
	{
		myAlloc.destroy(--qDst);
	}
	myAlloc.deallocate(pSrc, 5);

	std::allocator<int> myAlloc2;
	cout << "max_size:" << myAlloc2.max_size() << endl;
	int *intArr = myAlloc2.allocate(100);
	intArr[5] = 333;
	myAlloc2.deallocate(intArr, 100);

	//localtime/localtime_s
	time_t seconds =25000000000;
	tm myTime;
	tm* pMyTime = nullptr;
	memset(&myTime, 0, sizeof(myTime));
	pMyTime = localtime(&seconds);
	localtime_s(&myTime, &seconds);

	//array pass by reference引用方式传输组
	const int arrSize = 5;
	int myArr[arrSize] = { 1, 2, 3, 4, 5 };
	cout << "myArr:0x" << std::hex << myArr << endl;
	MyArrayByReference(myArr);
	MyArrayByPointer(myArr, arrSize);

	//查看new创建的对象生命周期是否超过大括号范围
	int* pMemNewInt = nullptr;
	{
		MemberNew memNew(10);
		pMemNewInt = memNew.temp;
	}

	//vector保存指针，查看vector析构时是否会释放指针所指空间
	std::vector<char*>* vec_p = new vector<char*>();
	char* vec_char = new char[20]{0};
	char* vec_src = "1234567890";
	memcpy(vec_char, vec_src, sizeof(vec_src));
	vec_p->push_back(vec_char);
	delete vec_p;

	//double
	double db1 = 1 / 2;
	double db2 = 1 / (double)2;

	//继承单例
	SingletonAAA* st = SingletonAAA::GetInstance();
	st->Show();

	//hash
	std::hash<int> hash_int;
	std::vector<int> n{1, 2, 3, 4, 5};
	std::transform(std::begin(n), std::end(n), std::ostream_iterator<size_t>(std::cout, ", "), hash_int);
	cout << endl;
	cout << hash_int(n[0]) << endl;

	vector<int> vec = GetVec();

	//深拷贝的拷贝构造函数
	A gg;
	gg.GetFuncPointer();

	//正则表达式分割字符串
	std::string mail("123@qq.vip.com,456@gmail.com,789@163.com,abcd@my.com");
	std::regex reg(",");
	std::sregex_token_iterator seprator(mail.begin(), mail.end(), reg, -1);//submatches参数0为匹配reg字符串，-1为非reg字符串（反选）
	decltype(seprator) s_end;
	for (; seprator != s_end; ++seprator)
	{
		cout << seprator->str() << endl;
	}
	string G5string("gs-G5-sd-23-x8");
	std::regex G5reg("g5", regex::icase);
	if (std::regex_search(G5string, G5reg))
	{
		cout << "regex find" << endl;
	}

	//装饰模式
	{
		MyCar car;
		InstallWheel wheel(car);
		wheel.Run();
	}

	bool flag02 = false;
	const int len02 = 3;
	bool arr02[len02] = { true, false, true };
	if (arr02[0]) flag02 = arr02[0];
	flag02 &= arr02[1];
	flag02 &= arr02[2];

	//38.ctime、iomanip输出格式化的日期时间
	{
		//time_t now = time(0);
		//tm* startTime(localtime(&now));
		//std::stringstream ss;
		//ss << dec << 1900 + startTime->tm_year << "-" << 1 + startTime->tm_mon << "-" << startTime->tm_mday \
		//	<< " " << setw(2) << setfill('0') << startTime->tm_hour << ":" << setw(2) << setfill('0') << startTime->tm_min << ":" << setw(2) << setfill('0') << startTime->tm_sec;
		//cout << ss.str() << endl;
		auto t_now = chrono::system_clock::to_time_t(chrono::system_clock::now());
		cout << std::put_time(std::localtime(&t_now), "%Y-%m-%d %H:%M:%S") << endl;
	}

	//代理模式
	{
		using namespace proxy;
		ProxyClass proxySender;
		proxySender.Send();
	}

	//工厂模式
	{
		using namespace factory;
		ThingAFactory factoryA;
		auto a1 = factoryA.Create();
		auto a2 = factoryA.Create();
		ThingBFactory factoryB;
		auto b1 = factoryB.Create();
		auto b2 = factoryB.Create();
	}

	char nchArr[4] = { 0 };
	_snprintf(nchArr, 4-1, "%s", "12345");
	printf("%s\n", nchArr);

	//使用sizeof计算不定长数组长度
	{
		struct StrBuff
		{
			StrBuff(const std::string& l, const std::string& r) :m_l(l), m_r(r) { return; }
			std::string m_l;
			std::string m_r;
		};
		StrBuff strArray[] = { StrBuff("111", "222"), StrBuff("111", "222"), StrBuff("111", "222") };
		int strArrayLen = sizeof(strArray) / sizeof(StrBuff);
	}

	//观察int32给char[4]赋值后字节顺序
	{
		MyInt4 mint = { 0x1000 };
		int tt = 0x12345678;
		memset(mint.c4, 0, 4);
		memcpy(mint.c4, &tt, 4);
		tt = 0;
	}

	{
		Man m3;//局部对象会在定义时初始化
		int t = 0;
	}

	//冒泡排序
	{
		int arr[5] = { 7, 5, 3, 4, 1 };
		BubbleSort(arr, 5);
		int t = 1;
	}
	
	//测试map、vector遍历时删除元素
	{
		std::map<char, int> content;
		content['a'] = 1;
		content['b'] = 2;
		content['c'] = 3;

		for (auto it = content.begin(); it != content.end();)
		{
			if (it->second == 2)
			{
				it = content.erase(it);
			}
			else
			{
				++it;
			}
		}

		std::vector<int> vect = { 1, 1, 1, 2, 3 };
		for (auto it = vect.begin(); it != vect.end(); it++)
		{
			if(*it == 2)
			{
				//vect.erase(it);//会崩溃，因为释放it所指对象后没有给it重新赋值，在后续it++中调用已释放内存导致崩溃
			}
		}
		//尝试使用std::remove
		auto i = std::remove(vect.begin(), vect.end(), 1);//会将非1的数值向前复制并覆盖，变成{ 2, 3, 1, 2, 3 }，其中只有2、3向前复制了
		vect.erase(i, vect.end());
		int t = 0;
	}

	//命令模式
	{
		Receiver* receiver = new Receiver;
		ConcreteCommand* cmd1 = new ConcreteCommand(receiver);
		Invoker ivk;

		ivk.SetCommand(cmd1);
		ivk.ExecuteCommand();

		delete cmd1;
		delete receiver;
	}

	std::cout << dec;

	//职责链模式
	{
		int request = 70;//请求可以根据业务需要改成类的形式
		Handler20* h20 = new Handler20;
		Handler60* h60 = new Handler60;
		Handler100* h100 = new Handler100;
		h20->SetSuccessor(h60);
		h60->SetSuccessor(h100);
		h100->SetSuccessor(nullptr);

		h20->HandleRequest(request);//从最初的节点开始尝试处理请求

		delete h100;
		delete h60;
		delete h20;
	}

	//中介者模式
	{
		ConcreteMediator* m = new ConcreteMediator;
		ConcreteColleague1* colleague1 = new ConcreteColleague1(m);
		ConcreteColleague2* colleague2 = new ConcreteColleague2(m);
		m->SetColleague1(colleague1);
		m->SetColleague2(colleague2);

		colleague1->Send("this is colleague1");
		colleague2->Send("this is colleague2");

		delete colleague2;
		delete colleague1;
		delete m;
	}

	//popen
	{
		const int length = 10;
		char temp[length] = { 0 };
		FILE* fp = _popen("ipconfig", "r");

		while (fgets(temp, length, fp))//fgets自动保留1字节作为\0
		{
			//printf(temp);
		}

		_pclose(fp);
	}

	{
		char buf[24] = { 0 };
		int t = 0;
	}

	//享元模式
	{
		WebSiteFactory webFactory;
		WebSite* site01 =  webFactory.GetWebSite("site01");
		User user("wang");

		site01->Use(user);
	}

	//int转unsigned int再转int
	{
		int a1 = -1;
		unsigned int a2 = a1;
		int a3 = a2;
		int temp = 0;
	}

	{
		char save[10];
		memset(save, 0, 10);

		//strcat(save, "123456789+123456789+");//该方式存入过量字符后，在程序结束会抛出运行时栈溢出异常（run-time check failure#2-stack）

		memset(save, 0, 10);
		strncpy_s(save, 10, "123456789+123456789+", 9);

		memset(save, 0, 10);
		strcpy_s(save, 10, "1234");

		int temp = 0;
	}

	//std::bind()
	{
		auto fn_sum = std::bind(PrintSum, std::placeholders::_1, std::placeholders::_2);

		fn_sum(1, 2);
	}

	//stl优先队列
	{
		priority_queue<int, vector<int>, greater<int>> que;//优先队列，默认大顶堆，greater小顶堆
		que.push(1);
		que.push(4);
		que.push(2);
		que.push(3);

		std::cout << "priority queue:" << std::endl;
		while (!que.empty())
		{
			std::cout << que.top();
			que.pop();
		}
		std::cout << std::endl;
	}

	//负0
	{
		double t1 = 0.0;
		double t2 = -0.0;
		if (t1 == t2)
		{
			int t = 1;
		}
		int tmp = 1;
	}

	{
		int a = 1;
		int b = 2;
		std::cout << "template GetSum:" << GetSum(a, b) << std::endl;
		int tmp = 1;
	}

	{
		auto pFunc = []()->int{ return 222; };
		std::cout << "lambda function with return value:" << pFunc() << std::endl;
	}

	//测试模板返回值是用auto，尾置返回->decltype(xxx)
	//使用remove_reference得到原始类型，返回原始类型的拷贝
	{
		std::vector<int> v = { 11, 12, 13 };
		auto &first = GetFirstElementRef(v.begin(), v.end());//返回引用
		std::cout << "GetFirstElementRef:" << first << std::endl;

		auto f = GetFirstElement(v.begin(), v.end());//返回拷贝
		std::cout << "GetFirstElement:" << f << std::endl;
	}

	//右值引用参数，int传入参数为int&&的模板函数后被自动转换成int&
	{
		int a = 1;
		SetIntger(a);
		std::cout << "SetIntger:" << a << std::endl;
	}
	{
		int a = 1;
		DoubleRef(a);
	}

	{
		int a = 1;
		std::ref(a);
		std::forward<int>(a);
	}

	//格式化字符串显示当前时间，c标准库函数
	{
		time_t aTime = time(NULL);
		struct tm * curtime = localtime(&aTime);
		char ctemp[32];
		strftime(ctemp, 32, "%Y-%m-%d %H:%M:%S", curtime);
		std::cout << ctemp << std::endl;
	}

	{
		std::list<int> in = { 1, 2, 3 };
		std::list<int> out;
		for (auto it = in.begin(); it != in.end(); ++it)
		{
			out.push_back(std::move(*it));
		}
		std::swap(in, out);
		int tmp = 0;
	}

	//可变参数模板
	{
		int i = 1;
		int j = 2;
		int k = 3;
		double d = 3.14;
		string s("hello");
		MyTemplate::foo(i, d, s);
		MyTemplate::print(cout, i, d, s);//模仿printf
		std::cout << std::endl;
		MyTemplate::printSumOne(cout, i, j, k);//对可变参数增加统一处理函数
		std::cout << std::endl;
		const int i2 = 5;
		//MyTemplate::ChangeRightValue(i2);//把const传给右值引用参数，如果函数内会修改变量，则编译实例化函数时会报错
	}

	{
		int* p = nullptr;
		bool t = true;
		t = (p);
		int temp = 0;
	}

	{
		const char* test_str = "test";
		const wchar_t* wtest_str = L"test";
		int len = strlen(test_str);
		int wlen = wcslen(wtest_str);
		int itest = 0x12345678;
		for (int i = 0; i < 99; i++)
		{
			int t = 0;
		}
		itest = 3;
		int t2 = 0;
	}

	//测试UE中FString使用时要加*的原因
	//{
	//	MyString out("Hello_World");
	//	std::cout << *out << std::endl;

	//}

	::system("pause");
	return 0;
}
