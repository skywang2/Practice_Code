#include "MyLabel2.h"

MyLabel2::MyLabel2(QWidget *parent)
	: QLabel(parent)
{
}

MyLabel2::~MyLabel2()
{
}

void MyLabel2::mouseMoveEvent(QMouseEvent* ev)
{
	setText("<center><h1>mouse move event"
		"</h1></center>");

}

//void MyLabel2::mousePressEvent(QMouseEvent* ev)
//{
//	this->setText("<center><h1>label_2 mousePressEvent"
//		"</h1></center>");
//}
//
//void MyLabel2::enterEvent(QEvent* e)
//{
//	QString txt = QString("<center><h1>mouse enter"
//		"</h1></center>");
//
//	this->setText(txt);
//}
//
//void MyLabel2::leaveEvent(QEvent* e)
//{
//	QString txt = QString("<center><h1>mouse leave"
//		"</h1></center>");
//
//	this->setText(txt);
//
//}
