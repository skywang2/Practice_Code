#include "FramelessWindow.h"
#include "MainWindow.h"
#include "TitleBar.h"

/*
����ṹ��
FramelessWindow
-MainWindow
-TitleBar
*/

FramelessWindow::FramelessWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//ʹ��ui�е�layout
	MainWindow* m = new MainWindow(this);
	TitleBar* title = new TitleBar(this);
	ui.titleLayout->addWidget(title);
	ui.mainLayout->addWidget(m);
	//ʹ���Լ����봴����layout
	//QGridLayout* layout = new QGridLayout(this);
	//layout->addWidget(title, 0, 0);
	//layout->addWidget(m, 0, 1);
	//this->setLayout(layout);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

FramelessWindow::~FramelessWindow()
{}