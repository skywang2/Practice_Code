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
	//��갴���¼�
	void mousePressEvent(QMouseEvent* ev);
	//����ɿ��¼�
	void mouseReleaseEvent(QMouseEvent* ev);
	//����ƶ��¼�
	void mouseMoveEvent(QMouseEvent* ev);
	//�����봰���¼�
	void MyLabel::enterEvent(QEvent* e);
	//����뿪�����¼�
	void MyLabel::leaveEvent(QEvent* e);
	//��д�¼�ת������
	bool event(QEvent* e);

};