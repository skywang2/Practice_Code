#pragma once
#include <iostream>
using std::cout;
using std::endl;

class Car
{
public:
	virtual void Run() = 0;
};

//被装饰类
class MyCar : public Car
{
public:
	MyCar() :Car() { return; }
	void Run() { cout << "run" << endl; }
};

//装饰类，父类
class Install : public Car
{
public:
	Install() : Car()
	{ return; }

	void Run() { cout << "install something" << endl; }
};

//装饰类，子类
class InstallWheel : public Install
{
public:
	InstallWheel(Car& car)
		:Install(),
		car(car)
	{ return; }

	void Run()
	{
		cout << "install wheel" << endl;
		car.Run();
	}
private:
	Car& car;
};
