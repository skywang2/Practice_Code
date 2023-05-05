#include "qanimationtest.h"
#include <QPalette>
#include <QSequentialAnimationGroup>
#include <QTimeLine>

qAnimationTest::qAnimationTest(QWidget *parent)
	: QMainWindow(parent)
	, m_moveFlag(0)
{
	this->resize(400, 300);
	m_labTest = new QLabel("test", this);
	m_btnStart = new QPushButton("start1", this);
	m_btnStart2 = new QPushButton("start2", this);

	m_btnStart->setGeometry(QRect(280, 240, 75, 23));
	m_btnStart2->setGeometry(QRect(280, 270, 75, 23));

	//label背景色等
	QPalette palette = m_labTest->palette();
	palette.setColor(QPalette::Background, QColor(0, 255, 0));
	m_labTest->setPalette(palette);
	m_labTest->setAutoFillBackground(true);
	m_labTest->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	m_labTest->setGeometry(QRect(0, 0, 40, 40));

	m_animation = new QPropertyAnimation(m_labTest, "geometry", this);//geometry是Q_PROPERTY指定的属性，QWidget类的属性
	m_animation->setEasingCurve(QEasingCurve::InElastic);//差值曲线

	//QTimeLine* timeLine = new QTimeLine(5000);
	//timeLine->setFrameRange(0, 2000);
	//connect(timeLine, &QTimeLine::frameChanged, this, &qAnimationTest::OnButtonReleaseStart);

	connect(m_btnStart, &QPushButton::released, this, &qAnimationTest::OnButtonReleaseStart);
	connect(m_btnStart2, &QPushButton::released, this, &qAnimationTest::OnButtonReleaseStart2);

	//timeLine->setLoopCount(0);
	//timeLine->start();
}

qAnimationTest::~qAnimationTest()
{
}

void qAnimationTest::OnButtonReleaseStart()
{
	QRect rect = m_labTest->rect();

	if (0 == m_moveFlag)
	{
		m_animation->setStartValue(QRect(0, 0, rect.width(), rect.height()));
		m_animation->setEndValue(QRect(200, 0, rect.width()*2, rect.height()*2));
		m_animation->setDuration(3000);
		m_animation->start();
		m_moveFlag = 1;
	}
	else if (1 == m_moveFlag)
	{
		m_animation->setEndValue(QRect(0, 0, rect.width()/2, rect.height()/2));
		m_animation->setStartValue(QRect(200, 0, rect.width(), rect.height()));
		m_animation->setDuration(3000); 
		m_animation->start();
		m_moveFlag = 0;
	}
}

void qAnimationTest::OnButtonReleaseStart2()
{
	QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
	QRect rect = m_labTest->rect();

	QPropertyAnimation* down = new QPropertyAnimation(m_labTest, "geometry", this);
	down->setStartValue(QRect(0, 0, rect.width(), rect.height()));
	down->setEndValue(QRect(0, 100, rect.width(), rect.height()));
	down->setDuration(1000);

	QPropertyAnimation* right = new QPropertyAnimation(m_labTest, "geometry", this);
	right->setStartValue(QRect(0, 100, rect.width(), rect.height()));
	right->setEndValue(QRect(100, 100, rect.width(), rect.height()));
	right->setDuration(1000);

	group->addAnimation(down);
	group->addPause(1000);//添加暂停
	group->addAnimation(right);
	group->start(QAbstractAnimation::DeleteWhenStopped);//播放后删除
}
