#pragma once

#include <QWidget>
#include "ui_TitleBar.h"

class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

signals:
	void SigCloseWidget();

private:
	Ui::TitleBarClass ui;
};
