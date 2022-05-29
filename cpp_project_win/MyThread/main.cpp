#include "MyThread.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyThread w;
	w.show();
	return a.exec();
}
