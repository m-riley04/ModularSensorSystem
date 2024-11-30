#pragma once

#include <QDialog>
#include <QtConcurrent>
#include "ui_CameraPropertiesWindow.h"
#include "../Sensors/Camera.h"

class CameraPropertiesWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::CameraPropertiesWindowClass ui;
	Camera* m_camera;

	void initWidgets();
	void initSignals();
	void updateDeviceDropdown();

public:
	CameraPropertiesWindow(QWidget *parent = nullptr, Camera *m_camera = nullptr);
	~CameraPropertiesWindow();

};
