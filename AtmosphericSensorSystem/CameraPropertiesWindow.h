#pragma once

#include <QDialog>
#include <QtConcurrent>
#include "Camera.h"
#include "ui_CameraPropertiesWindow.h"

class CameraPropertiesWindow : public QDialog
{
	Q_OBJECT

public:
	CameraPropertiesWindow(QWidget *parent = nullptr, Camera *camera = nullptr);
	~CameraPropertiesWindow();
	
signals:
	void brightnessChanged(int value);
	void contrastChanged(int value);
	void saturationChanged(int value);
	void gainChanged(int value);
	void exposureChanged(int value);
	void backlightChanged(bool value);
	void autoExposureChanged(bool value);

private:
	Ui::CameraPropertiesWindowClass ui;
	Camera* camera;

	void initWidgets();
	void initSignals();
	void updateDeviceDropdown();
};
