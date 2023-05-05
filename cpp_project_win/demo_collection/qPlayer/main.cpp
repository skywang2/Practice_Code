#include "qplayer.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile qssFile(":/qPlayer/myqss.qss");
	qssFile.open(QIODevice::ReadOnly);
	if (qssFile.isOpen())
	{
		a.setStyleSheet(qssFile.readAll());
		qssFile.close();
	}
	qPlayer w;
	w.show();
	return a.exec();
}
