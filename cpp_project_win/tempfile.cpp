//练习设计模式
//代码基本来自《大话设计模式》每篇的总结，代码使用C++，代码有部分省略可能需要求改才能编译过

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
  virtual bool Algorithm() = 0;//纯虚函数，该类为纯虚类
};

class StrategyA : public Strategy
{
public:
  virtual bool Algorithm() override { cout << "hello world" << endl;}
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
	virtual void Update() = 0;
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
	virtual void Insert(string query);	
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

//状态模式
//将与某一状态相关的操作封装到类中，得到多个状态类，通过增加新的状态类可以容易的增加新的状态相关操作和状态转换
//状态模式通过把各类状态操作分布到State的子类中来减少相互依赖，减少使用if判断状态量的操作
//适用场景：对象的行为取决于他的状态，并且对象会在运行时改变状态来改变行为
class State
{
public:
	virtual void WriteProgram(Work& w) = 0;
};
class Time01 : public State
{
public:
	void DoSomething(Worker& w)
	{ 
		cout << "动作01" << endl;
		w.Action01();
		w.SetState(new TimeDefault());//从Time01状态转到TimeDefault状态
	}
};
class Time02 : public State
{
public:
	void DoSomething(Worker& w)
	{ 
		cout << "动作02" << endl;
		w.Action02();
		w.SetState(new TimeDefault());//从Time02状态转到TimeDefault状态
	}
};
class TimeDefault : public State
{
public:
	void DoSomething(Work& w)
	{ 
		cout << "默认动作" << endl;
		w.ActionDefault();
	}
};
class Worker
{
public:
	void Worker() { currentState = new Time01; }//假设初始状态是Time01
	
	void Action01() { return; }//代表具体操作，由state对象调用
	void Action02() { return; }
	void ActionDefault() { return; }
	
	void SetState(State* s) { currentState = s; }
	void Do() { currentState->DoSomething(); }//调用状态对象实现操作，并转换状态
private:
	State* currentState;
};

//适配器模式
//系统的数据和行为都正确，但是接口不符合，可以考虑适配器模式，这样可以快速复用已有类型
class Target
{
public:
	//描述客户端需要使用的接口形式
	virtual void Request() { /*默认操作*/ };
};
class Adaptee
{
public:
	void Do() { /*需要适配的操作*/ }
};
class Adapter : public Target
{
private:
	Adaptee* ad;
public:
	Adapter() { ad = new Adaptee(); }
	void Request()
	{
		ad->Do();
	}
};

{
	Target* t = new Adapter;
	t->Request();//实际执行的是Adaptee的Do操作
}

//备忘录模式
//在不破坏封装性的前提下，在对象外保存对象内部状态，用于后续恢复对象原有状态
//适用于功能复杂，需要记录历史数据的类
class GameRole
{
public:
	GameRole()
	{
		data.push_back(1);//插入数据
		data.push_back(2);
	}
	Memento SaveData()
	{
		return Memento(data);
	}
	void LoadData(Memnto m)
	{
		data = m.data;//让设置成员变量的操作在函数内进行，保证封装性
	}
private:
	std::vector<int> data;
};
class Memento
{
public:
	Memento(std::vector<int> data) { this->data = data; }

	std::vector<int> data;//该类型取决于要保存的数据，可以考虑使用模板类
};
class MementoManager//负责保存管理Memento对象
{
public:
	void SetMemento(Memento m) { holder = m; }
	Memento GetMemento() { return holder; }
private:
	Memento holder;
};

{
	GameRole player;
	
	MementoManager manager;
	manager.SetMemento(player.SaveData());//保存数据
	
	//此处player内的数据被修改
	
	player.LoadData(manager.GetMemento());//加载数据
}

//组合模式
//特点：由很多同类对象组成树状结构；整体与部分可以被一致对待
//适用场景：需要体现部分与整体层次，并且需要部分与整体操作方式统一的时候可用组合模式
//在实现时存在两种方式：透明方式；安全方式，两者各有优势
//透明方式：Leaf节点也实现Add、Remove等方法，使得所有类都有统一的接口
//安全方式：Leaf节点不实现Add、Remove等方法，需要在使用时进行额外判断，相对麻烦
class Component//定义所有类共有接口
{
private:
	std::string m_objectName;
public:
	Component(std::string name) { m_objectName = name; }
	virtual void Add(Component* com) = 0;
	virtual void Remove(Component* com) = 0;
	virtual void DoSomething() = 0;
};
class Leaf : public Component//叶节点
{
public:
	Leaf(std::string name)
		: Component(name)
	{ return; }
	
	void Add(Component* com) override { return; }
	void Remove(Component* com) override { return; }
	void DoSomething()
	{
		//进行一些操作
	}
};
class Composite : public Component//枝节点
{
private:
	std::List<Component*> children;
public:
	Composite(std::string name)
		: Component(name)
	{ return; }
	
	void Add(Component* com) override { children.push_back(com); }
	void Remove(Component* com) override { children.remove(com); }
	void DoSomething()
	{
		//进行一些操作
		for(auto& child : children)
		{
			child->DoSomething();
		}
	}
};

{
	Composite* root = new Composite("root");
	root->Add(new Leaf("Leaf A"));//增加叶节点
	
	Composite* branch1 = new Composite("branch1");
	branch1->Add(new Leaf("Leaf A"));
	root->Add(branch1);//增加枝节点
	
	root->DoSomething();
	root->Remove(branch1);
}

//迭代器模式
//将遍历操作抽象并封装成Iterator类，该模式普遍存在与各种面向对象语言的标准库中，即容器遍历
//C++中一般使用模板类实现通用迭代器
//本例中迭代器为C++风格，简单描述vector数组迭代器
template<typename T>
class Iterator
{
public:
	virtual T begin();
	virtual T end();
	virtual T next();
	virtual void pop_back();
};
class Aggregate
{
public:
	virtual Iterator* CreateIterator();
};
template<typename T>
class ConcreteIterator : Iterator
{
private:
	ConcreteAggregate* dataSet;
	int currentIndex;
public:
	ConcreteIterator(ConcreteAggregate* data)
		: dataSet(data),
		currentIndex(0)
	{}
	T begin()
	{
		return (*dataSet)[0];
	}
	T next()
	{
		current++;
		if(current < dataSet.Count())
		{
			return (*dataSet)[current];
		}
	}
};
template<typename T>
class ConcreteAggregate
{
private:
	T[100] data;//假设容器大小固定
public:
	Iterator* CreateIterator()
	{
		return new ConcreteIterator(this);
	}
	int Count() { return 100; }
	T& operator[](int index)//返回引用
	{
		return data[index];
	}
};

{
	ConcreteAggregate set;
	
	set[0] = 0;
	set[1] = 1;
	
	Iterator* it = set.CreateIterator();
	std::cout << it->begin() << std::endl;
	std::cout << it->next() << std::endl;
}

//单例模式
//C++实现中可以有静态成员变量或指针变量，在多线程程序中需要加锁以及双重判空
//懒汉模式：用到时才初始化
//饿汉模式：定义变量时即初始化
//非线程安全+懒汉
class Singleton
{
public:
	static Singleton* GetInstance();
	~Singleton() {}
private:
	Singleton() {}//隐藏默认构造
	Singleton(const Singleton& obj) = delete;//删除拷贝构造
	Singleton& operator=(const Singleton& obj) = delete;//删除赋值构造
	
	static Singleton* m_pSingle;
};
Singleton* Singleton::m_pSingle = nullptr;//静态成员要在类外定义
Singleton* GetInstance()
{
	if(!m_pSingle)
	{
		m_pSingle = new Singleton;
	}
	return m_pSingle;
}
//线程安全+懒汉
std::mutex mtx;//互斥量
class Singleton
{
public:
	static Singleton* GetInstance();
	~Singleton() {}
private:
	Singleton() {}//隐藏默认构造
	Singleton(const Singleton& obj) = delete;//删除拷贝构造
	Singleton& operator=(const Singleton& obj) = delete;//删除赋值构造
	
	static Singleton* m_pSingle;
};
Singleton* Singleton::m_pSingle = nullptr;//静态成员要在类外定义
Singleton* GetInstance()
{
	if(!m_pSingle)//双重判空
	{
		std::lock_guard<std::mutex> lck(mtx);
		if(!m_pSingle)
		{
			m_pSingle = new Singleton;
		}
	}
	return m_pSingle;
}
//静态局部变量（懒汉）
//缺点：最好在单线程启动阶段调用并初始化
class Singleton
{
public:
    static Singleton& getInstance();
private:
    Singleton(){}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

Singleton& Singleton::getInstance()
{
    static Singleton singleton;
    return singleton;
}
//饿汉，定义变量时即初始化
class Singleton
{
public:
    static Singleton& getInstance();
private:
    Singleton(){}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
	
	static Singleton* m_pSingle;
};
Singleton* Singleton::m_pSingle = new Singleton();
Singleton* GetInstance()
{
	return m_pSingle;
}

//桥接模式
//将抽象与实现分离，意思是运用聚合、组合将B类作为A类的成员变量（A、B均为抽象类、接口），在A中定义调用B的方法，然后定义A、B类的子类并实现，这样由父类定义调用行为，子类定义具体实现
//适用场景：子类需要大量修改和扩展子类数量时，可以依赖抽象而不是具体实现，满足依赖倒转原则
//类似于策略模式，策略模式针对函数，桥接模式针对类型
class App
{
public:
	virtual void run() = 0;
};
class Phone
{
public:
	virtual void UseApp() { app->run(); }
private:
	App* app;
};
//此时要拓展子类数量，不会影响父类调用逻辑
class GameApp
{
public:
	void run() { std::cout << __FUNCTION__ << std::endl; }
};
class ApplePhone
{
public:
	ApplePhone(App* p) : app(p) { return; }
	virtual void UseApp() { app->run(); }
private:
	App* app;
};

{
	App* app = new GameApp();
	ApplePhone aphone(app);
	aphone.UseApp();
}

//命令模式
//将命令和执行者封装成一个请求对象，由Invoker负责管理请求（命令），实现了命令与执行者的低耦合
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

{
	Receiver* receiver = new Receiver;
	ConcreteCommand* cmd1 = new ConcreteCommand(receiver);
	Invoker ivk;

	ivk.SetCommand(cmd1);
	ivk.ExecuteCommand();

	delete cmd1;
	delete receiver;
}

//职责链模式
//让每个处理节点都保存下一个节点的指针或引用，使得当前节点处理不了的请求可以送给下一个节点处理，过程类似递归
//减少通过if来区分处理事件或处理者，降低耦合
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

{
	int request = 70;//请求可以根据业务需要改成类的形式
	Handler20* h20 = new Handler20;
	Handler60* h60 = new Handler60;
	Handler100* h100 = new Handler100;
	h20->SetSuccessor(h60);
	h60->SetSuccessor(h100);
	h100->SetSuccessor(nullptr);

	h20->HandleRequest(request);//从最初的节点开始尝试处理请求
}

//中介者模式
//将对象之间的交互行为提取出来封装到中介者类中
//缺点：中介者类可能会非常复杂
//当类型很多中介者类维护困难或类之间的交互是多对多时，需要考虑是否系统设计的有问题
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
		if (sender == colleague1)//ConcreteColleague1、ConcreteColleague2虽然有前向定义，但任然要在使用前进行实现，例如在使用前定义或调用include
		{
			colleague2->Receive(json);
		}
		else if (sender == colleague2)
		{
			colleague1->Receive(json);
		}
	}
};

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






















