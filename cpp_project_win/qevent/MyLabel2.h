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
	//��갴���¼�
	//void mousePressEvent(QMouseEvent* ev);
	//�����봰���¼�
	//void enterEvent(QEvent* e);
	//����뿪�����¼�
	//void leaveEvent(QEvent* e);
	//����ƶ��¼�
	void mouseMoveEvent(QMouseEvent* ev);

};