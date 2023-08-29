#include "FramelessWindow.h"
#include "MainWindow.h"
#include "TitleBar.h"

/*
窗体结构：
FramelessWindow
-MainWindow
-TitleBar
*/

FramelessWindow::FramelessWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//使用ui中的layout
	MainWindow* m = new MainWindow(this);
	TitleBar* title = new TitleBar(this);
	ui.titleLayout->addWidget(title);
	ui.mainLayout->addWidget(m);
	//使用自己代码创建的layout
	//QGridLayout* layout = new QGridLayout(this);
	//layout->addWidget(title, 0, 0);
	//layout->addWidget(m, 0, 1);
	//this->setLayout(layout);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

FramelessWindow::~FramelessWindow()
{}
