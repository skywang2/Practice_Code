#include "MyThread.h"

MyThread::MyThread(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	myTimer = new QTimer(this);		//创建定时器
	oneThread = new OneThread();	//创建任务线程

	bool ret;
	ret = connect(myTimer, &QTimer::timeout, this, &MyThread::dealTimeout);
	ret = connect(ui.pushButton, &QPushButton::released, this, &MyThread::onReleasedBtn);
	ret = connect(oneThread, &OneThread::signalDone, this, &MyThread::threadOver);
	
	ret = connect(this, &MyThread::destroyed, this, &MyThread::stopThread);
}	
void MyThread::dealTimeout()
{
	static int i = 0;
	i++;
	qDebug() << "timeout i:" << i;

	ui.lcdNumber->display(i);
}

void MyThread::threadOver()
{
	qDebug() << "threadOver...";
	myTimer->stop();
}

void MyThread::stopThread()
{
	oneThread->quit();
	oneThread->wait();
	qDebug() << "stopThread()";
}

void MyThread::onReleasedBtn()
{
	//if (myTimer->isActive())
	//	return;
	//qDebug() << "btn i:" << i;
	//myTimer->setInterval(1000);
	//myTimer->start();

	if (myTimer->isActive() == false)
	{
		myTimer->start(500);
	}

	oneThread->start();

	//myTimer->stop();
	//qDebug() << "stop";


}
