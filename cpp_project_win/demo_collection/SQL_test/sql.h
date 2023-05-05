#ifndef SQL_H
#define SQL_H

#include <QWidget>
//#include <qstring.h>
#include "ui_sql.h"

class QPushButton;

class SQL : public QWidget
{
	Q_OBJECT

public:
	SQL(QWidget *parent = 0);
	~SQL();

private:
	Ui::SQLClass ui;
	QPushButton* addDatabase;
	QPushButton* delDatabase;
	QPushButton* addOneTable;
	QPushButton* addOneLine;

public slots:
	void CreateNewDB();
	void DeleteDB();
	void CreateNewTable();
	void AddOneLine();
	void CustomQuery();
};

#endif // SQL_H
