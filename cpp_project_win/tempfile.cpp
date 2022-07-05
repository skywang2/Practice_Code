//练习设计模式
//代码基本来自《大话设计模式》每篇的总结，代码使用C++

//简单工厂模式
class SimpleFactory
{
  //使用默认构造函数和析构函数
  
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
