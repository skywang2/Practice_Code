#include "qthreadpool.h"
#include <chrono>
#include <thread>
#include <QObject>
#include <QPushButton>
#include <QThread>
#include <QRunnable>
#include <QDebug>
#include <Windows.h>
#include "helloworldtask.h"


qThreadPool::qThreadPool(QWidget *parent)
	: QWidget(parent)
{
	this->resize(500, 500);
	QPushButton* btn01 = new QPushButton("start", this);
	connect(btn01, &QPushButton::released, this, &qThreadPool::OnPushButtonReleased);
	DWORD id = GetCurrentThreadId();

	m_pool.setMaxThreadCount(3);
}

qThreadPool::~qThreadPool()
{

}

void qThreadPool::OnPushButtonReleased()
{
	qDebug() << "button";
	static int count = 0;

	HelloWorldTask *hello = new HelloWorldTask();
	connect(hello, &HelloWorldTask::SigEnd, this, &qThreadPool::SlotStart);
	hello->setAutoDelete(true);
	hello->SetIndex(count++);

	auto pool = QThreadPool::globalInstance();
	pool->start(hello);
	//m_pool.start(hello);
}

void qThreadPool::SlotStart(int threadIdx)
{
	qDebug() << "end idx:" << threadIdx;
}
