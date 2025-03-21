#pragma once

#include <QWidget>
#include "ui_custompantiltwidget.h"

class CustomPanTiltWidget : public QWidget
{
	Q_OBJECT

public:
	CustomPanTiltWidget(QWidget *parent = nullptr);
	~CustomPanTiltWidget();

private:
	Ui::CustomPanTiltWidgetClass ui;
};
