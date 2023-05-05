#ifndef QAPP_EXAMPLE_H
#define QAPP_EXAMPLE_H

#include <QtWidgets/QMainWindow>
#include "ui_qapp_example.h"

class qApp_example : public QMainWindow
{
	Q_OBJECT

public:
	qApp_example(QWidget *parent = 0);
	~qApp_example();

private:
	Ui::qApp_exampleClass ui;
};

#endif // QAPP_EXAMPLE_H
