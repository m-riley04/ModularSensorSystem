#include "sessioncontrolswidget.h"
#include <Windows/SessionPropertiesDialog/sessionpropertiesdialog.h>

SessionControlsWidget::SessionControlsWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	updateUi();
}

SessionControlsWidget::~SessionControlsWidget()
{}

void SessionControlsWidget::initSignals()
{
	// Check for null
	if (!pController) { 
		qDebug() << "Cannot init signals for session controls - pController is null";
		return;
	}

	DeviceController* pDeviceController = pController->deviceController();
	RecordingController* pRecordingController = pController->recordingController();

	// Init open/close button
	connect(ui.buttonOpenCloseDevices, &QPushButton::clicked, [this, pDeviceController]() {
		if (pDeviceController->isOpen() || pDeviceController->isStopped()) {
			pDeviceController->closeDevices();
		}
		else if (pDeviceController->isClosed()) {
			pDeviceController->openDevices();
		}
		});

	// Init start/stop button
	connect(ui.buttonStartStopDevices, &QPushButton::clicked, [this, pDeviceController]() {
		if (pDeviceController->isStopped() || pDeviceController->isOpen()) { // CONSIDER: check recording state too?
			pDeviceController->startDevices();
		}
		else if (pDeviceController->isStarted()) {
			pDeviceController->stopDevices();
		}
		});

	// Recording button
	connect(ui.buttonRecord, &QPushButton::clicked, [this, pRecordingController]() {
		if (pRecordingController->isRecording()) { // CONSIDER: check device state too?
			pRecordingController->stop();
		}
		else if (pRecordingController->isStopped()) {
			pRecordingController->start();
		}
		});

	// Clip button
	connect(ui.buttonClip, &QPushButton::clicked, [this, pDeviceController]() {
		// TODO: Implement clip button
		});

	// Properties button
	connect(ui.buttonProperties, &QPushButton::clicked, [this]() {
		SessionPropertiesDialog* pSessionPropertiesDialog = new SessionPropertiesDialog(pController, this);
		pSessionPropertiesDialog->setAttribute(Qt::WA_DeleteOnClose);
		//pSessionPropertiesDialog->setWindowModality(Qt::WindowModal);
		pSessionPropertiesDialog->show();
		});

	// Init restart button
	connect(ui.buttonRestartDevices, &QPushButton::clicked, [this]() {
		ui.buttonRestartDevices->setEnabled(false);
		pController->deviceController()->restartDevices();
		});

	connect(pController->deviceController(), &DeviceController::devicesRestarted, [this]() {
		ui.buttonRestartDevices->setEnabled(true);
		});

	// External UI updates
	connect(pDeviceController, &DeviceController::deviceRemoved, this, &SessionControlsWidget::updateUi);
	connect(pDeviceController, &DeviceController::deviceAdded, this, &SessionControlsWidget::updateUi);
	connect(pDeviceController, &DeviceController::stateChanged, this, &SessionControlsWidget::updateUi);
	connect(pRecordingController, &RecordingController::started, this, &SessionControlsWidget::updateUi);
	connect(pRecordingController, &RecordingController::stopped, this, &SessionControlsWidget::updateUi);
	
}

void SessionControlsWidget::updateUi()
{
	this->setEnabled(!pController);
	if (!pController) return;

	// Check if there are any devices
	bool isEmpty = pController->deviceController()->isEmpty();
	this->setEnabled(!isEmpty);
	ui.buttonOpenCloseDevices->setEnabled(!isEmpty);
	ui.buttonStartStopDevices->setEnabled(!isEmpty);
	ui.buttonRestartDevices->setEnabled(!isEmpty);
	if (isEmpty) return;

	// Device controller updates
	DeviceController::State deviceState = pController->deviceController()->state();
	switch (deviceState) {
	case DeviceController::OPENED:
		ui.buttonOpenCloseDevices->setText("Close Devices");
		ui.buttonStartStopDevices->setEnabled(true);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case DeviceController::CLOSED:
		ui.buttonOpenCloseDevices->setText("Open Devices");
		ui.buttonStartStopDevices->setEnabled(false);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case DeviceController::STARTED:
		ui.buttonStartStopDevices->setText("Stop Devices");
		ui.buttonOpenCloseDevices->setEnabled(false);
		ui.buttonRecord->setEnabled(true);
		ui.buttonClip->setEnabled(true);
		break;
	case DeviceController::STOPPED:
		ui.buttonStartStopDevices->setText("Start Devices");
		ui.buttonOpenCloseDevices->setEnabled(true);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case DeviceController::ERROR:
		ui.buttonOpenCloseDevices->setText("Open Devices");
		ui.buttonStartStopDevices->setText("Start Devices");
		ui.buttonOpenCloseDevices->setEnabled(true);
		ui.buttonStartStopDevices->setEnabled(false);
		break;
	default:
		break;
	}

	// Recording controller updates
	RecordingController::State recState = pController->recordingController()->state();
	switch (recState) {
	case RecordingController::STARTED:
		ui.buttonRecord->setText("Stop Recording");
		//ui.buttonOpenCloseDevices->setEnabled(false);
		//ui.buttonStartStopDevices->setEnabled(false);
		break;
	case DeviceController::STOPPED:
		ui.buttonRecord->setText("Start Recording");
		//ui.buttonOpenCloseDevices->setEnabled(true);
		//ui.buttonStartStopDevices->setEnabled(true);
		break;
	default:
		ui.buttonRecord->setText("Start Recording");
		//ui.buttonOpenCloseDevices->setEnabled(true);
		//ui.buttonStartStopDevices->setEnabled(true);
		break;
	}
}
