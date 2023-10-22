#pragma once

#include <QWidget>
#include "ui_FramelessWindow.h"

enum Location {
	TOP = 1,
	RIGHT,
	BOTTOM,
	LEFT,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	CENTER
};

class FramelessWindow : public QWidget
{
	Q_OBJECT

public:
	FramelessWindow(QWidget *parent = nullptr);
	~FramelessWindow();

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;

	void appendStyleSheet(const QString& stylesheetFile);
	void setCursorShape(const QPoint& pos);

private:
	Ui::FramelessWindowClass ui;
	bool m_isLeftPressed;
	QPoint m_mouseOffset;
	Location m_location;
};
