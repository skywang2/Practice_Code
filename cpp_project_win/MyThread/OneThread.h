#pragma once

#include <QThread>
#include <QDebug>

class OneThread : public QThread
{
	Q_OBJECT

public:
	OneThread(QObject *parent=nullptr);
	~OneThread();

	void run();

signals:
	//自定义信号
	void signalDone();
};
