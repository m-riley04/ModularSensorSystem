#include "sensorscontainerwidget.h"

SensorsContainerWidget::SensorsContainerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setAcceptDrops(true);
}

SensorsContainerWidget::~SensorsContainerWidget()
{}
