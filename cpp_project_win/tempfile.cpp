//练习设计模式
//代码基本来自《大话设计模式》每篇的总结，代码使用C++

//简单工厂模式
//用于类型多，但每个类型的实例对象很少的情况
class SimpleFactory
{
  //使用默认构造函数和析构函数
public:
  static ParentClass* CreateObj(string type)
  {
    ParentClass* parent = nullptr;
    switch(type.toInt())
    {
      case SubClassA:
        parent = new SubClassA();//向上转型
        break;
      case SubClassB:
        parent = new SubClassB();
        break;
      default:
        break;
    }
    return parent;
  }
};

//策略模式，定义一组外部调用接口相同的算法
class Strategy
{
public:
  bool Algorithm() = 0;//纯虚函数，该类为纯虚类
};

class StrategyA : public Strategy
{
public:
  bool Algorithm() override { cout << "hello world" << endl;}
};

//1.经典策略模式，封装一套调用算法的统一接口类
class Context
{
public:
  Context(Strategy s) { this.stg = s; }//由外部构造具体算法对象并传入
  bool PublicInterface() { return stg.Algorithm(); }
  Strategy stg;
};

//2.策略模式与简单工厂结合
class Context
{
public:
  //由外部指定算法类型，在内部构造算法对象
  Context(string type)
    :stg(nullptr)
  {
    switch(type.toInt())
    {
      case StrategyA:
        stg = new StrategyA();
        break;
      case StrategyB:
        stg = new StrategyB();
        break;
      default:
        break;
    }
  }
  
  bool PublicInterface() { return stg->Algorithm(); }
  Strategy* stg;
};

//单一职责原则：一个类应只有一种引起它内部变化的操作。
//开放关闭原则：不修改已有的，可以在已有的基础上封装、继承、组合更新功能。写功能时可以先写一个具体类，在第一次增加功能相似的类时创建公共抽象类。
//依赖倒置原则：高层模块不应依赖低层模块，两者都应依赖抽象模块；细节应依赖抽象。意思是高层模块应尽量调用抽象类、抽象接口，低层模块要继承、实现抽象类。
//里氏代换原则：代码中子类替换掉父类时功能不受影响，外界看不出替换后的变化。

//装饰模式
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

{
	MyCar car;
	InstallWheel wheel(car);//出于简化目的在构造函数中传入被装饰对象，实际开发还可增加set函数来设置被装饰对象
	wheel.Run();
}

//代理模式
using namespace proxy
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
		ClientClass* m_client;//包含真正的请求对象
	};
}

{
	using namespace proxy;
	ProxyClass proxySender;
	proxySender.Send();
}

//工厂模式
//用于类型不多（多也没事），类型的实例化对象很多的情况，可以减少简单工厂中选择实例的判断语句
//缺点：增加具体产品类的时候需要增加对应的工厂类
namespace factory
{
	class Animal
	{
	public:
		void eat() {}
		void run() {}
	};
	
	//具体类A
	class ThingA : public Animal
	{};

	//具体类A
	class ThingB : public Animal
	{};

	//简单工厂版本
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

	//普通工厂版本
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
}//namespace

{
	using namespace factory;
	ThingAFactory factoryA;//通过多写工厂类子类来简化调用时的选择判断，既是优点也是缺点
	auto a1 = factoryA.Create();
	auto a2 = factoryA.Create();
	ThingBFactory factoryB;
	auto b1 = factoryB.Create();
	auto b2 = factoryB.Create();
}

//原型模式
//提供一个copy/clone函数，根据实现不同可提供浅拷贝或深拷贝
//浅拷贝可用于已存在实例，但不方便再调用构造函数的情况
//深拷贝可用于已存在实例，但构造过程复杂，拷贝效率较高的情况
class Cloneable
{
public:
	virtual Cloneable* clone() = 0;
};
class Resume : public Cloneable
{
	Cloneable* clone()
	{
		//copy/clone
	}
}

{
	Cloneable* item1 = new Resume;
	Cloneable* item2 = item1.clone();
}

//模板模式
//定义算法流程的固定步骤，将步骤的实现延迟到子类中实现，在子类中可修改算法某一步的具体操作而不影响整个流程
class Algorithm
{
	virtual void Step1();
	virtual void Step2();
	virtual void Step3();
	void Run()
	{
		Step1();
		Step2();
		Step3();
	}
}
class GeneticAlgorithm ：Algorithm
{
	void Step1() { /*具体实现*/ }
	void Step2() { /*具体实现*/ }
	void Step3() { /*具体实现*/ }
}

{
	GeneticAlgorithm ga;
	ga.Run();
}

