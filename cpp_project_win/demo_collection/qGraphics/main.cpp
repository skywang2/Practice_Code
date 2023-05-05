#include "qgraphics.h"
#include <QDebug>
#include <QtWidgets/QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>

class MyGraphicsView : public QGraphicsView
{
public:
	MyGraphicsView(){}
	MyGraphicsView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR) :QGraphicsView(scene, Q_NULLPTR){}
	~MyGraphicsView(){}
	void mousePressEvent(QMouseEvent *event)
	{
		qDebug() << "press:" << event->pos();
		QGraphicsView::mousePressEvent(event);
	}
public slots:
	void zoomIn(){ scale(1.2, 1.2); }
	void zoomOut(){ scale(1 / 1.2, 1 / 1.2); }
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QGraphicsScene scene;//创建scene
	//QBrush backGroundColor(QColor(243, 219, 203));
	scene.setBackgroundBrush(QColor(243, 219, 203));
	scene.setSceneRect(-300, -300, 600, 600);

	QPen pen;
	pen.setColor(QColor(0, 0, 0));
	pen.setWidth(1);

	QGraphicsRectItem *m_rect = new QGraphicsRectItem();//创建矩形
	m_rect->setRect(100, 0, 60, 90);
	m_rect->setPen(pen);
	m_rect->setBrush(QColor(192, 223, 97));
	m_rect->setPos(QPointF(0.0, 0.0));
	m_rect->setFlag(QGraphicsItem::ItemIsMovable);
	qDebug() << m_rect->pos();
	qDebug() << m_rect->scenePos();

	QGraphicsRectItem *m_rect2 = new QGraphicsRectItem();//创建矩形
	m_rect2->setRect(0, 0, 60, 90);
	m_rect2->setPen(pen);
	m_rect2->setPos(QPointF(0.0, 0.0));
	m_rect2->setFlag(QGraphicsItem::ItemIsMovable);

	QDialog *dialog = new QDialog();//创建操作对话框
	QVBoxLayout *vlayout = new QVBoxLayout();
	QPushButton *m_zoomInBtn = new QPushButton("zoom in");
	QPushButton *m_zoomOutBtn = new QPushButton("zoom out");
	vlayout->addWidget(m_zoomInBtn);
	vlayout->addWidget(m_zoomOutBtn);
	dialog->setLayout(vlayout);

	scene.addItem(m_rect);//加入scene
	scene.addItem(m_rect2);
	scene.addWidget(dialog);

	MyGraphicsView view(&scene);//创建视口
	view.resize(600, 400);
	view.show();

	QObject::connect(m_zoomInBtn, &QPushButton::released, &view, &MyGraphicsView::zoomIn);
	QObject::connect(m_zoomOutBtn, &QPushButton::released, &view, &MyGraphicsView::zoomOut);

	return a.exec();
}
