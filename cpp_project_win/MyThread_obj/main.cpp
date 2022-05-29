#include "MyThread_obj.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyThread_obj w;
	w.show();
	return a.exec();
}
