#include "CommodityTest.h"
#include <QTest>
#include <QStringList>

int main(int argc, char *argv[])
{
	int status = 0;
	{
		CommodityTest tc;
		QStringList param{ "-vs", "-txt", "-o", "testcases01.txt" };
		status |= QTest::qExec(&tc, param);
	}
	{
		TestClassB tc;
		QStringList param{ "-vs", "-xml", "-o", "testcases02.txt" };
		status |= QTest::qExec(&tc, param);
	}
	return status;
}

//QTEST_MAIN(CommodityTest)
