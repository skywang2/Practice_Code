#include <iostream>
#include <string>
#include <Windows.h>
#include <ImageHlp.h>
#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QTimer> 
#include "qtest2_4.h"

using namespace std;

bool SetWindowLanguage(QTranslator* const translator)
{
	if (!translator)
	{
		return false;
	}

	bool ret = false;
	LANGID id = GetSystemDefaultLangID();
	switch (id)
	{
		/*
		0x0404		Chinese(Taiwan   Region)				台湾
		0x0804		Chinese(PRC)									中华人民共和国
		0x0c04		Chinese(Hong   Kong   SAR, PRC)	香港
		0x1004		Chinese(Singapore)							新加坡
		*/
	case 0x0404:
	case 0x0804:
	case 0x0c04:
	case 0x1004:
		ret = translator->load("./qtest2_4_zh.qm");
		break;
	default:
		ret = true;
		break;
	}

	return ret;
}

int main(int argc, char *argv[])
{
	//MakeSureDirectoryPathExists("REC\\new\\");
	//CopyFile(L"REC\\qtio.PNG", L"REC\\new\\qtio.PNG", true);
	QApplication a(argc, argv);
	QTranslator* translator = new QTranslator();

	if (SetWindowLanguage(translator))
	{
		a.installTranslator(translator);
	}

	QWidget x;
	x.show();
	QTimer::singleShot(5000, &x, SLOT(hide()));

	qtest2_4 w;
	QTimer::singleShot(2000, &w, SLOT(show()));
	//w.show();
	return a.exec();
}
