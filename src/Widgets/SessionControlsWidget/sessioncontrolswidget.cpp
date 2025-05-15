#include "sessioncontrolswidget.h"

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
		if (pDeviceController->isStopped() || pDeviceController->isOpen()) {
			pDeviceController->startDevices();
		}
		else if (pDeviceController->isStarted()) {
			pDeviceController->stopDevices();
		}
		});

	// External UI updates
	connect(pDeviceController, &DeviceController::deviceRemoved, this, &SessionControlsWidget::updateUi);
	connect(pDeviceController, &DeviceController::deviceAdded, this, &SessionControlsWidget::updateUi);
	connect(pDeviceController, &DeviceController::stateChanged, this, &SessionControlsWidget::updateUi);

	// Init restart button
	connect(ui.buttonRestartDevices, &QPushButton::clicked, [this]() {
		ui.buttonRestartDevices->setEnabled(false);
		pController->deviceController()->restartDevices();
		});

	connect(pController->deviceController(), &DeviceController::devicesRestarted, [this]() {
		ui.buttonRestartDevices->setEnabled(true);
		});

	
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

	// Check state
	DeviceController::State state = pController->deviceController()->state();
	switch (state) {
	case DeviceController::OPENED:
		ui.buttonOpenCloseDevices->setText("Close Devices");
		ui.buttonStartStopDevices->setEnabled(true);
		break;
	case DeviceController::CLOSED:
		ui.buttonOpenCloseDevices->setText("Open Devices");
		ui.buttonStartStopDevices->setEnabled(false);
		break;
	case DeviceController::STARTED:
		ui.buttonStartStopDevices->setText("Stop Devices");
		ui.buttonOpenCloseDevices->setEnabled(false);
		break;
	case DeviceController::STOPPED:
		ui.buttonStartStopDevices->setText("Start Devices");
		ui.buttonOpenCloseDevices->setEnabled(true);
		break;
	case DeviceController::ERROR:
		ui.buttonOpenCloseDevices->setText("Open Devices");
		ui.buttonStartStopDevices->setEnabled(false);
		break;
	default:
		break;
	}
}
