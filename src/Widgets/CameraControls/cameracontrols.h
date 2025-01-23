#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_cameracontrols.h"
#include <Sensors/Camera/Camera.h>

class CameraControls : public QWidget
{
	Q_OBJECT

public:
	CameraControls(QWidget *parent = nullptr, Camera* camera = nullptr);
	~CameraControls();

private:
	Ui::CameraControlsClass ui;
	Camera* pCamera = nullptr;

public slots:
	void setCamera(Camera* camera);

signals:
	void cameraChanged(Camera* camera);
};
