#include <QDebug>
#include <QDateTime> 
#include <QThread> 
#include "log.h"
// 开发过程中暂时使用qDebug替代 1-使用debug打印输出  0-使用日志输出

#define LOG_INIT(path) Log::instance()->init(path);

enum Level_Value
{
	RT_DEBUG = 0,
	RT_INFO = 1,
	RT_WARN = 2,
	RT_ERROR = 3,
};

#if 0

// 以文件行列记录信息，非以类名对象形式
#define LOG_DEBUG(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("DEBUG")  \
                                           .arg(msg);

#define LOG_INFO(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("INFO")  \
                                           .arg(msg);

#define LOG_WARN(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("WARN")  \
                                           .arg(msg);

#define LOG_ERROR(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1:%2:%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("ERROR")  \
                                           .arg(msg);


// 以类名对象形式记录信息
#define LOG_OBJECT_DEBUG(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("DEBUG")  \
                                           .arg(msg);

#define LOG_OBJECT_INFO(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("INFO")  \
                                           .arg(msg);

#define LOG_OBJECT_WARN(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("WARN")  \
                                           .arg(msg);

#define LOG_OBJECT_ERROR(msg) qDebug() << QString("[%1][%2][%3][%4]%5")  \
                                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))  \
                                           .arg(QString("%1::%2:%3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__))  \
                                           .arg(QThread::currentThread()->objectName())  \
                                           .arg("ERROR")  \
                                           .arg(msg);


// 写入日志系统
#else

// 以文件行列记录信息，非以类名对象形式
#define LOG_DEBUG(msg) Log::instance()->debug(QString("%1 %2").arg(QString("[%1][%2][%3]").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)).arg(msg));

#define LOG_INFO(msg) Log::instance()->info(QString("%1 %2").arg(QString("[%1][%2][%3]").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)).arg(msg));

#define LOG_WARN(msg) Log::instance()->warn(QString("%1 %2").arg(QString("[%1][%2][%3]").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)).arg(msg));

#define LOG_ERROR(msg) Log::instance()->error(QString("%1 %2").arg(QString("[%1][%2][%3]").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)).arg(msg));

#define LOG_DBG(level, msg) if(RT_DEBUG == level){LOG_DEBUG(msg)} \
							else if(RT_INFO == level){LOG_INFO(msg)}\
							else if(RT_WARN == level){LOG_WARN(msg)}\
							else if(RT_ERROR == level){LOG_ERROR(msg)}\
							else{};

// 以类名对象形式记录信息
#define LOG_OBJECT_DEBUG(msg) Log::instance()->debug(QString("[%1] %2").arg(QString("%1::%2 %3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__)).arg(msg));

#define LOG_OBJECT_INFO(msg) Log::instance()->info(QString("[%1] %2").arg(QString("%1::%2 %3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__)).arg(msg));

#define LOG_OBJECT_WARN(msg) Log::instance()->warn(QString("[%1] %2").arg(QString("%1::%2 %3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__)).arg(msg));

#define LOG_OBJECT_ERROR(msg) Log::instance()->error(QString("[%1] %2").arg(QString("%1::%2 %3").arg(this->metaObject()->className()).arg(__FUNCTION__).arg(__LINE__)).arg(msg));

#define LOG_OBJ_DBG(level, msg) if(RT_DEBUG == level){LOG_OBJECT_DEBUG(msg)} \
							else if(RT_INFO == level){LOG_OBJECT_INFO(msg)}\
							else if(RT_WARN == level){LOG_OBJECT_WARN(msg)}\
							else if(RT_ERROR == level){LOG_OBJECT_ERROR(msg)}\
							else{};

#endif

