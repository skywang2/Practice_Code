#ifndef HELLOWORLDTASK_H
#define HELLOWORLDTASK_H
#include <chrono>
#include <thread>
#include <QObject>
#include <QRunnable>
#include <QDebug>
#include <Windows.h>

class HelloWorldTask : public QObject, public QRunnable//没有继承QObject没法使用信号
{
	Q_OBJECT

public:
	HelloWorldTask(QObject* parent = nullptr)
		: QObject(parent),
		m_index(-1)
	{
		;
	}

	int m_index;

	void SetIndex(int threadIdx)
	{
		DWORD id = GetCurrentThreadId();
		m_index = threadIdx;
	}

	void run()
	{
		//qDebug() << "Hello world from thread start" << QThread::currentThread();
		DWORD id = GetCurrentThreadId();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		//qDebug() << "Hello world from thread end" << QThread::currentThread();// << ", used time:" << ;
		emit SigEnd(m_index);
	}
signals:
	void SigEnd(int threadIdx);
};

#endif
