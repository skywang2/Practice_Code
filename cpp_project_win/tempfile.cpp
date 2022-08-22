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


//迪米特法则：如果两个类没有直接通信，那就不应该产生联系。如果A类要调用B类的一个方法，可以通过第三方转发这个方法

//外观模式
//将一组有相同功能的对象作为成员变量，在外观类中定义函数，在函数中按需要的顺序调用功能类的同一种函数
//非常常用的模式，有许多变体，可以从父类继承一套统一接口，也可以把一组功能通过上层类进行函数封装并调用（即父类函数中调用一组子类函数）
class Control
{
public:
	virtual void start() = 0;
	virtual void end() = 0;
}
class Engine : public Control
{
public:
	void start() override { m_s = new std::string; }
	void end() override { delete m_s; }
private:
	std::string* m_s;
}
class Light : public Control
{
public:
	void start() override { m_isOpen = true; }
	void end() override { m_isOpen = false; }
private:
	bool m_isOpen;
}
class Facade : public Control
{
	void start() override
	{
		m_engine.start();
		m_light.start();
	}
	void end() override
	{
		m_engine.end();
		m_light.end();
	}
private:
	Engine m_engine;
	Light m_light;
}

//建造者模式
//用于构造新对象，新对象使用一组固定流程进行建造（初始化），并且流程的每一步可以根据类型进行定制
//具体实现有建造类、指挥类，建造类负责实现建造函数（初始化）和返回被建造的产品对象，指挥类负责调用建造类对象
class Product
{};
class Computer : public Product
{
	std::vector<Product*> m_componts;
public:
	void Add(Product* obj)
	{
		m_componts.push_back(obj);
	}
};
class Memory : public Product
{};
class CPU : public Product
{};
class Builder
{
public:
	void BuildPartA(Product* obj) = 0;
	void BuildPartB(Product* obj) = 0;
	Product* GetResult() = 0;
}
class ComputerBuilder : public Builder
{
private:
	Product* m_computer = new Computer;
public:
	void BuildPartA()
	{
		m_computer.Add(new Memory);
	}
	void BuildPartB()
	{
		m_computer.Add(new CPU);
	}
	Product* GetResult()
	{
		return m_computer;
	}
};
class Director
{
public:
	//调用建造类建造对应的对象
	void StartBuild(Builder* bdr)
	{
		bdr->BuildPartA();
		bdr->BuildPartB();
	}
}

{
	ComputerBuilder builder;
	Director director;
	director.StartBuild(builder);
	
	Computer* computer01 = builder.GetResult();
}

//观察者模式，又称发布-订阅模式
//通过构造抽象通知类和观察者类，实现解耦效果，体现依赖倒转原则
//通常用于一对多的关系，一个对象改变需要通知其他对象都改变，并且被通知的对象互相独立
//经典观察者模式，观察者拥有函数名相同的响应函数
class Subject
{
	std::vector<Observer*> observers;
public:
	void Attach(Observer* ob)
	{
		observers.push_back(ob);
	}
	void Attach(Observer* ob)
	{
		auto it = std::find(observers.begin(), observers.end(), ob);
		if(it != observers.end())
		{
			observers.push_back(it);
		}
	}
	void Notify()
	{
		for(auto& it : observers)
		{
			it->Update();
		}
	}
};
class SubClass : public Subject
{
//do something
};
class Observer
{
public:
	void Update() = 0;
};
class ObClassA : public Observer
{
public:
	void Update()
	{
		//do something
	}
};
class ObClassB : public Observer
{
public:
	void Update()
	{
		//do something
	}
};

{
	SubClassA subject;
	ObClassA* obA = new ObClassA;
	ObClassB* obB = new ObClassB;
	
	subject.Attach(obA);
	subject.Attach(obB);
	subject.Notify();
	subject.Detech(obA);
}
//使用事件委托的观察者模式（.net原生支持，c++需要自己实现），抽象通知类不依赖抽象观察者类
//在通知类中加入实现事件委托的成员对象，在外部将观察者的通知处理函数注册到事件委托对象中，因此观察者中接受通知的处理函数可以不同名

//抽象工厂模式
//使用两层抽象，即工厂类是抽象类，工厂类中生成并返回的对象也以抽象类形式表示，一个工厂可以生成多种抽象类
class IDepartment//抽象类，创建一张“部门”相关的数据表
{
public:
	void Insert(string query);	
};
class SqlserverDepartment : public IDepartment//Sqlserver数据库
{
public：
	void Insert(string query);
};
class AccessDepartment : public IDepartment//access数据库
{
public：
	void Insert(string query);
};
class IPeople//抽象类，创建一张“员工”相关的数据表
{
public:
	void Insert(string query);	
};
class SqlserverPeople : public IPeople//Sqlserver数据库
{
public：
	void Insert(string query);
};
class AccessPeople : public IPeople//access数据库
{
public：
	void Insert(string query);
};
class IFactory//抽象工厂类	
{
public:
	IDepartment CreateDepartment();
	IPeople CreatePeople();
};
class SqlserverFactory : public IFactory
{
public:
	IDepartment CreateDepartment() { return SqlserverDepartment() }
	IPeople CreatePeople() { return SqlserverPeople() }
};
class AccessFactory : public IFactory
{
public:
	IDepartment CreateDepartment() { return AccessDepartment() }
	IPeople CreatePeople() { return AccessPeople() }
};

{
	IFactory* factory = new SqlserverFactory();//使用抽象工厂类指针指向具体工厂类
	IDepartment* id = factory->CreateDepartment();
	IPeople* ip = factory->();
	
	id->Insert("xxxxxx");
	ip->Insert("xxxxxx");
}



