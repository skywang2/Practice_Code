#pragma once
#include <iostream>
#include <vector>

class ConcreteElementA;
class ConcreteElementB;

class Visitor
{
public:
	virtual void VisitConcreteElementA(ConcreteElementA* val) = 0;//��A�������͵Ĳ���
	virtual void VisitConcreteElementB(ConcreteElementB* val) = 0;//��B�������͵Ĳ���
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
	void OtherFunction() { return; }//����������
};

class ConcreteElementB : public Element
{
public:
	void Accept(Visitor* visitor) override
	{
		visitor->VisitConcreteElementB(this);
	}
	void OtherFunction() { return; }//����������
};

class ObjectStructure//����Ԫ�صļ���
{
private:
	std::vector<std::shared_ptr<Element>> elements;
public:
	void Attach(std::shared_ptr<Element> e) { elements.push_back(e); }
	//Ϊ�˼򻯲�����û��Remove����
	void Accept(Visitor* visitor)
	{
		for (auto& e : elements)
		{
			e->Accept(visitor);
		}
	}
};