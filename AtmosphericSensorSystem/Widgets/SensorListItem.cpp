#include "SensorListItem.h"

SensorListItem::SensorListItem(const QString& portName, const QString& className, 
	const QString& manufacturerName, const QString& productName, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.portLabel->setText(portName);
	ui.classLabel->setText(className);
	ui.manufacturerLabel->setText(manufacturerName);
	ui.productLabel->setText(productName);
}

SensorListItem::~SensorListItem()
{}
