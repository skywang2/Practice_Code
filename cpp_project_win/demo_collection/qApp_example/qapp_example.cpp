#include "qapp_example.h"
#include <cstdio>
#include <iostream>
#include <QDebug>

qApp_example::qApp_example(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	qDebug() << "qt:" << __FUNCTION__ << endl;
	printf("printf, %s\n", __FUNCTION__);
	fprintf(stdout, "fprintf, %s\n", __FUNCTION__);
	fflush(stdout);
	std::cout << "cout, " << __FUNCTION__ << std::endl;


}

qApp_example::~qApp_example()
{

}
