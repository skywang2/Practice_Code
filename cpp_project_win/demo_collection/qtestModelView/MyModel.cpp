#include "MyModel.h"


MyModel::MyModel(QObject *parent)
	:QAbstractTableModel(parent),
	progress(0)
{
	//QStringList headers;
	//headers << "a" << QString::fromLocal8Bit("b") << QString::fromLocal8Bit("v") << QString::fromLocal8Bit("d") << QString::fromLocal8Bit("e");
	//for (auto header : headers)
	//{
	//	setHeaderData(0, Qt::Horizontal, header);
	//}
}

MyModel::~MyModel()
{
}

int MyModel::rowCount(const QModelIndex & /*parent*/) const
{
	return 8;
}

int MyModel::columnCount(const QModelIndex & /*parent*/) const
{
	return 5;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
	if (Qt::DisplayRole == role)
	{
		if (index.column() == 1)
		{
			return (index.row() + 1) * 10;
		}
		else
		{
			return (index.row() + 1) + (index.column() + 1);
		}
	}
	return QVariant();
}

//QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
//	{
//		return m_headers.at(section);
//	}
//	return QAbstractItemModel::headerData(section, orientation, role);
//}
//
//void MyModel::setHeaderData(const QStringList& headerList)
//{
//	m_headers = headerList;
//}


