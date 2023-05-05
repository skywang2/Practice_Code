#pragma once
#include "qtableview.h"
#include <QLineEdit>
#include <QStandardItemModel>
#include "ProgressBarDelegate.h"
class MyModel;
class ProgressBarDelegate;
class QStandardItemModel;

class MyTableView :
	public QTableView
{
	Q_OBJECT
public:
	MyTableView(QWidget *parent=nullptr);
	~MyTableView();
	QStandardItemModel& GetTableModel();

private:
	//MyModel* m_model;
	QStandardItemModel m_model;
	ProgressBarDelegate m_ProgressBarDelegate;
};

