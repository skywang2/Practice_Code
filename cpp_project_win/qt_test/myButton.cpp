#include "myButton.h"

myButton::myButton(QWidget *parent):QPushButton(parent)
{

}

myButton::~myButton()
{
	qDebug() << "button over";
}