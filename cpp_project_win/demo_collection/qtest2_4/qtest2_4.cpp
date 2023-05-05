#include "qtest2_4.h"
#include <stdio.h>
#include <locale>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include "signal.h"
#include "eventFilter.h"
#include "event_button.h"

qtest2_4::qtest2_4(QMainWindow *parent)
	: QMainWindow(parent)
{
	this->resize(600, 350);
	//设置界面字体大小
	//QFont font;
	//font.setPointSize(11);
	//this->setFont(font);

	//创建一个快捷功能
	menuBar = new QMenuBar(this);
	menu = menuBar->addMenu(tr("menu1"));
	actOpen = menu->addAction(tr("Open"));
	connect(actOpen, &QAction::triggered, this, &qtest2_4::test1);//[&](){ QMessageBox::information(this, "title", "hello!"); });
	this->setMenuBar(menuBar);
	//icon图标
	QIcon icon;
	icon.addFile(":/new/prefix1/REC/qtio.PNG");
	actOpen->setIcon(icon);
	this->setWindowIcon(icon);//给窗口添加图标

	//添加工具栏、状态栏
	mainToolBar = new QToolBar(this);
	this->addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->addAction(actOpen);

	statusBar = new QStatusBar(this);
	this->setStatusBar(statusBar);

	//状态栏添加label
	labStatus = new QLabel(this);
	statusBar->addPermanentWidget(labStatus);//右侧持久显示控件
	labStatus->setText(
		QString(tr("this is status bar. this is Translation**%1%2**")).arg(QString::fromLocal8Bit("你好")).arg("1234abcd").toStdString().c_str()
		);
	statusBar->showMessage("1111111111");//左侧临时消息

	//central widget
	myCentral = new QWidget(this);
	this->setCentralWidget(myCentral);
	//ledtMy1 = new QLineEdit(myCentral);
	//ledtMy2 = new QLineEdit(myCentral);
	//ledtMy3 = new QLineEdit(myCentral);
	//vLayout = new QVBoxLayout(myCentral);
	//vLayout->addWidget(ledtMy1);
	//vLayout->addWidget(ledtMy2);
	//vLayout->addWidget(ledtMy3);
	spinBMy = new QSpinBox;

	//pushbutton
	//btnC1 = new QPushButton("1", this);
	//btnC2 = new QPushButton("2", this);
	//btnC3 = new QPushButton("3", this);
	//QHBoxLayout* hLayout = new QHBoxLayout();
	//hLayout->addWidget(btnC1);
	//hLayout->addWidget(btnC2);
	//hLayout->addWidget(btnC3);
	//myCentral->setLayout(hLayout);

	//设置状态栏颜色
	QColor color(255, 165, 0);
	QPalette palette;
	palette.setColor(QPalette::Window, color);
	statusBar->setPalette(QPalette(color));
	statusBar->setAutoFillBackground(true);

	QGridLayout* layAll1 = new QGridLayout();
	QGroupBox* bg1 = new QGroupBox("1", this);
	QGroupBox* bg2 = new QGroupBox("2", this);
	QGroupBox* bg3 = new QGroupBox("3", this);
	QGroupBox* bg4 = new QGroupBox("4", this);
	QGridLayout* lay1 = new QGridLayout(bg1);
	QHBoxLayout* lay2 = new QHBoxLayout(bg2);
	QVBoxLayout* lay3 = new QVBoxLayout(bg3);
	QVBoxLayout* lay4 = new QVBoxLayout(bg4);
	QPushButton* btn1_1 = new QPushButton("1-1", bg1);
	QPushButton* btn1_2 = new QPushButton("1-2", bg1);
	QPushButton* btn1_3 = new QPushButton("1-3", bg1);
	QPushButton* btn2_1 = new QPushButton("2-1", bg2);
	QPushButton* btn3_1 = new QPushButton("3-1", bg3);
	QPushButton* btn3_2 = new QPushButton("3-2", bg3);
	MyEventButton* btn4_1 = new MyEventButton("MyEventButton, 4-1", bg4);

	lay1->addWidget(btn1_1, 1, 1);
	lay1->addWidget(btn1_2, 1, 2);
	lay1->addWidget(btn1_3, 2, 2);
	bg1->setLayout(lay1);

	lay2->addWidget(btn2_1);
	bg2->setLayout(lay2);

	lay3->addWidget(btn3_1);
	bg3->setLayout(lay3);
	lay3->addWidget(btn3_2);

	lay4->addWidget(btn4_1);
	bg4->setLayout(lay4);

	layAll1->addWidget(bg1, 1, 1);
	layAll1->addWidget(bg2, 2, 1);
	layAll1->addWidget(bg3, 1, 2);
	layAll1->addWidget(bg4, 2, 2);

	myCentral->setLayout(layAll1);
	myCentral->setPalette(QPalette(QColor(255, 231, 186)));
	myCentral->setAutoFillBackground(true);

	QIcon icon2;
	icon2.addFile(":/new/prefix1/Resources/Icon.ico");
	trayHandle = new QSystemTrayIcon(this);
	trayHandle->setIcon(icon2);
	trayHandle->show();
	connect(trayHandle, &QSystemTrayIcon::activated, this, &qtest2_4::onSystemTrayActivation);
	connect(this, &qtest2_4::destroyed, trayHandle, &QSystemTrayIcon::deleteLater);

	//安装自定义事件过滤器
	KeyPressEventFilter* kpEventFilter = new KeyPressEventFilter(this);//主窗口键盘、鼠标事件过滤器
	this->installEventFilter(kpEventFilter);
	//btn4_1->installEventFilter(kpEventFilter);
}

qtest2_4::~qtest2_4()
{

}

void qtest2_4::test1()
{
	//QMessageBox::information(this, "title", "hello!");
	//QObject* obj = new QMessageBox(QMessageBox::Critical, "title", "hello obj");

	int arr[16] = { 0 };
	size_t t = sizeof(arr);

	int aa = 1;
	++aa = 5;

	auto p1 = &(++aa);
	auto p2 = &aa;

	QList<QString> test;
	test << "1" << "2" << "3";
	test.removeAt(1);

	QVector<QString> test2;
	test2 << "1" << "2" << "3";
	test2.removeAt(1);

	//objectNmae
	const QMetaObject* meta = metaObject();
	QString name = menu->metaObject()->className();

	//property
	setProperty("fontSizeMy", "set");
	setProperty("strLevel", "66666");

	//QMultiMap
	QString output;
	QMultiMap<QString, int> qmap;
	qmap.insert("pencil", 97);
	qmap.insert("pencil", 98);
	qmap.insert("pencil", 99);
	qmap.insert("pen", 65);
	qmap.insert("pen", 66);
	//QMultiMap遍历某个key
	QList<int> qlist = qmap.values("pencil");
	for (auto i : qlist)
	{
		output.append(QString(i));
	}

	output.clear();
	//JAVA型迭代器
	QMutableMapIterator<QString, int> qIter(qmap);//一开始指向第一个元素之前
	while (qIter.hasNext())
	{
		output += QString().number(qIter.next().value());
		output += " ";
	}

	output.clear();
	//C++型迭代器
	QMap<QString, int>::const_iterator cIter;
	for (cIter = qmap.constBegin(); cIter != qmap.constEnd(); ++cIter)
	{
		cIter.key();
		output += QString().number(*cIter);
		output += "_";
	}
	map<string, int> vec;
	vec.insert({ "a", 109 });
	vec.insert({ "b", 110 });
	vec.insert({ "c", 111 });
	string f; int s;
	for (auto i : vec)
	{
		f = i.first;
		s = i.second;
	}
	output.clear();
	output = QString("%1, %2").arg(f.c_str()).arg(s);

	//output = QString("%1, %2, %3, %4")
	//	.arg(btnC1->sizeHint().width())
	//	.arg(btnC1->sizeHint().height())
	//	.arg(btnC1->minimumSizeHint().width())
	//	.arg(btnC1->minimumSizeHint().height());

	Camera *g5 = new G5();
	connect(g5, &Camera::SigDo, this, &qtest2_4::test2);//子类中使用父类信号
	g5->Do();

	//QMessageBox msgA(QMessageBox::Critical, "title", fontSizeMy, QMessageBox::Cancel | QMessageBox::Apply | QMessageBox::Yes, this);
	//msgA.exec();

	QJsonDocument jsonFile;
	QJsonObject allObj{ { "file_num", 2 } };
	QJsonArray fileInfo;
	QJsonObject f1, f2;
	f1.insert("file_name", "abc");
	f1.insert("file_size", 111);
	f2.insert("file_name", "bbbb");
	f2.insert("file_size", 2222);
	fileInfo.push_back(f1);
	fileInfo.push_back(f2);

	allObj.insert("file_info", fileInfo);
	jsonFile.setObject(allObj);
	QString txt = jsonFile.toJson();

	//section
	QString str;
	QString csv = "forename,middlename,surname,phone";
	str = csv.section(',', 0, 1);
	str = csv.section(',', 1, 1);
	str = csv.section(',', 2, 2);
	str = csv.section(',', 2, 3);

	setlocale(LC_ALL, "chs");
	for (int i = 0; i < 1; ++i)
	{
		//auto chars = L"转码进度";
		//wprintf(L"%s:%d\r", L"转码进度", i);
		cout << "转码进度：" << fixed << setprecision(2) << 13.1 << '%' << '\r';
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	cout << endl;
}

void qtest2_4::test2()
{
	qDebug() << "[" << __FUNCTION__ << ":" << __LINE__ << "], test2" << endl;
	QMessageBox msgA(QMessageBox::Critical, "title", fontSizeMy, QMessageBox::Cancel | QMessageBox::Apply | QMessageBox::Yes, this);
	msgA.exec();

}

void qtest2_4::changeEvent(QEvent *event) {
	if (event->type() == QEvent::WindowStateChange) {
		Qt::WindowStates state = this->windowState();
		switch (state) {
			case Qt::WindowNoState : {
				
			}
			break;
			case Qt::WindowMinimized: {
				this->hide();
				qDebug() << "WindowMinimized";
				trayHandle->show();
				trayHandle->showMessage("biaoti", "xxxxx");
				event->ignore();//丢弃该事件
			}
			break;
			//Qt::WindowMaximized
			//Qt::WindowFullScreen
			//Qt::WindowActive
		default:
			break;
		}
	}
	QWidget::changeEvent(event);
}

void qtest2_4::onSystemTrayActivation(QSystemTrayIcon::ActivationReason reason) {
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
	case QSystemTrayIcon::Context:
		break;
	case QSystemTrayIcon::DoubleClick:
		qDebug() << "DoubleClick";
		this->showNormal();
		this->raise();
		break;
	case QSystemTrayIcon::Trigger:
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	default:
		break;
	}
}
