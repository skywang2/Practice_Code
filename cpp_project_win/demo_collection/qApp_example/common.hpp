#pragma once
#include <windows.h>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QSettings>

QSettings gSettings(GetTargetPath() + QDir::separator() + "settings.ini", QSettings::IniFormat);

int runExternalProcess(const QString &program, const QStringList &args)
{
	QProcess proc;
	proc.setProgram(program);
	proc.setArguments(args);
	proc.start();
	proc.waitForFinished();
	return proc.exitCode();
}


QString GetTargetPath()
{
	char path[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, path, sizeof(path));
	return QFileInfo(QString::fromLocal8Bit(path)).absolutePath();
}

/********************
*log类
*
********************/
//.h
class CLogger
{
	static CLogger logger;
private:
	CLogger() :logType(LOG_TO_CONSOLE){}
public:
	static CLogger &getInstance() { return logger; }
	enum LoggerType{ LOG_TO_CONSOLE, LOG_TO_FILE };

	CLogger(const CLogger &) = delete;
	CLogger &operator=(const CLogger &) = delete;
	~CLogger(){}

	void setType(LoggerType t) { logType = t; }
	void setLogPath(const QString &path);
	template<typename T> CLogger &operator<<(T&& t);

private:
	QString logPath;
	LoggerType logType;
	QFile file;
	QMutex mt;
};

template<typename T>
CLogger &CLogger::operator<<(T&& t)
{
	if (LOG_TO_CONSOLE == logType)
	{
		qDebug() << t;
	}
	else
	{
		QMutexLocker lock(&mt);
		if (!file.isOpen())
		{
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				return *this;
			}
		}
		QTextStream ts(&file);
		ts << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]\t") << t << endl;
	}
	return *this;
}

extern CLogger &gLogger;

//.cpp
CLogger &gLogger = CLogger::getInstance();

void CLogger::setLogPath(const QString &path)
{
	if (logPath != path)
	{
		QMutexLocker lock(&mt);
		if (file.isOpen())
		{
			file.close();
		}
		file.setFileName(path);
		logPath = path;
	}
}

CLogger CLogger::logger;

//================
QString getTitle()
{
	std::map<QString, int> monthMap =
	{
		{ "Jan", 1 },
		{ "Feb", 2 },
		{ "Mar", 3 },
		{ "Apr", 4 },
		{ "May", 5 },
		{ "Jun", 6 },
		{ "Jul", 7 },
		{ "Aug", 8 },
		{ "Sep", 9 },
		{ "Oct", 10 },
		{ "Nov", 11 },
		{ "Dec", 12 }
	};

	QString buildDate = __DATE__;

	auto it = monthMap.find(buildDate.section(" ", 0, 0, QString::SectionSkipEmpty));
	QString monthStr = QString("%1").arg(it->second, 2, 10, QChar('0'));
	QString dayStr = QString("%1").arg(buildDate.section(" ", 1, 1, QString::SectionSkipEmpty), 2, QChar('0'));    //当日期为单位数时，__DATE__可能出现两个空格，所以要指定QString::SectionSkipEmpty参数。
	QString yearStr = buildDate.section(" ", 2, 2, QString::SectionSkipEmpty);

	QString title = QString("%1-%2 build %3-%4-%5 %6").arg(QString::fromLocal8Bit("xxx")).arg(xxx).arg(yearStr).arg(monthStr).arg(dayStr).arg(__TIME__);
	return title;
}
