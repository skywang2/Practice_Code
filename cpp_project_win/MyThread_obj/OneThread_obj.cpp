#include "OneThread_obj.h"

OneThread_obj::OneThread_obj(QObject *parent)
	: QObject(parent)
{
	isStop = false;
}

OneThread_obj::~OneThread_obj()
{
}

void OneThread_obj::setFlag(bool flag)
{
	isStop = flag;
}

bool OneThread_obj::getFlag()
{
	return isStop;
}

void OneThread_obj::ttttt()
{
	//static int i = 0;
	//qDebug() << "i:" << i;
	//i++;
	//while (false == isStop)
	//{
		qDebug() << "[" << __FILE__ << "], Thread id:" << QThread::currentThreadId();
		QThread::sleep(1);
		//当标志位被修改，跳出自定义子线程处理函数
		//if (true == isStop)
		//{
		//	qDebug() << "thread is stop...";
		//	break;
		//}
	/*}*/
	//emit mySignal();
}
