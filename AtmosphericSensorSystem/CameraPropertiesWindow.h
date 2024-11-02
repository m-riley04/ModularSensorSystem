#pragma once

#include <QWidget>
#include "ui_CameraPropertiesWindow.h"

class CameraPropertiesWindow : public QWidget
{
	Q_OBJECT

public:
	CameraPropertiesWindow(QWidget *parent = nullptr);
	~CameraPropertiesWindow();

private:
	Ui::CameraPropertiesWindowClass ui;
};
