#include "OneThread.h"

OneThread::OneThread(QObject *parent)
	: QThread(parent)
{
}

OneThread::~OneThread()
{
}

void OneThread::run()
{
	msleep(5000);
	qDebug() << "[" << __FILE__ << "], Thread id:" << this->currentThreadId();

	emit signalDone();

}
