#include "MyThread_obj.h"

MyThread_obj::MyThread_obj(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//给新线程分配空间
	//不能指定父对象，有父对象会导致无法调用moveToThread()
	thread_obj = new OneThread_obj();
	//创建子线程
	thread_1 = new QThread(this);
	//将自定义线程加入到子线程中
	thread_obj->moveToThread(thread_1);
	connect(thread_1, QThread::finished, thread_obj, QObject::deleteLater);

	bool ret;
	//按钮信号
	ret = connect(ui.pushButton, &QPushButton::released, this, &MyThread_obj::onReleasedBtn);
	//接收自定义子线程返回的信号
	//ret = connect(thread_obj, &OneThread_obj::mySignal, this, &MyThread_obj::slotsDealSig);
	//启动线程处理函数
	ret = connect(this, &MyThread_obj::doOneThread, thread_obj, &OneThread_obj::ttttt);
	//***connect()的第五个参数，连接方式：默认，队列，直接***
	//***多线程时才有意义***
	//***多线程默认为队列，单线程默认为直接***
	//***队列：槽函数所在线程与接收者一样
	//***直接：槽函数所在线程与发送者一样

}

void MyThread_obj::onReleasedBtn()
{
	qDebug() << "[" << __FILE__ << "], Thread id:" << QThread::currentThreadId();

	//启动子线程
	thread_1->start();
	thread_obj->setFlag(false);
	//启动自定义子线程处理函数（不能直接调用，会导致线程处理函数在主线程中运行）
	//只能通过信号和槽来调用线程处理函数
	//线程处理函数中不能处理图形界面(可以考虑发信号让主线程处理)
	//用信号方式启动是因为connect()的第五个参数会影响槽函数所属的线程
	emit doOneThread();
}

void MyThread_obj::slotsDealSig()
{
	//qDebug() << "[" << __FILE__ << "], Thread id:" << QThread::currentThread();
	//thread_1->quit();
}

void MyThread_obj::closeEvent(QCloseEvent* e)
{
	thread_obj->setFlag(true);
	thread_1->quit();
	thread_1->wait();
	//e->ignore();
}
