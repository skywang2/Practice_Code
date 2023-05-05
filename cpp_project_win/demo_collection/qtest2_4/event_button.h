#ifndef EVENTBUTTON_H
#define EVENTBUTTON_H
#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <qpushbutton.h>

class QString;//前向声明

class MyEventButton : public QPushButton
{
	Q_OBJECT
public:
	MyEventButton(const QString &text, QWidget* parent = nullptr) : QPushButton(text, parent){}
	~MyEventButton(){}

	bool event(QEvent *ev)
	{
		if (ev->type() == QEvent::MouseButtonPress) {
			qDebug() << "this type:" << typeid(*this).name();
			qDebug() << "mouse process";
			return true;
		}

		return QPushButton::event(ev);
	}
protected:

private:

};

#endif
