#pragma once

#include <QObject>
#include <QTest>
#include "ClassA.h"
#include "NormalClassB.h"

//��֯������������
class TestCaseClassA : public QObject
{
	Q_OBJECT

public:
	TestCaseClassA(QObject *parent=nullptr);
	~TestCaseClassA();

private slots://������slots����������ʾ�ۺ����ĺ꣬�������ܱ����Կ��ʶ��
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