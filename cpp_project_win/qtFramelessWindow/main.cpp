#include <QtWidgets/QApplication>
#include <QGridLayout>
#include "FramelessWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置样式表

    FramelessWindow w;
    w.show();
    return a.exec();
}
