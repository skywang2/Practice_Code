#ifndef QPLAYER_H
#define QPLAYER_H

#include <Windows.h>
#include <QtWidgets/QMainWindow>
#include <QString>
#include "ui_qplayer.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;

class qPlayer : public QMainWindow
{
	Q_OBJECT

public:
	qPlayer(QWidget *parent = 0);
	~qPlayer();
	void OnBtnClickedLogin();
	void OnBtnClickedLogout();
	void OnBtnClickedStartPlay();
	void OnBtnClickedStopPlay();

private:
	Ui::qPlayerClass ui;
	//UI
	QGridLayout *gridMainWin;
	QLineEdit *ledtip;
	QLineEdit *ledtPassword;
	QLabel *labMainWin;
	QPushButton *btnLogin;
	QPushButton *btnLogout;
	QPushButton *btnStartPlay;
	QPushButton *btnStopPlay;
	//data
	QString ip;
	QString Password;
	LONG userID;
	LONG lRealPlayHandle;
	RECT windowRect;
	RECT clientRect;

};

#endif // QPLAYER_H
