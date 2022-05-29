#pragma once

#include <qwidget.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qboxlayout.h>

class myWidget : public QWidget
{
	Q_OBJECT

public:
	myWidget(QWidget *parent);
	~myWidget();
};
