//练习设计模式
//代码基本来自《大话设计模式》每篇的总结，代码使用C++

//简单工厂模式
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

//1.经典策略模式，封装一套调用算法的统一接口
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







