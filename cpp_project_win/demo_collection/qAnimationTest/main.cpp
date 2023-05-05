#include "qanimationtest.h"
#include <QtWidgets/QApplication>
#include <QString>
#include <QFile>

void SetQssScript(QApplication& app)
{
	QFile script("qssScript.qss");

	script.open(QFile::ReadOnly);
	if (script.isOpen())
	{
		QString content = script.readAll();
		app.setStyleSheet(content);
		script.close();
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	SetQssScript(a);//设置style sheet

	qAnimationTest w;
	w.show();
	return a.exec();
}
