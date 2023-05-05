#include "MyTableView.h"
#include <QHeaderView>
#include <QDebug>
#include "MyModel.h"
#include "ProgressBarDelegate.h"
#include <QStandardItemModel>

MyTableView::MyTableView(QWidget *parent)
	:QTableView(parent)
{
	QStringList headersLabel;
	headersLabel << QString::fromLocal8Bit("a") << QString::fromLocal8Bit("b") << QString::fromLocal8Bit("c") << QString::fromLocal8Bit("d") << QString::fromLocal8Bit("e");
	//set model
	m_model.setParent(this);
	m_model.setColumnCount(5);
	m_model.setRowCount(8);
	m_model.setHorizontalHeaderLabels(headersLabel);
	for (int column = 0; column < 5; ++column)
	{
		for (int row = 0; row < 8; ++row)
		{
			QStandardItem *item = nullptr;
			if (0 == column)
			{
				item = new QStandardItem(QString::number(0));
			}
			else if (1 == column || 2 == column)
			{
				item = new QStandardItem();
			}
			else if (3 == column)
			{
				item = new QStandardItem(QString::number(column + row + 2));
				item->setFlags(item->flags() & (~Qt::ItemIsEditable) & (~Qt::ItemIsSelectable));
			}
			else if (4 == column)
			{
				item = new QStandardItem(QString("text"));
				item->setFlags(item->flags() & (~Qt::ItemIsEditable) & (~Qt::ItemIsSelectable));
			}
			m_model.setItem(row, column, item);
		}
	}
	this->setModel(&m_model);

	//set delegate
	m_ProgressBarDelegate.setParent(this);
	this->setItemDelegate(&m_ProgressBarDelegate);

	//set view
	QVector<int> headersWidth = { 50, 110, 110, 250 };
	int colIndex = 0;
	for (auto width : headersWidth)
	{
		this->setColumnWidth(colIndex++, width);
	}
	QHeaderView* pHHeader = this->horizontalHeader();
	pHHeader->setSectionResizeMode(4, QHeaderView::Stretch);
	QHeaderView* pVHeader = this->verticalHeader();
	pVHeader->setSectionResizeMode(QHeaderView::Stretch);

}

MyTableView::~MyTableView()
{
}

QStandardItemModel& MyTableView::GetTableModel()
{
	QVariant value = m_model.item(0, 0)->data(Qt::DisplayRole);
	int v = value.toInt();
	return m_model;
}
