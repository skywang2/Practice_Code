#pragma once

#include <qwidget.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qboxlayout.h>
#include <QSpacerItem> 

class myWidget : public QWidget
{
	Q_OBJECT

public:
	explicit myWidget(QWidget* parent = 0);
	~myWidget();
};
