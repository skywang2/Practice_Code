#include "mywidget.h"

myWidget::myWidget(QWidget *parent)
	: QWidget(parent)
{
	QSpinBox* spin = new QSpinBox(this);
	QSlider* slider = new QSlider(Qt::Horizontal, this);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(spin);
	hLayout->addWidget(slider);
}

myWidget::~myWidget()
{
}
