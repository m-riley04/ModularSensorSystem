#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_cameracontrols.h"
#include <Sensors/Camera/Camera.h>
#include <Windows/QtCameraControlsWindow/QtCameraControlsDialog.h>
#include <Windows/QtRecordingControlsDialog/qtrecordingcontrolsdialog.h>

class CameraControls : public QWidget
{
	Q_OBJECT

public:
	CameraControls(QWidget* parent = nullptr, Camera* camera = nullptr);
	~CameraControls();

private:
	Ui::CameraControlsClass ui;
	Camera* pCamera = nullptr;
	QStorageInfo storageInfo;
	bool areSignalsInitialized = false;

	void initSignals();
	void initWidgets();

	void updateStorageLabel(qint64 availableBytes);

	void populateAudioDevices();

public slots:
	void setCamera(Camera* camera);

signals:
	void cameraChanged(Camera* camera);
};
