#include "sql.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SQL w;
	w.show();
	return a.exec();
}
