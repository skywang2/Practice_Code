#include "MyEvent.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyEvent w;
	w.show();
	return a.exec();
}
