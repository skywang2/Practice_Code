#pragma once

#include <QObject>
#include <QTest>
#include "ClassA.h"
#include "NormalClassB.h"

//组织测试用例的类
class TestCaseClassA : public QObject
{
	Q_OBJECT

public:
	TestCaseClassA(QObject *parent=nullptr);
	~TestCaseClassA();

private slots://必须用slots或者其他表示槽函数的宏，用例才能被测试框架识别
	void CaseGetTrue()
	{
		ClassA test;
		QVERIFY(true == test.GetTrue());
	}

	void CaseGetFalse()
	{
		ClassA test;
		QVERIFY(true == test.GetFalse());
	}

	void CaseGetInt()
	{
		ClassA test;
		QVERIFY(2 == test.GetInt(2));
	}

};

class TestCaseNormalClassB : public QObject
{
	Q_OBJECT

public:
	TestCaseNormalClassB(QObject* parent = nullptr) {};
	~TestCaseNormalClassB() {};

private slots:
	void CaseGetTrue()
	{
		NormalClassB test;
		QVERIFY(true == test.GetTrue());
	}

};
