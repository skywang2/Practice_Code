#include "FramelessWindow.h"
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include "MainWindow.h"
#include "TitleBar.h"

#define PADDING 10 //定义能改变鼠标形状的边框宽度

/*
窗体结构：
FramelessWindow
-MainWindow
-TitleBar
*/

FramelessWindow::FramelessWindow(QWidget *parent)
	: QWidget(parent),
	m_isLeftPressed(false)
{
	ui.setupUi(this);

	//使用ui中的layout
	MainWindow* main = new MainWindow(this);
	TitleBar* title = new TitleBar(this);
	ui.titleLayout->addWidget(title);
	ui.mainLayout->addWidget(main);
	main->setMouseTracking(true);
	title->setMouseTracking(true);

	// 去除标题栏
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

	// 设置qss
	this->appendStyleSheet(":/qss/resource/qss/FramelessWindow.qss");

	// 设置信号
	connect(title, &TitleBar::SigCloseWidget, this, &QWidget::close);
}

FramelessWindow::~FramelessWindow()
{}

void FramelessWindow::mousePressEvent(QMouseEvent * event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		m_isLeftPressed = true;
		m_mouseOffset = event->globalPos() - this->frameGeometry().topLeft();// globalPos原点是屏幕左上角，frameGeometry.topleft是窗口边框左上角
		break;
	default:
		break;
	}
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		m_isLeftPressed = false;
		break;
	default:
		break;
	}
}

void FramelessWindow::mouseMoveEvent(QMouseEvent* event)
{
	QPoint globalPos = event->globalPos();

	// 鼠标未按下
	if (!m_isLeftPressed)
	{
		setCursorShape(globalPos);
		static int t = 0;
		qDebug() << "t:" << t++ << ", " << m_location;
		return;
	}
	// 鼠标按下（按下时不改变形状），并且在中间区域（可以移动窗口）
	if (m_isLeftPressed && CENTER == m_location)
	{
		this->move(globalPos - m_mouseOffset);// move是以边框左上角为基准计算的
		event->accept();
	}
	// 鼠标按下，在边缘位置，窗口缩放
}

void FramelessWindow::appendStyleSheet(const QString& stylesheetFile)
{
	QFile file(stylesheetFile);
	if (file.open(QFile::ReadOnly))
	{
		QString stylesheet = this->styleSheet();
		stylesheet += QLatin1String(file.readAll());
		this->setStyleSheet(stylesheet);
		file.close();
	}
	else
	{
		QMessageBox::information(this, "info", "Open qss file error.");
	}
}

void FramelessWindow::setCursorShape(const QPoint& pos)
{
	QRect rect = this->rect();// 不包括边框
	QPoint topleft = mapToGlobal(rect.topLeft());
	QPoint bottomRight = mapToGlobal(rect.bottomRight());

	int x = pos.x();
	int y = pos.y();

	// 判断鼠标在边框的哪个区域
	if (x >= topleft.x() && x <= topleft.x() + PADDING
		&& y >= topleft.y() && y <= topleft.y() + PADDING)// 左上
	{
		m_location = TOP_LEFT;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= topleft.x() + PADDING && x <= bottomRight.x() - PADDING
		&& y >= topleft.y() && y <= topleft.y() + PADDING)// 上
	{
		m_location = TOP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (x >= bottomRight.x() - PADDING && x <= bottomRight.x()
		&& y >= topleft.y() && y <= topleft.y() + PADDING)// 右上
	{
		m_location = TOP_RIGHT;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x >= bottomRight.x() - PADDING && x <= bottomRight.x()
		&& y >= topleft.y() + PADDING && y <= bottomRight.y() - PADDING)// 右
	{
		m_location = RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (x >= bottomRight.x() - PADDING && x <= bottomRight.x()
		&& y >= bottomRight.y() - PADDING && y <= bottomRight.y())// 右下
	{
		m_location = BOTTOM_RIGHT;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x >= topleft.x() + PADDING && x <= bottomRight.x() - PADDING
		&& y >= bottomRight.y() - PADDING && y <= bottomRight.y())// 下
	{
		m_location = BOTTOM;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (x >= topleft.x() && x <= topleft.x() + PADDING
		&& y >= bottomRight.y() - PADDING && y <= bottomRight.y())// 左下
	{
		m_location = BOTTOM_LEFT;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (x >= topleft.x() && x <= topleft.x() + PADDING
		&& y >= topleft.y() + PADDING && y <= bottomRight.y() - PADDING)// 左
	{
		m_location = LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else
	{
		m_location = CENTER;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}
