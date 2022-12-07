#include <QtCore/QCoreApplication>
#include <QString>
#include <QDebug>
#include <QMetaProperty>
#include "TestCase.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestCase01 case01_1;
    const QMetaObject* mate = case01_1.metaObject();

    //1.遍历类的属性
    int propertyCnt = mate->propertyCount();
    for (int i = 0; i < propertyCnt; ++i)
    {
        QMetaProperty oneProperty = mate->property(i);
        qDebug() << "name:" << oneProperty.name();
        qDebug() << "type:" << QVariant::typeToName(oneProperty.type()) << endl;
    }

    //2.遍历类的函数成员
    int methodCnt = mate->methodCount();
    for (int idx = 0; idx < methodCnt; ++idx)
    {
        QMetaMethod oneMethod = mate->method(idx);
        //qDebug() << "--------begin-------";
        //qDebug() << "typeName:" << oneMethod.typeName();
        //qDebug() << "signature:" << oneMethod.methodSignature();
        //qDebug() << "methodType:" << oneMethod.methodType();//函数类型：槽、信号、普通函数
        //qDebug() << "--------end---------" << endl;
    }

    //3.使用反射
    qDebug() << "-------test property-----------";
    TestCase01 case01_2;
    case01_2.setProperty("IsFinished", true);
    qDebug() << QString("IsFinished:%1").arg(case01_2.property("IsFinished").toBool());
    qDebug() << case01_2.Finish();
    qDebug() << "--------end----------" << endl;

    //4.应用反射创建对象
    qRegisterMetaType<TestCase01>("TestCase01");

    TestCaseBase* case01_3 = SimpleFactoryMate::CreateObj("TestCase01");
    if (case01_3)
    {
        case01_3->DoSomething();
    }

    return a.exec();
}
