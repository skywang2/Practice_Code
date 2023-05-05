#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include "ui_paintboard.h"

class PaintBoard : public QMainWindow
{
	Q_OBJECT

public:
	PaintBoard(QWidget *parent = 0);
	~PaintBoard();
	void ZoomIn(){ scale *= 2; update(); }
protected:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);

private:
	Ui::PaintBoardClass ui;
	QPixmap m_pix;
	QPoint m_lastPoint;
	QPoint m_endPoint;

	int mode;
	int scale;
};

#endif // PAINTBOARD_H
