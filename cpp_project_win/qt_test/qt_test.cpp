#include "qt_test.h"
#include <QComboBox>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QString>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTextEdit>
#include <QDockWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QCompleter>
#include <QLineEdit>
#include <QStringList>
#include <QMovie>
#include <QSqlDatabase>
#include <QTableView>
#include <QSqlTableModel>
#include "initDb.hpp"

qt_test::qt_test(QWidget *parent)
	: QMainWindow(parent)
{
	move(0, 0);
	ui.setupUi(this);

	//菜单栏
	QMenuBar *pBar = menuBar();
	setMenuBar(pBar);
	//添加菜单
	QMenu* pBarFile = pBar->addMenu(QString::fromLocal8Bit("文件"));
	//添加菜单项，添加动作
	QAction *pBarFileNew = pBarFile->addAction(QString::fromLocal8Bit("新建"));
	pBarFile->addSeparator();
	QAction* pBarFileOpen = pBarFile->addAction(QString::fromLocal8Bit("模态"));
	QAction* pBarFileOpen2 = pBarFile->addAction(QString::fromLocal8Bit("非模态"));
	QAction* pBarMess = pBarFile->addAction("Message Box");
	QAction* pBarQues = pBarFile->addAction("Question Box");
	QAction* pBarFileOpen3 = pBarFile->addAction(QString::fromLocal8Bit("打开"));
	(void)connect(pBarFileNew, &QAction::triggered, this, &qt_test::newFile);
	(void)connect(pBarFileOpen, &QAction::triggered, this, &qt_test::fileOpen);
	(void)connect(pBarFileOpen2, &QAction::triggered, this, &qt_test::fileOpen2);
	(void)connect(pBarMess, &QAction::triggered, this, &qt_test::message);
	(void)connect(pBarQues, &QAction::triggered, this, &qt_test::question);
	(void)connect(pBarFileOpen3, &QAction::triggered, this, &qt_test::fileOpen3);
	//工具栏
	QToolBar* pTool = addToolBar("toolBar");
	//工具栏快捷键
	pTool->addAction(pBarFileNew);
	//工具栏快捷键（按钮）
	QPushButton* pButBarFileNew = new QPushButton(this);
	pButBarFileNew->setText(QString::fromLocal8Bit("新建"));
	pTool->addWidget(pButBarFileNew);
	(void)connect(pButBarFileNew, &QPushButton::released, this, &qt_test::newFile);
	//状态栏
	QStatusBar* pStatusBar = statusBar();
	QLabel* pLabel = new QLabel(this);
	pLabel->setText("Normal text file");
	pStatusBar->addWidget(pLabel);
	pStatusBar->addPermanentWidget(new QLabel("222", this));
	//button1
	b1.setParent(this);
	b1.setText("111");
	b1.resize(100, 50);
	b1.move(50, 50);
	b1.show();
	//button2
	b2.setParent(this);
	b2.setText("222");
	b2.resize(100, 50);
	b2.move(200, 50);
	b2.show();
	ui.stackedWidget->widget(0)->layout()->addWidget(&b1);//放到stackedWidget中
	ui.stackedWidget->widget(0)->layout()->addWidget(&b2);
	//重载槽函数，需要定义成员函数指针才能用于Qt::connect
	void (qt_test:: * noParam)() = &qt_test::mySlots;
	void (qt_test:: * oneParam)(int) = &qt_test::mySlots;
	(void)connect(&b1, &QPushButton::released, this, noParam);

	//核心控件
	//QTextEdit* pTxtEdit = new QTextEdit(this);
	//setCentralWidget(pTxtEdit);

	//浮动窗口
	//QDockWidget* pDock = new QDockWidget(this);
	//addDockWidget(Qt::RightDockWidgetArea, pDock);
	//QTextEdit* pTxtSub = new QTextEdit(this);
	//pDock->setWidget(pTxtSub);

	//下拉列表显示补全信息
	QLineEdit* ledit = new QLineEdit(this);
	QStringList edtList;
	edtList << "111" << "2222" << "333";
	ledit->setText("asdfAAA");
	ledit->setGeometry(50, 200, 100, 30);
	ui.stackedWidget->widget(0)->layout()->addWidget(ledit);
	QCompleter* com = new QCompleter(edtList, this);
	com->setCaseSensitivity(Qt::CaseInsensitive);
	ledit->setCompleter(com);

	QMovie Movie(":/images/test.jpg");//没用到
	//设置styleSheet
	ui.testCss->setStyleSheet("QLabel{color:red;}");

	resize(720, 480);
	setFixedSize(this->width(),this->height());

	//初始化数据库和tableview、tablemodel
	InitDb();

	//使用下拉列表对stackedwidget换页，这一段放在函数最后
	for(int i=0 ; i <ui.stackedWidget->count() ; ++i)
		ui.comboBox->addItem(QString("page%1").arg(i));
	(void)connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged)
		, ui.stackedWidget, &QStackedWidget::setCurrentIndex);//currentIndexChanged是一个重载函数
}

qt_test::~qt_test()
{

}

void qt_test::mySlots()
{
	qDebug() << "b1 111";
}
//重载槽函数
void qt_test::mySlots(int i)
{
	qDebug() << "b1 111, i=" << i;
}

void qt_test::newFile()
{
	qDebug() << "new file";
}

//模态对话框
void qt_test::fileOpen()
{
	QDialog dlg;
	dlg.resize(200, 100);
	dlg.exec();
	qDebug() << "next dlg";
}

//非模态对话框
void qt_test::fileOpen2()
{
	dlg2.resize(200, 100);
	dlg2.show();
	qDebug() << "next dlg2";
}

void qt_test::message()
{
	QMessageBox::about(this, "box", "hello!!!");
}

void qt_test::question()
{
	QMessageBox::StandardButton ret = QMessageBox::question(this,
		"question", "Are you OK?", 
		QMessageBox::Ok | QMessageBox::Yes | QMessageBox::No
	);
	switch (ret)
	{
	case QMessageBox::Yes:
		qDebug() << "yes";
		break;
	case QMessageBox::No:
		qDebug() << "no";
		break;
	default:
		break;
	}

}

void qt_test::fileOpen3()
{
	auto fileName = QFileDialog::getOpenFileName(this, "open", "./", "Text(*.txt);;All(*.*)");
}

void qt_test::InitDb()
{
	qDebug() << QSqlDatabase::drivers();
	QDir(".").remove("testDB.db");
	if (!QSqlDatabase::drivers().contains("QSQLITE"))
	{
		QMessageBox::critical(this, "Unable to load database", "This demo needs the SQLITE driver");
		return;
	}
	QSqlError err = initDb();
	if (err.type() != QSqlError::NoError)
	{
		qDebug() << "sql error:" << err;
		return;
	}
	if (QSqlDatabase::contains("MyDBLink01"))
	{
		QSqlDatabase db = QSqlDatabase::database("MyDBLink01");
		qDebug() << "db tables:" << db.tables();
		db.close();
	}
	m_pTableView = new QTableView(this);
	ui.stackedWidget->insertWidget(1, m_pTableView);
	m_pTableModel = new QSqlTableModel(m_pTableView);
	//m_pTableModel->setTable("books");
	//if (!m_pTableModel->select()) {
	//	qDebug() << "model select error";
	//}


}
