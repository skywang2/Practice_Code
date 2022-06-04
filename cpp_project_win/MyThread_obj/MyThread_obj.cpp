#include "MyThread_obj.h"

MyThread_obj::MyThread_obj(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//�����̷߳���ռ�
	//����ָ���������и�����ᵼ���޷�����moveToThread()
	thread_obj = new OneThread_obj();
	//�������߳�
	thread_1 = new QThread(this);
	//���Զ����̼߳��뵽���߳���
	thread_obj->moveToThread(thread_1);
	connect(thread_1, QThread::finished, thread_obj, QObject::deleteLater);

	bool ret;
	//��ť�ź�
	ret = connect(ui.pushButton, &QPushButton::released, this, &MyThread_obj::onReleasedBtn);
	//�����Զ������̷߳��ص��ź�
	//ret = connect(thread_obj, &OneThread_obj::mySignal, this, &MyThread_obj::slotsDealSig);
	//�����̴߳�������
	ret = connect(this, &MyThread_obj::doOneThread, thread_obj, &OneThread_obj::ttttt);
	//***connect()�ĵ�������������ӷ�ʽ��Ĭ�ϣ����У�ֱ��***
	//***���߳�ʱ��������***
	//***���߳�Ĭ��Ϊ���У����߳�Ĭ��Ϊֱ��***
	//***���У��ۺ��������߳��������һ��
	//***ֱ�ӣ��ۺ��������߳��뷢����һ��

}

void MyThread_obj::onReleasedBtn()
{
	qDebug() << "[" << __FILE__ << "], Thread id:" << QThread::currentThreadId();

	//�������߳�
	thread_1->start();
	thread_obj->setFlag(false);
	//�����Զ������̴߳�������������ֱ�ӵ��ã��ᵼ���̴߳������������߳������У�
	//ֻ��ͨ���źźͲ��������̴߳�������
	//�̴߳��������в��ܴ���ͼ�ν���(���Կ��Ƿ��ź������̴߳���)
	//���źŷ�ʽ��������Ϊconnect()�ĵ����������Ӱ��ۺ����������߳�
	emit doOneThread();
}

void MyThread_obj::slotsDealSig()
{
	//qDebug() << "[" << __FILE__ << "], Thread id:" << QThread::currentThread();
	//thread_1->quit();
}

void MyThread_obj::closeEvent(QCloseEvent* e)
{
	thread_obj->setFlag(true);
	thread_1->quit();
	thread_1->wait();
	//e->ignore();
}