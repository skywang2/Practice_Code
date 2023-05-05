#include "qtestmodelview.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTestModelView w;
	w.show();
	return a.exec();
}
