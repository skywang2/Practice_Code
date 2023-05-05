#pragma once
#include <QAbstractTableModel>

class MyModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	MyModel(QObject *parent=nullptr);
	~MyModel();
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	//QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	//void setHeaderData(const QStringList& headerList);
private:
	QStringList m_headers;
	int progress;
};

