#include "devicepropertieswidget.h"

DevicePropertiesWidget::DevicePropertiesWidget(Device* device, QWidget *parent)
	: QWidget(parent), pDevice(device)
{
	ui.setupUi(this);
}

DevicePropertiesWidget::~DevicePropertiesWidget()
{}
