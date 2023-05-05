#include "qapp_example.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	setbuf(stdout, NULL);
	//setvbuf(stdout, NULL, _IONBF, 0);
	printf("printf, %s\n", __FUNCTION__);
	fprintf(stdout, "fprintf, %s\n", __FUNCTION__);
	fflush(stdout);
	std::cout << "cout, " << __FUNCTION__ << std::endl;

	qApp_example w;
	w.show();
	return a.exec();
}
