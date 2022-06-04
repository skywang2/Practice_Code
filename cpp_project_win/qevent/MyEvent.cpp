#include "MyEvent.h"

MyEvent::MyEvent(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//this->startTimer(1000);		//1000ms����һ��

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
	QMessageBox::StandardButton ret = QMessageBox::question(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ�ر�"));
	switch (ret)
	{
	case QMessageBox::Yes:
		break;
	case QMessageBox::No:
		e->ignore();	//�˴������źŵ���������widget,ignore����������ܹ����մ��¼�,���൱�������������ӵ��ر��¼�
		break;
	}
	//dlg.resize(200, 100);
	//dlg.exec();
	//qDebug() << "next dlg";

}


bool MyEvent::event(QEvent* e)
{
	//�¼��ַ�
	//ֻ���ض����¼��ַ�����д���¼���������
	//�����¼�����Ĭ��
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
		return QWidget::event(e);	//û���Զ��崦�����¼�ʹ��widget��Ĭ�ϴ���
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
