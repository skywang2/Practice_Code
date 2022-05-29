#pragma once

#include <QObject>
#include <QThread>
#include <QDebug>

class OneThread_obj : public QObject
{
	Q_OBJECT

public:
	OneThread_obj(QObject *parent=0);
	~OneThread_obj();

	void setFlag(bool flag = true);
	bool getFlag();

	void ttttt();	//线程处理函数

signals:
	void mySignal();

private:
	bool isStop;
};
