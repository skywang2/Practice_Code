#pragma once

#include <QObject>
#include <QString>

class ClassA : public QObject
{
	Q_OBJECT

public:
	ClassA(QObject *parent=nullptr);
	~ClassA();

	bool GetTrue() { return true; }
	bool GetFalse() { return true; }
	int GetInt(int i) { return i; }
	QString GetQString(const QString& s) { return s; }
};
