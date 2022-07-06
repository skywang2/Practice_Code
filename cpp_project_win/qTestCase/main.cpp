#include <QtCore/QCoreApplication>
#include <QTest>
#include <QStringList>
#include "TestCaseClassA.h"

int main(int argc, char *argv[])
{
    int status = 0;

    {
        TestCaseClassA tc;
        QStringList param{"-vs", "-txt", "-o", "TestCaseClassA.txt"};
        status |= QTest::qExec(&tc, param);
    }
    {
        TestCaseNormalClassB tc;
        QStringList param{ "-vs", "-xml", "-o", "TestCaseNormalClassB.txt" };
        status |= QTest::qExec(&tc, param);
    }

    system("pause");
    return status;
}

//QTEST_MAIN(TestCaseClassA)
