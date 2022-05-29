#include "mywidget.h"

myWidget::myWidget(QWidget *parent)
	: QWidget(parent)
{
	QSpinBox* spin = new QSpinBox(this);
	QSlider* slider = new QSlider(Qt::Horizontal, this);
	//QSpacerItem* Spacer = new QSpacerItem(50, 20);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(spin);
	//hLayout->addSpacerItem(Spacer);
	hLayout->addStretch();
	hLayout->addWidget(slider);
	 
	setLayout(hLayout);

	connect(spin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), slider, &QSlider::setValue);
}

myWidget::~myWidget()
{
}
