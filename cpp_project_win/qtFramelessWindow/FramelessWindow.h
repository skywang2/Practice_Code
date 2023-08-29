#pragma once

#include <QWidget>
#include "ui_FramelessWindow.h"

class FramelessWindow : public QWidget
{
	Q_OBJECT

public:
	FramelessWindow(QWidget *parent = nullptr);
	~FramelessWindow();

private:
	Ui::FramelessWindowClass ui;
};
