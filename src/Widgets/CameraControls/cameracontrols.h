#pragma once

#include <QWidget>
#include <QtMultimedia>
#include "ui_cameracontrols.h"
#include "Devices/CameraDevice/cameradevice.h"
#include <Windows/QtCameraControlsWindow/QtCameraControlsDialog.h>
#include <Windows/QtRecordingControlsDialog/qtrecordingcontrolsdialog.h>
#include "Helpers/unitconversionhelper.h"

class CameraControls : public QWidget
{
	Q_OBJECT

public:
	CameraControls(QWidget* parent = nullptr, CameraDevice* camera = nullptr);
	~CameraControls();

private:
	Ui::CameraControlsClass ui;
	CameraDevice* pCamera = nullptr;
	QStorageInfo storageInfo;
	bool areSignalsInitialized = false;

	void initSignals();
	void initWidgets();

	void updateStorageLabel(qint64 availableBytes);

	void populateAudioDevices();

public slots:
	void setCamera(CameraDevice* camera);

signals:
	void cameraChanged(CameraDevice* camera);
};
