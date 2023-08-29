#pragma once

#include <QWidget>
#include "ui_TitleBar.h"

class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

private:
	Ui::TitleBarClass ui;
};
