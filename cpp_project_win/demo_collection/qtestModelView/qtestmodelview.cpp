#include "qtestmodelview.h"
#include <QTableView>
#include <QGridLayout>
#include <QVBoxLayout> 
#include <QPushButton> 
#include <QMenuBar> 
#include <QGroupBox> 
#include <QLabel> 
#include <QLineEdit>
#include <QStandardItemModel>
#include <QDebug>
#include "MyModel.h"
#include "MyTableView.h"

/*
brief 主窗口构造函数
param[in] parent，父窗口指针
return 无
*/
QTestModelView::QTestModelView(QWidget *parent)
	: QMainWindow(parent)
{
	this->resize(800, 600);

	//创建菜单栏
	QMenuBar *pMenuBar = new QMenuBar(this);
	QMenu *menu = pMenuBar->addMenu(tr("menu1"));
	this->setMenuBar(pMenuBar);

	//创建GroupBox
	QGroupBox *pGBox = new QGroupBox(this);
	QGridLayout *pGridLayout = new QGridLayout(pGBox);
	QPushButton *pBtn01 = new QPushButton(QString::fromLocal8Bit("打开"), this);
	QPushButton *pBtn02 = new QPushButton(QString::fromLocal8Bit("开始"), this);
	QLabel *pLabel = new QLabel(QString::fromLocal8Bit("1"), this);
	QLineEdit *pEdit = new QLineEdit(this);
	pGridLayout->addWidget(pLabel, 0, 0);
	pGridLayout->addWidget(pEdit, 0, 1);
	pGridLayout->addWidget(pBtn01, 0, 2);
	pGridLayout->addWidget(pBtn02, 1, 2);

	//创建TableView和Model
	m_pTableView = new MyTableView(this);

	//创建中间Widget及其layout
	QWidget *pCentral = new QWidget(this);//QMainWindow中有默认layout，用于管理控制栏，中间窗口，状态栏等，所以中间窗口需要重设才能加layout
	QVBoxLayout *pCentralLayout = new QVBoxLayout(pCentral);
	this->setCentralWidget(pCentral);

	//将控件加入中间layout
	pCentralLayout->addWidget(pGBox);
	pCentralLayout->addWidget(m_pTableView);

	//connect
	connect(pBtn02, &QPushButton::released, this, &QTestModelView::OnButtonReleasedSlot);
}

QTestModelView::~QTestModelView()
{

}

void QTestModelView::OnButtonReleasedSlot()
{
	QStandardItemModel& model = m_pTableView->GetTableModel();
	QVariant value = model.item(0, 0)->data(Qt::DisplayRole);
	qDebug() << "isValid:" << value.isValid() << ", value:" << value.toInt();

	static int process = 1;
	model.item(0, 3)->setData(process++, Qt::DisplayRole);
}
