#pragma once

#include <QtWidgets/QMainWindow>
#include <qpushbutton.h>
#include <qdebug.h>
#include <qmenubar.h>
#include <qmenu.h>
#include <qstring.h>
#include <qaction.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qdockwidget.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qcompleter.h>
#include <qlineedit.h>
#include <qstringlist.h>
#include <qmovie.h>
#include "myButton.h"

#include "ui_qt_test.h"

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

private:
	QPushButton b1;
	myButton b2;
	QDialog dlg2;
	Ui::qt_testClass ui;
};
