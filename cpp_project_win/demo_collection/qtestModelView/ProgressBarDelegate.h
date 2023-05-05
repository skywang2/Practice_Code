#pragma once
#include <QObject>
#include <QStyledItemDelegate>
class QPainter;
class QStyleOptionViewItem;
class QModelIndex;

class ProgressBarDelegate :
	public QStyledItemDelegate//QItemDelegate
{
	Q_OBJECT;
public:
	ProgressBarDelegate(QObject* parent=nullptr);
	~ProgressBarDelegate();
	//绘图
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	//创建编辑控件
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const;
	//从model获取数据并设置给编辑控件
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	//从编辑控件获取数据并设置给model
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	//更新编辑控件图形
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

