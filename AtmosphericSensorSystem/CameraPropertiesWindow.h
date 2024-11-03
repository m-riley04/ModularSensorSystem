#pragma once

#include <QDialog>
#include "ui_CameraPropertiesWindow.h"

class CameraPropertiesWindow : public QDialog
{
	Q_OBJECT

public:
	CameraPropertiesWindow(QWidget *parent = nullptr);
	~CameraPropertiesWindow();

private:
	Ui::CameraPropertiesWindowClass ui;
};
