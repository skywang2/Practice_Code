#pragma once

#include <QObject>
#include <QLabel>
#include <QDebug>

class MyLabel2 : public QLabel
{
	Q_OBJECT

public:
	MyLabel2(QWidget *parent);
	~MyLabel2();

protected:
	//鼠标按下事件
	//void mousePressEvent(QMouseEvent* ev);
	//光标进入窗口事件
	//void enterEvent(QEvent* e);
	//光标离开窗口事件
	//void leaveEvent(QEvent* e);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent* ev);

};
