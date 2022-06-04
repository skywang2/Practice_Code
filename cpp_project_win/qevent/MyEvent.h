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
	//���̰����¼�
	void keyPressEvent(QKeyEvent* e);
	//��ʱ���¼�
	void MyEvent::timerEvent(QTimerEvent* e);
	//�ر��¼�
	void closeEvent(QCloseEvent* e);
	//��д�¼�ת������
	bool event(QEvent* e);
	//��д�¼�������
	bool eventFilter(QObject* obj, QEvent* e);
};