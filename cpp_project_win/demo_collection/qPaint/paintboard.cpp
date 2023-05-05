#include "paintboard.h"
#include <iostream>
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <qpushbutton.h>

using namespace std;

PaintBoard::PaintBoard(QWidget *parent)
	: QMainWindow(parent),
	mode(0),
	scale(1)
{
	//ui.setupUi(this);
	mode = 1;//0-绘制各种图案，1-绘制2d绘图板，2-双缓冲绘图
	resize(400, 400);
	m_pix = QPixmap(200, 200);
	m_pix.fill(Qt::white);

	QPushButton* btn = new QPushButton("zoomIn", this);
	btn->move(350, 350);
	connect(btn, &QPushButton::released, this, &PaintBoard::ZoomIn);
}

PaintBoard::~PaintBoard()
{

}

void PaintBoard::mousePressEvent(QMouseEvent *event)
{
	if (1 == mode)
	{
		if (event->button() == Qt::LeftButton)
		{
			m_lastPoint = event->pos();
		}
	}
	else if (2 == mode)
	{

	}
}

void PaintBoard::mouseMoveEvent(QMouseEvent *event)
{
	if (1 == mode)
	{
		if (event->buttons()&Qt::LeftButton)//注意是buttons()，该函数返回按键状态
		{
			m_endPoint = event->pos();
			update();
		}
	}
	else if (2 == mode)
	{

	}

}

void PaintBoard::mouseReleaseEvent(QMouseEvent *event)
{
	if (1 == mode)
	{
		if (event->button() == Qt::LeftButton)
		{
			m_endPoint = event->pos();
			update();
		}
	}
	else if (2 == mode)
	{

	}

}

//重写绘图事件函数，该事件在窗口第一次出现、窗口大小变化、获得或失去窗口焦点、调用update()等需要重绘的情况会被调用
void PaintBoard::paintEvent(QPaintEvent*) {
	if (1 == mode)
	{
		if (1 != scale)
		{
			QPixmap backPix(m_pix.size()*scale);
			QPainter tmpP(&backPix);
			tmpP.scale(scale, scale);
			tmpP.drawPixmap(0, 0, m_pix);//把临时画布上放大后的结果画到原画布
			m_pix = backPix;
			scale = 1;//重置scale
		}
		QPainter p1(&m_pix);
		p1.drawLine(m_lastPoint, m_endPoint);
		m_lastPoint = m_endPoint;
		QPainter p2(this);
		p2.drawPixmap(0, 0, m_pix);
		return;
	}
	else if (2 == mode)
	{

	}

	QPainter painter(this);
	
	//画一条直线
	painter.drawLine(QPoint(0, 0), QPointF(100, 100));

	//绘制文字
	QDateTime time = QDateTime::currentDateTimeUtc();
	QString timeStr = time.toString("hh:mm:ss.zzz");
	painter.drawText(QPointF(100, 100), timeStr);

	QPen pen;
	pen.setColor(QColor(255, 0, 0));

	QBrush brush;//注意：QBrush不带参数初始化时，style默认为Qt::NoBrush，不会填充选区
	brush.setColor(QColor(255, 228, 185, 255));
	brush.setStyle(Qt::SolidPattern);
	//brush.setStyle(Qt::HorPattern);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawRect(200, 20, 100, 50);

	//线性渐变
	QLinearGradient linearGradient(QPointF(120, 120), QPointF(220, 170));
	linearGradient.setColorAt(0, Qt::yellow);
	linearGradient.setColorAt(0.5, Qt::red);
	linearGradient.setColorAt(1, Qt::green);
	linearGradient.setSpread(QGradient::PadSpread);
	painter.setBrush(linearGradient);
	painter.drawRect(120, 120, 100, 50);

	//辐射渐变
	QRadialGradient radialGradient(QPointF(50, 230), 50, QPointF(50, 200));
	radialGradient.setColorAt(0, QColor(255, 255, 100, 150));
	radialGradient.setColorAt(1, QColor(0, 0, 0, 50));
	painter.setBrush(radialGradient);
	painter.drawEllipse(QPointF(50, 230), 50, 80);

	//锥形渐变
	QConicalGradient conicalGradient(QPointF(300, 300), 50);
	conicalGradient.setColorAt(0.2, Qt::cyan);
	conicalGradient.setColorAt(0.9, Qt::black);
	painter.setBrush(conicalGradient);
	painter.drawEllipse(QPointF(300, 300), 50, 50);

	//绘制路径
	QPainterPath path;
	path.addEllipse(250, 150, 50, 50);
	path.lineTo(350, 200);
	brush.setColor(QColor(238, 121, 66, 255));
	painter.drawPath(path);

	//绘制路径（扇形）
	QPainterPath path2;
	path2.moveTo(QPointF(400, 200));
	path2.arcTo(QRectF(300, 100, 200, 200), 45, 90);
	path2.closeSubpath();
	path2.lineTo(350, 200);
	brush.setColor(QColor(221, 160, 221));
	painter.setBrush(brush);
	painter.drawPath(path2);

	//绘制像素图
	QPixmap pix;
	pix.load(":/PaintBoard/qPaint.ico");
	//painter.translate(100, 100);//改变坐标原点，新的点会成为（0，0）
	painter.drawPixmap(500, 200, 100, 100, pix);
	pix.scaled(pix.width() * 5, pix.height() * 5, Qt::KeepAspectRatio);//, Qt::KeepAspectRatio
	painter.drawPixmap(500, 400, 300, 300, pix);

	pix.load(":/PaintBoard/qPaint.ico");
	painter.rotate(90);
	painter.drawPixmap(300, 200, pix);

}
