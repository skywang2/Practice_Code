#pragma once

#include <QtWidgets/QWidget>
#include <QObject>
#include <QThread>
#include <QEvent>
#include "ui_MyThread_obj.h"
#include "OneThread_obj.h"

class MyThread_obj : public QWidget
{
	Q_OBJECT

public:
	MyThread_obj(QWidget *parent = Q_NULLPTR);
	OneThread_obj* thread_obj;
	QThread* thread_1;

	void onReleasedBtn();
	void slotsDealSig();

private:
	Ui::MyThread_objClass ui;

protected:
	void closeEvent(QCloseEvent* e);
signals:
	void doOneThread();	//开启线程处理函数的信号
};
