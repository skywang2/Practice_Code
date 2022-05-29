#pragma once

#include <QLabel>
#include <QMouseEvent>

class MyLabel : public QLabel
{
	Q_OBJECT

public:
	MyLabel(QWidget *parent);
	~MyLabel();

protected:
	//鼠标按下事件
	void mousePressEvent(QMouseEvent* ev);
	//鼠标松开事件
	void mouseReleaseEvent(QMouseEvent* ev);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent* ev);
	//光标进入窗口事件
	void MyLabel::enterEvent(QEvent* e);
	//光标离开窗口事件
	void MyLabel::leaveEvent(QEvent* e);
	//重写事件转发函数
	bool event(QEvent* e);

};
