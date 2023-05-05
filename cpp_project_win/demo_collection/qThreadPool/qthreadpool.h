#ifndef QTHREADPOOLPROJ_H
#define QTHREADPOOLPROJ_H

#include <QtWidgets/QWidget>
#include <QThreadPool>

class qThreadPool : public QWidget
{
	Q_OBJECT

public:
	qThreadPool(QWidget *parent = 0);
	~qThreadPool();

signals:
private:
	QThreadPool m_pool;

	void OnPushButtonReleased();
	void SlotStart(int threadIdx);
};

#endif // QTHREADPOOL_H
