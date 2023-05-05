#ifndef QANIMATIONTEST_H
#define QANIMATIONTEST_H

#include <QtWidgets/QMainWindow>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QPropertyAnimation>

class qAnimationTest : public QMainWindow
{
	Q_OBJECT

public:
	qAnimationTest(QWidget *parent = 0);
	~qAnimationTest();

private:
	QLabel* m_labTest;
	QPushButton* m_btnStart;
	QPushButton* m_btnStart2;

	QPropertyAnimation* m_animation;

	int m_moveFlag;
private:
	void OnButtonReleaseStart();
	void OnButtonReleaseStart2();
};

#endif // QANIMATIONTEST_H
