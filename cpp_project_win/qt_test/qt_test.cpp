#include "qt_test.h"

qt_test::qt_test(QWidget *parent)
	: QMainWindow(parent)
{
	move(0, 0);

	ui.setupUi(this);

	//�˵���
	QMenuBar *pBar = menuBar();
	setMenuBar(pBar);
	//���Ӳ˵�
	QMenu* pBarFile = pBar->addMenu(QString::fromLocal8Bit("�ļ�"));
	//���Ӳ˵�����Ӷ���
	QAction *pBarFileNew = pBarFile->addAction(QString::fromLocal8Bit("�½�"));
	pBarFile->addSeparator();
	QAction* pBarFileOpen = pBarFile->addAction(QString::fromLocal8Bit("ģ̬"));
	QAction* pBarFileOpen2 = pBarFile->addAction(QString::fromLocal8Bit("��ģ̬"));
	QAction* pBarMess = pBarFile->addAction("Message Box");
	QAction* pBarQues = pBarFile->addAction("Question Box");
	QAction* pBarFileOpen3 = pBarFile->addAction(QString::fromLocal8Bit("��"));

	(void)connect(pBarFileNew, &QAction::triggered, this, &qt_test::newFile);
	(void)connect(pBarFileOpen, &QAction::triggered, this, &qt_test::fileOpen);
	(void)connect(pBarFileOpen2, &QAction::triggered, this, &qt_test::fileOpen2);
	(void)connect(pBarMess, &QAction::triggered, this, &qt_test::message);
	(void)connect(pBarQues, &QAction::triggered, this, &qt_test::question);
	(void)connect(pBarFileOpen3, &QAction::triggered, this, &qt_test::fileOpen3);

	//������
	QToolBar* pTool = addToolBar("toolBar");
	//��������ݼ�
	pTool->addAction(pBarFileNew);
	//��������ݼ�����ť��
	QPushButton* pButBarFileNew = new QPushButton(this);
	pButBarFileNew->setText(QString::fromLocal8Bit("�½�"));
	pTool->addWidget(pButBarFileNew);
	(void)connect(pButBarFileNew, &QPushButton::released, this, &qt_test::newFile);
		 
	b1.setParent(this);
	b1.setText("111");
	b1.resize(100, 50);
	b1.move(50, 50);
	b1.show();

	b2.setParent(this);
	b2.setText("222");
	b2.resize(100, 50);
	b2.move(200, 50);
	b2.show();
	 
	void (qt_test:: * noParam)() = &qt_test::mySlots;
	//void (qt_test:: * oneP aram)(int) = &qt_test::mySlots;
	(void)connect(&b1, &QPushButton::released, this, noParam);
	//(void)connect(&b1, &QPushButton::released,
	//	[]()
	//	{
	//		qDebug() << "b1 lambda";
	//	}
	//		
	//	);

	//״̬��
	QStatusBar* pStatusBar = statusBar();
	QLabel* pLabel = new QLabel(this);
	pLabel->setText("Normal text file");
	pStatusBar->addWidget(pLabel);
	pStatusBar->addPermanentWidget(new QLabel("222",this));

	//���Ŀؼ�
	//QTextEdit* pTxtEdit = new QTextEdit(this);
	//setCentralWidget(pTxtEdit);

	//��������
	//QDockWidget* pDock = new QDockWidget(this);
	//addDockWidget(Qt::RightDockWidgetArea, pDock);
	//QTextEdit* pTxtSub = new QTextEdit(this);
	//pDock->setWidget(pTxtSub);

	QLineEdit* ledit = new QLineEdit(this);
	ledit->setText("asdfAAA");
	ledit->setGeometry(50, 200, 100, 30);

	QStringList list;
	list << "111" << "2222" << "333";
	
	//�����б���ʾ��ȫ��Ϣ
	QCompleter* com = new QCompleter(list, this);
	com->setCaseSensitivity(Qt::CaseInsensitive);

	ledit->setCompleter(com);

	//QLabel* lab = new QLabel("xxx", this);
	//lab->setGeometry(100, 100, 500, 300);
	//lab->setPixmap(QPixmap(":/images/test.jpg"));
	//lab->setScaledContents(true);

	QMovie* Movie = new QMovie(":/images/test.jpg");

	//����styleSheet
	ui.testCss->setStyleSheet("QLabel{color:red;}");

	resize(720, 480);
	setFixedSize(this->width(),this->height());
}

qt_test::~qt_test()
{

}

void qt_test::mySlots()
{
	qDebug() << "b1 111";
}

void qt_test::mySlots(int i)
{
	qDebug() << "b1 111, i=" << i;
}

void qt_test::newFile()
{
	qDebug() << "new file";
}

//ģ̬�Ի���
void qt_test::fileOpen()
{
	QDialog dlg;
	dlg.resize(200, 100);
	dlg.exec();
	qDebug() << "next dlg";
}

//��ģ̬�Ի���
void qt_test::fileOpen2()
{
	dlg2.resize(200, 100);
	dlg2.show();
	qDebug() << "next dlg2";
}

void qt_test::message()
{
	QMessageBox::about(this, "box", "hello!!!");
}

void qt_test::question()
{
	QMessageBox::StandardButton ret = QMessageBox::question(this,
		"question", "Are you OK?", 
		QMessageBox::Ok | QMessageBox::Yes | QMessageBox::No
	);
	switch (ret)
	{
	case QMessageBox::Yes:
		qDebug() << "yes";
		break;
	case QMessageBox::No:
		qDebug() << "no";
		break;
	default:
		break;
	}

}

void qt_test::fileOpen3()
{
	QFileDialog::getOpenFileName(this, "open", "./", "Text(*.txt);;All(*.*)");
}