#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent)
	: QLabel(parent)
{
	//设置默认追踪鼠标
	this->setMouseTracking(true);
}

MyLabel::~MyLabel()
{
}

void MyLabel::mousePressEvent(QMouseEvent* ev)
{
	int i = ev->x();
	int j = ev->y();

	QString txt = QString("<center><h1>mouse press:(%1, %2)"
		"</h1></center>").arg(i).arg(j);

	//若单击事件由event函数接收，该处理函数可由双击事件触发
	//this->setText("22222222");

	this->setText(txt);
}

void MyLabel::mouseReleaseEvent(QMouseEvent* ev)
{
	QString txt = QString("<center><h1>mouse release:(%1, %2)"
		"</h1></center>").arg(ev->x()).arg(ev->y());
	
	//this->setText(txt);
}

void MyLabel::mouseMoveEvent(QMouseEvent* ev)
{
	QString txt = QString("<center><h1>mouse move:(%1, %2)"
		"</h1></center>").arg(ev->x()).arg(ev->y());
	
	//this->setText(txt);
}

void MyLabel::enterEvent(QEvent* e)
{
	QString txt = QString("<center><h1>mouse enter"
		"</h1></center>");

	this->setText(txt);
}

void MyLabel::leaveEvent(QEvent* e)
{
	QString txt = QString("<center><h1>mouse leave"
		"</h1></center>");

	this->setText(txt);

}

bool MyLabel::event(QEvent* e)
{
	//事件分发
	switch (e->type())
	{
	case QEvent::MouseButtonPress:
		mousePressEvent(static_cast<QMouseEvent*>(e));
		return true;
		break;	
	case QEvent::MouseButtonDblClick:
		mousePressEvent(static_cast<QMouseEvent*>(e));
		return true;
		break;
	case QEvent::MouseButtonRelease:
		mouseReleaseEvent(static_cast<QMouseEvent*>(e));
		return true;
		break;
	case QEvent::MouseMove:
		mouseMoveEvent(static_cast<QMouseEvent*>(e));
		return true;
		break;
	case QEvent::Enter:
		enterEvent(e);
		return true;
		break;
	case QEvent::Leave:
		leaveEvent(e);
		return true;
		break;

	default:
		return QWidget::event(e);	//没有自定义处理的事件使用widget的默认处理
	}

}
