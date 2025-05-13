#pragma once

#include <QWidget>
#include "ui_devicelistwidget.h"

class DeviceListWidget : public QWidget
{
	Q_OBJECT

public:
	DeviceListWidget(QWidget *parent = nullptr);
	~DeviceListWidget();

private:
	Ui::DeviceListWidgetClass ui;
};
