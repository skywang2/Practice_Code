#pragma once
#include <QObject>

//测试用例类，他的private函数都是测试用例，可以被测试框架调用
class CommodityTest : public QObject
{
	Q_OBJECT
public:
	CommodityTest(QObject* parent = nullptr);
	~CommodityTest();

private slots:
	void Case1_MySplit();
};

class TestClassB : public QObject
{
	Q_OBJECT
public:
	TestClassB(QObject* parent = nullptr);
	~TestClassB();

	private slots:
	void Case1_MySplit();
};

