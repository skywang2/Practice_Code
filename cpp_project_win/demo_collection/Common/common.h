#ifndef COMMON_H
#define COMMON_H

#include <iostream>

#define PRINT_LINE() \
std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl

class A
{
public:
	A(){};
	~A(){};

	void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};

//普通继承
class A_child: public A
{
public:
	A_child(){};
	~A_child(){};

	void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};

class B
{
public:
	B(){ PRINT_LINE(); };
	virtual ~B(){ PRINT_LINE(); };

	virtual void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};

//父类带虚函数的继承B
class B_child : public B
{
public:
	B_child() :B(){ PRINT_LINE(); };
	virtual ~B_child(){ PRINT_LINE(); };

	void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};

class B_child_child : public B_child
{
public:
	B_child_child() :B_child(){ PRINT_LINE(); };
	virtual ~B_child_child(){ PRINT_LINE(); };

	void print(){ PRINT_LINE(); }
};

class C
{
public:
	C(){ PRINT_LINE(); };
	~C(){ PRINT_LINE(); };

	virtual void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};

//父类带虚函数的继承C
class C_child : public C
{
public:
	C_child(){ PRINT_LINE(); };
	~C_child(){ PRINT_LINE(); };

	void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};

//父类都带虚函数的多继承D
class D :public B, public C
{
public:
	D() :B(), C(){ PRINT_LINE(); };
	~D(){ PRINT_LINE(); };

	void print(){ std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "]" << std::endl; }
};
#endif
