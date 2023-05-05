#include "paintboard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PaintBoard w;
	w.show();
	return a.exec();
}
