#pragma once

#include <QDialog>
#include "ui_sessionpropertieswidget.h"

class SessionPropertiesWidget : public QDialog
{
	Q_OBJECT

public:
	SessionPropertiesWidget(QWidget *parent = nullptr);
	~SessionPropertiesWidget();

private:
	Ui::SessionPropertiesWidgetClass ui;
};

