#ifndef QTEST2_4_H
#define QTEST2_4_H

#include <iostream>
#include <string>
#include <QWidget>
#include <QMainWindow> 
#include <QTextEdit>
#include <QSpinBox>
#include <QFontComboBox> 
#include <QMenuBar> 
#include <QToolBar>
#include <QStatusBar> 
#include <QMessageBox>
#include <QDebug>
#include <QtGlobal>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QMap>
#include <QMultiMap> 
#include <QPushbutton>
#include <QList>
#include <QLabel>
#include <QPalette>
#include <QGroupBox> 
#include <QEvent>
#include <QSystemTrayIcon> 
#include <map>

using namespace std;

class qtest2_4 : public QMainWindow
{
	Q_OBJECT;
	Q_PROPERTY(QString fontSizeMy READ GetFontSizeMy WRITE SetFontSizeMy);
	//Q_PROPERTY(QString fontSizeMy READ GetFontSizeMy WRITE SetFontSizeMy);
	Q_PROPERTY(QString strLevel MEMBER strLevel);

public:
	qtest2_4(QMainWindow *parent = 0);
	~qtest2_4();

	void test(){ cout << 1234 << endl; };

private:
	//QAction *actCut;
	//QAction *actCopy;
	//QAction *actPaste;
	//QAction *actFontBold;
	//QAction *actFontItalic;
	//QAction *actClose;
	QAction *actOpen;
	//QAction *actClear;
	//QAction *actFont;
	//QAction *actNew;
	//QAction *actToolbarLab;
	QWidget* myCentral;
	QTextEdit *txtEdit;
	QSpinBox *spinBox;
	QFontComboBox *fontComboBox;
	QMenuBar *menuBar;
	QMenu *menu;
	//QMenu *menu_2;
	//QMenu *menu_3;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;
	QPushButton* btnC1;
	QPushButton* btnC2;
	QPushButton* btnC3;
	QLabel* labStatus;

	QString fontSizeMy;
	const QString& GetFontSizeMy(){ return fontSizeMy; }
	void SetFontSizeMy(const QString& s){ fontSizeMy = s; }

	QLineEdit* ledtMy1;
	QLineEdit* ledtMy2;
	QLineEdit* ledtMy3;
	QSpinBox* spinBMy;
	QVBoxLayout* vLayout;

	QSystemTrayIcon* trayHandle;
	QString strLevel;

	void Open(){ qDebug() << "Open" << endl; }

private slots:
	void test1();
	void test2();//随便打印一些内容
	void changeEvent(QEvent *event) override;
	void onSystemTrayActivation(QSystemTrayIcon::ActivationReason reason);
};

#endif // QTEST2_4_H
