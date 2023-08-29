#pragma once

#include <QObject>
#include <QString>
#include <QDebug>

//测试项基类
class TestCaseBase : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool IsFinished READ Finish WRITE SetFinish)//注册属性，可以用别名，和变量名不同
public:
	TestCaseBase(QObject* parent = nullptr)
		: QObject(parent),
		m_endFlag(false)
	{
		return;
	}

	Q_INVOKABLE bool Finish() { return m_endFlag; }
	Q_INVOKABLE void SetFinish(bool f) { m_endFlag = f; }
	virtual void DoSomething() { qDebug() << __FUNCTION__; }

private:
	bool m_endFlag;//结束标记
};

//测试项01
class TestCase01 : public TestCaseBase
{
	Q_OBJECT
public:
	Q_INVOKABLE TestCase01(QObject* parent = nullptr)
		: TestCaseBase(parent)
	{
		qDebug() << QString("parent:0x%1").arg((unsigned int)parent, 0, 16);
		return;
	}

	Q_INVOKABLE void DoSomething() { qDebug() << __FUNCTION__; }//执行具体测试
};

enum TestCaseType
{
	Case01 = 0,
};

//简单工厂
class SimpleFactory
{
	//使用默认构造函数和析构函数
public:
	static TestCaseBase* CreateObj(int type, QObject* parent = nullptr)
	{
		TestCaseBase* obj = nullptr;
		switch (type)
		{
		case Case01:
			obj = new TestCase01();//向上转型
			break;
		//case SubClassB:
		//	obj = new TestCase02();
		//	break;
		default:
			break;
		}
		return obj;
	}
};

//简单工厂，使用字符串来选择要生成的对象
class SimpleFactoryMate
{
public:
	SimpleFactoryMate()
	{
	}

	static TestCaseBase* CreateObj(const QByteArray& className, QObject* parent = nullptr)
	{
		TestCaseBase* obj = nullptr;

		int id = QMetaType::type(className);
		if (QMetaType::UnknownType != id)
		{
			//qDebug() << "typeName:" << QMetaType::typeName(id);
			try
			{
				const QMetaObject* mate = QMetaType::metaObjectForType(id);
				qDebug() << "typeName:" << mate->className();
				obj = dynamic_cast<TestCaseBase*>(mate->newInstance(Q_ARG(QObject*, parent)));
			}
			catch (std::bad_cast err)
			{
				qDebug() << err.what();
			}
		}
		else
		{
			qDebug() << QString("class name '%1' is unknow").arg(QString(className));
		}

		return obj;
	}
};

