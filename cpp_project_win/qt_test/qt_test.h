#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QDialog>
#include "myButton.h"
#include "ui_qt_test.h"
class QTableView;
class QSqlTableModel;

class qt_test : public QMainWindow
{
	Q_OBJECT

public:
	qt_test(QWidget *parent = Q_NULLPTR);
	~qt_test();
	void mySlots();
	void mySlots(int i);
	void newFile();
	void fileOpen();
	void fileOpen2();
	void message();
	void question();
	void fileOpen3();
	//??ʼ?????ݿ⣻
	void InitDb();

private:
	QPushButton b1;
	myButton b2;
	QDialog dlg2;
	QTableView* m_pTableView;
	QSqlTableModel* m_pTableModel;
	Ui::qt_testClass ui;
};
