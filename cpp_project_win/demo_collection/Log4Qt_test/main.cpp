#include <QtCore/QCoreApplication>
#include "define.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	LOG_INIT("./testLog4qt.txt");

	LOG_DEBUG(__FUNCTION__);


	return a.exec();
}
