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
	//?Զ????ź?
	void signalDone();
};
