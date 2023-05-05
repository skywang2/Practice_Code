#include "sql.h"
#include <QtDebug>
#include <QHBoxLayout> 
#include <QSqlDriver> 
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord> 
#include <QSqlError> 
#include <QMessageBox>
#include <QInputDialog> 
#include <qpushbutton.h>

SQL::SQL(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QHBoxLayout* hLayout = new QHBoxLayout(this);
	this->setLayout(hLayout);
	addDatabase = new QPushButton(QString::fromLocal8Bit("连接数据库"), this);
	delDatabase = new QPushButton(QString::fromLocal8Bit("断开数据库"), this);
	addOneTable = new QPushButton(QString::fromLocal8Bit("增加一张表"), this);
	addOneLine = new QPushButton(QString::fromLocal8Bit("增加一条数据"), this);
	hLayout->addWidget(addDatabase);
	hLayout->addWidget(delDatabase);
	hLayout->addWidget(addOneTable);
	hLayout->addWidget(addOneLine);

	connect(addDatabase, &QPushButton::released, this, &SQL::CreateNewDB);
	connect(delDatabase, &QPushButton::released, this, &SQL::DeleteDB);
	connect(addOneTable, &QPushButton::released, this, &SQL::CreateNewTable);
	connect(addOneLine, &QPushButton::released, this, &SQL::AddOneLine);
}

SQL::~SQL()
{

}

void SQL::CreateNewDB()
{
	qDebug() << __FUNCTION__ << ":" << __LINE__;
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "myConnectionName");
	db.setDatabaseName("./qtDb.db");
	db.setHostName("myHostName");
	db.setUserName("myUserName");
	db.setPassword("myPassword");
	if (!db.open()) {
		qDebug() << __FUNCTION__ << ":" << __LINE__ << QString::fromLocal8Bit("， 无法建立数据库连接");
		return;
	}
	db.close();
}

void SQL::DeleteDB()
{
	qDebug() << __FUNCTION__ << ":" << __LINE__;
	QSqlDatabase::removeDatabase("myConnectionName");
}

void SQL::CreateNewTable()
{
	qDebug() << __FUNCTION__ << ":" << __LINE__;
	QSqlDatabase db = QSqlDatabase::database("myConnectionName");
	QSqlQuery query(db);
	bool ret = query.exec("CREATE TABLE automobil(id int primary key, attribute varchar, type varchar, "
										"kind varchar)");
	if (ret) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据库表创建成功"));
	}
	else {
		QSqlError error = query.lastError();
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), error.driverText());
	}

	db.close();
}

void SQL::AddOneLine()
{
	qDebug() << __FUNCTION__ << ":" << __LINE__;
	QSqlDatabase db = QSqlDatabase::database("myConnectionName");
	QSqlQuery query(db);
	query.prepare("insert into automobil values(?, ?, ?, ?)");

	int serial = QInputDialog::getInt(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入序号："), -1);
	if (-1 == serial) {
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入正确的序号"));
		return;
	}

	query.bindValue(0, serial);
	query.bindValue(1, QString::fromLocal8Bit("四轮"));
	query.bindValue(2, QString::fromLocal8Bit("小汽车"));
	query.bindValue(3, QString::fromLocal8Bit("东风"));

	bool ret = query.exec();
	if (ret) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据项添加成功"));
	}
	else {
		QSqlError error = query.lastError();
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), error.driverText() + " / " + error.databaseText());
	}

	db.close();
	return;
}

void SQL::CustomQuery()
{
	qDebug() << __FUNCTION__ << ":" << __LINE__;
}
