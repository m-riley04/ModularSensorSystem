#pragma once

#include <QWidget>
#include "ui_SensorListItem.h"

class SensorListItem : public QWidget
{
	Q_OBJECT

public:
	SensorListItem(const QString& portName = "", const QString& className = "", const QString& manufacturerName = "",
		const QString& productName = "", QWidget* parent = nullptr);
	~SensorListItem();

private:
	Ui::SensorListItemClass ui;
};
