#include "groupcontrols.h"

GroupControls::GroupControls(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// Recording
	connect(ui.buttonRecord, &QPushButton::clicked, [this]() {
		ui.buttonRecord->setText(isRecording ? "Record" : "Stop Recording");

		pController->recordCameras();

		isRecording = !isRecording;

		});
}

GroupControls::~GroupControls()
{}

void GroupControls::setSensorController(SensorController * controller)
{
	pController = controller;
	emit sensorControllerChanged(pController);
}

void GroupControls::setController(MainController* controller)
{
	pMainController = controller;
	emit mainControllerChanged(pMainController);
}
