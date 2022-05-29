#include "MyEvent.h"

MyEvent::MyEvent(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//this->startTimer(1000);		//1000ms触发一次

	ui.label_2->installEventFilter(this);
}

MyEvent::~MyEvent()
{

}

void MyEvent::keyPressEvent(QKeyEvent *e)
{
	qDebug() << (char)e->key();
}

void MyEvent::timerEvent(QTimerEvent* e)
{
	static int sec = 0;
	QDateTime dateTime(QDateTime::currentDateTime());
	QString time = dateTime.toString("hh:mm:ss");
	ui.label->setText(
		QString("<center><h1>time out, %1</h1></center>").arg(time)
					);
}

void MyEvent::closeEvent(QCloseEvent* e)
{
	QMessageBox::StandardButton ret = QMessageBox::question(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否关闭"));
	switch (ret)
	{
	case QMessageBox::Yes:
		break;
	case QMessageBox::No:
		e->ignore();	//此处接收信号的是最外层的widget,ignore后不再有组件能够接收此事件,故相当于整个程序无视掉关闭事件
		break;
	}
	//dlg.resize(200, 100);
	//dlg.exec();
	//qDebug() << "next dlg";

}


bool MyEvent::event(QEvent* e)
{
	//事件分发
	//只有特定的事件分发给重写的事件函数处理
	//其余事件保持默认
	switch (e->type())
	{
	case QEvent::Close:
		closeEvent(static_cast<QCloseEvent*>(e));
		return true;
		break;
	case QEvent::MouseMove:
		mouseMoveEvent(static_cast<QMouseEvent*>(e));
		return true;
		break;
	default:
		return QWidget::event(e);	//没有自定义处理的事件使用widget的默认处理
	}

}

bool MyEvent::eventFilter(QObject* obj, QEvent* e)
{
	if (obj == ui.label_2)
	{
		if (e->type() == QEvent::MouseMove)
		{
			QMouseEvent* eMouse = static_cast<QMouseEvent*>(e);
			ui.label_2->setText("<center><h1>***eventFilter***</h1></center>");
			return true;
		}
		else if(e->type() == QEvent::Leave)
		{
			QMouseEvent* eMouse = static_cast<QMouseEvent*>(e);
			ui.label_2->setText("<center><h1>leave</h1></center>");
			return true;
		}
		else if (e->type() == QEvent::Enter)
		{
			QMouseEvent* eMouse = static_cast<QMouseEvent*>(e);
			ui.label_2->setText("<center><h1>enter</h1></center>");
			return true;
		}

	}
	return QWidget::eventFilter(obj, e);
}

