#pragma once

#include <QtWidgets/QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QDateTime>
#include <QEvent>
#include <QDialog>
#include <QMessageBox>
#include "ui_MyEvent.h"

class MyEvent : public QWidget
{
	Q_OBJECT

public:
	MyEvent(QWidget *parent = Q_NULLPTR);
	~MyEvent();

private:
	Ui::MyEventClass ui;

protected:
	//键盘按下事件
	void keyPressEvent(QKeyEvent* e);
	//定时器事件
	void MyEvent::timerEvent(QTimerEvent* e);
	//关闭事件
	void closeEvent(QCloseEvent* e);
	//重写事件转发函数
	bool event(QEvent* e);
	//重写事件过滤器
	bool eventFilter(QObject* obj, QEvent* e);
};
