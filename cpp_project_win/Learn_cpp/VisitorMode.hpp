#pragma once
#include <iostream>
#include <vector>

class ConcreteElementA;
class ConcreteElementB;

class Visitor
{
public:
	virtual void VisitConcreteElementA(ConcreteElementA* val) = 0;//对A具体类型的操作
	virtual void VisitConcreteElementB(ConcreteElementB* val) = 0;//对B具体类型的操作
};

class ConcreteVisitor1 : public Visitor
{
public:
	void VisitConcreteElementA(ConcreteElementA* val) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	void VisitConcreteElementB(ConcreteElementB* val) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};

class ConcreteVisitor2 : public Visitor
{
public:
	void VisitConcreteElementA(ConcreteElementA* val) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	void VisitConcreteElementB(ConcreteElementB* val) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};

class Element
{
public:
	virtual void Accept(Visitor* visitor) = 0;
};

class ConcreteElementA : public Element
{
public:
	void Accept(Visitor* visitor) override
	{
		visitor->VisitConcreteElementA(this);
	}
	void OtherFunction() { return; }//其他，凑数
};

class ConcreteElementB : public Element
{
public:
	void Accept(Visitor* visitor) override
	{
		visitor->VisitConcreteElementB(this);
	}
	void OtherFunction() { return; }//其他，凑数
};

class ObjectStructure//管理元素的集合
{
private:
	std::vector<std::shared_ptr<Element>> elements;
public:
	void Attach(std::shared_ptr<Element> e) { elements.push_back(e); }
	//为了简化操作，没有Remove函数
	void Accept(Visitor* visitor)
	{
		for (auto& e : elements)
		{
			e->Accept(visitor);
		}
	}
};
