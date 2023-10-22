#include "TitleBar.h"

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.close, &QPushButton::released, this, &TitleBar::SigCloseWidget);//转发关闭窗口信号
}

TitleBar::~TitleBar()
{}
