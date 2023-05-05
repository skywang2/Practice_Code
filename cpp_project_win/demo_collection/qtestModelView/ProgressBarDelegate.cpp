#include "ProgressBarDelegate.h"
#include <QStyleOptionProgressBar> 
#include <QPainter>
#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QComboBox>
#include <memory>

ProgressBarDelegate::ProgressBarDelegate(QObject* parent)
	:QStyledItemDelegate(parent)
{
}


ProgressBarDelegate::~ProgressBarDelegate()
{
}

//创建编辑控件
QWidget *ProgressBarDelegate::createEditor(QWidget *parent,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	//qDebug() << __FUNCTION__;
	int col = index.column();
	if (0 == col)
	{
		QComboBox* cb = new QComboBox(parent);
		cb->addItems({"off", "on"});
		return cb;
	}
	else
	{
		return QStyledItemDelegate::createEditor(parent, option, index);
	}
}

//从model获取数据并设置给编辑控件
void ProgressBarDelegate::setEditorData(QWidget *editor,
	const QModelIndex &index) const
{
	int col = index.column();
	if (0 == col)
	{
		auto p = index.model();
		int value = index.data(Qt::DisplayRole).toInt();
		QComboBox *cb = static_cast<QComboBox*>(editor);
		cb->setCurrentIndex(value);
	}
	else
	{
		return QStyledItemDelegate::setEditorData(editor, index);
	}
}

//从编辑控件获取数据并设置给model
void ProgressBarDelegate::setModelData(QWidget *editor, 
	QAbstractItemModel *model,
	const QModelIndex &index) const
{
	int col = index.column();
	if (0 == col)
	{
		QComboBox* cb = static_cast<QComboBox*>(editor);
		int value = cb->currentIndex();
		model->setData(index, value);
	}
	else
	{
		return QStyledItemDelegate::setModelData(editor, model, index);
	}
}

//更新编辑控件图形
void ProgressBarDelegate::updateEditorGeometry(QWidget *editor, 
	const QStyleOptionViewItem &option, 
	const QModelIndex &index) const
{
		//editor->setGeometry(option.rect);
		//qDebug() << __FUNCTION__;
		return QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	int col = index.column();
	std::unique_ptr<QStyleOption> opt;

	if (0 == col)//check box
	{
		int value = index.data(Qt::DisplayRole).toInt();
		painter->drawText(option.rect, Qt::AlignVCenter, QString(!value ? "off" : "on"));
	}
	else if (3 == col)//progress bar
	{
		int value = index.model()->data(index).toInt();
		value = value > 100 ? 100 : value;
		QStyleOptionProgressBar optionBar;//记录了控件的默认绘制参数
		optionBar.minimum = 0;
		optionBar.maximum = 100;
		optionBar.progress = value;
		optionBar.textVisible = true;
		optionBar.text = QString("%1%").arg(value);
		//垂直居中
		int H = option.rect.height();
		int Y = option.rect.y();
		int barH = 20;
		optionBar.rect = option.rect;
		optionBar.rect.setY(Y + H / 2 - barH / 2);//注意，会改变hight
		optionBar.rect.setHeight(barH);

		QApplication::style()->drawControl(QStyle::CE_ProgressBar, &optionBar, painter);
	}
	else
	{
		return QStyledItemDelegate::paint(painter, option, index);
	}
}
