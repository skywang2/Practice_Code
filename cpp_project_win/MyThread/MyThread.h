#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include "ui_MyThread.h"
#include "OneThread.h"

class MyThread : public QMainWindow
{
	Q_OBJECT

public:
	explicit MyThread(QWidget *parent = Q_NULLPTR);

	void dealTimeout();
	void threadOver();
	void stopThread();
	void onReleasedBtn();

private:
	Ui::MyThreadClass ui;

	QTimer* myTimer;
	OneThread* oneThread;
};
