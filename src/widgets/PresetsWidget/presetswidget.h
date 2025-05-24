#pragma once

#include <QWidget>
#include "ui_presetswidget.h"

class PresetsWidget : public QWidget
{
	Q_OBJECT

public:
	PresetsWidget(QWidget *parent = nullptr);
	~PresetsWidget();



private:
	Ui::PresetsWidgetClass ui;
};
