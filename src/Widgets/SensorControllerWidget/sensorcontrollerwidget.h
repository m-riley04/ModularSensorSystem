#pragma once

#include <QWidget>
#include "ui_sensorcontrollerwidget.h"
#include "Sensors/Camera/Camera.h"
#include "Widgets/CustomSinkWidget/customsinkwidget.h"
#include <Windows/AddCameraDialog/AddCameraDialog.h>
#include <Controllers/SensorController/SensorController.h>

class SensorControllerWidget : public QWidget
{
	Q_OBJECT

public:
	SensorControllerWidget(QWidget *parent = nullptr, SensorController* sensorController = nullptr);
	~SensorControllerWidget();

private:
	Ui::SensorControllerWidgetClass ui;
	std::vector<CustomSinkWidget*> mSinkWidgets;
	SensorController* pController = nullptr;

	void initWidgets();
	void initSignals();

public slots:
	void addVideoWidget(Camera* camera);
	void openAddCameraDialog();
	void removeCamera();

	void setSensorController(SensorController* sensorController) {
		if (pController == sensorController) return;
		pController = sensorController;

		// Reinitialize connections
		initSignals();

		emit sensorControllerChanged(sensorController);
	}

signals:
	void cameraChanged(Camera* camera);
	void videoWidgetAdded(Camera* camera);
	void controlsChanged(bool enabled);
	void sensorControllerChanged(SensorController* sensorController);
};
