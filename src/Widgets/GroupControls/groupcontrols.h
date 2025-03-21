#pragma once

#include <QWidget>
#include "ui_groupcontrols.h"
#include "Controllers/SensorController/SensorController.h"
#include "Controllers/MainController/maincontroller.h"

class GroupControls : public QWidget
{
	Q_OBJECT

public:
	GroupControls(QWidget *parent = nullptr);
	~GroupControls();

	void setSensorController(SensorController* controller);
	void setController(MainController* controller);

private:
    Ui::GroupControlsClass ui;
    SensorController* pController;
	MainController* pMainController;
    bool isRecording = false;

signals:
	void recordingStarted();
	void recordingStopped();
	void sensorControllerChanged(SensorController* controller);
	void mainControllerChanged(MainController* controller);
	
};
