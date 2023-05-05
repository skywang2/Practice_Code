#ifndef QTESTMODELVIEW_H
#define QTESTMODELVIEW_H
#include <QtWidgets/QMainWindow>
class QStandardItemModel;
class MyTableView;

class QTestModelView : public QMainWindow
{
	Q_OBJECT

public:
	QTestModelView(QWidget *parent = 0);
	~QTestModelView();

	void OnButtonReleasedSlot();
private:
	MyTableView* m_pTableView;
};

#endif // QTESTMODELVIEW_H
