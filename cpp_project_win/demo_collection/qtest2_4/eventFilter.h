#ifndef EVENTFILTER_H
#define EVENTFILTER_H
#include <typeinfo>
#include <QObject>
#include <QDebug>
#include <QKeyEvent>

class KeyPressEventFilter : public QObject
{
	Q_OBJECT
public:
	KeyPressEventFilter(QObject* parent=nullptr):QObject(parent){}
	~KeyPressEventFilter(){}

protected:
	bool eventFilter(QObject* obj, QEvent* ev)
	{
		if (ev->type() == QEvent::KeyPress)
		{
			qDebug() << "in eventFilter this type:" << typeid(*this).name();
			qDebug() << "key press:" << static_cast<QKeyEvent*>(ev)->key();
			return true;
		}
		else if (ev->type() == QEvent::MouseButtonPress)
		{
			qDebug() << "this type:" << typeid(*this).name();
			qDebug() << "mouse press";
			//return true;
			return QObject::eventFilter(obj, ev);
		}
		else
		{
			return QObject::eventFilter(obj, ev);
		}
	}

private:

};

#endif
