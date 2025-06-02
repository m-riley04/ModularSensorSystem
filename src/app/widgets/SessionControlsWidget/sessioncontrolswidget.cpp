#include "sessioncontrolswidget.h"
#include <dialogs/SessionPropertiesDialog/sessionpropertiesdialog.h>

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

	SourceController* pSourceController = pController->sourceController();
	RecordingController* pRecordingController = pController->recordingController();

	// Init open/close button
	connect(ui.buttonOpenCloseDevices, &QPushButton::clicked, [this, pSourceController]() {
		if (pSourceController->isOpen() || pSourceController->isStopped()) {
			pSourceController->closeSources();
		}
		else if (pSourceController->isClosed()) {
			pSourceController->openSources();
		}
		});

	// Init start/stop button
	connect(ui.buttonStartStopDevices, &QPushButton::clicked, [this, pSourceController]() {
		if (pSourceController->isStopped() || pSourceController->isOpen()) { // CONSIDER: check recording state too?
			pSourceController->startSources();
		}
		else if (pSourceController->isStarted()) {
			pSourceController->stopSources();
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
	connect(ui.buttonClip, &QPushButton::clicked, [this, pSourceController]() {
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
		pController->sourceController()->restartSources();
		});

	connect(pController->sourceController(), &SourceController::sourcesRestarted, [this]() {
		ui.buttonRestartDevices->setEnabled(true);
		});

	// External UI updates
	connect(pSourceController, &SourceController::sourceRemoved, this, &SessionControlsWidget::updateUi);
	connect(pSourceController, &SourceController::sourceAdded, this, &SessionControlsWidget::updateUi);
	connect(pSourceController, &SourceController::stateChanged, this, &SessionControlsWidget::updateUi);
	connect(pRecordingController, &RecordingController::started, this, &SessionControlsWidget::updateUi);
	connect(pRecordingController, &RecordingController::stopped, this, &SessionControlsWidget::updateUi);
	
}

void SessionControlsWidget::updateUi()
{
	this->setEnabled(!pController);
	if (!pController) return;

	// Check if there are any devices
	bool isEmpty = pController->sourceController()->isEmpty();
	this->setEnabled(!isEmpty);
	ui.buttonOpenCloseDevices->setEnabled(!isEmpty);
	ui.buttonStartStopDevices->setEnabled(!isEmpty);
	ui.buttonRestartDevices->setEnabled(!isEmpty);
	if (isEmpty) return;

	// Device controller updates
	SourceController::State deviceState = pController->sourceController()->state();
	switch (deviceState) {
	case SourceController::OPENED:
		ui.buttonOpenCloseDevices->setText("Close Devices");
		ui.buttonStartStopDevices->setEnabled(true);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case SourceController::CLOSED:
		ui.buttonOpenCloseDevices->setText("Open Devices");
		ui.buttonStartStopDevices->setEnabled(false);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case SourceController::STARTED:
		ui.buttonStartStopDevices->setText("Stop Devices");
		ui.buttonOpenCloseDevices->setEnabled(false);
		ui.buttonRecord->setEnabled(true);
		ui.buttonClip->setEnabled(true);
		break;
	case SourceController::STOPPED:
		ui.buttonStartStopDevices->setText("Start Devices");
		ui.buttonOpenCloseDevices->setEnabled(true);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case SourceController::ERROR:
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
	case SourceController::STOPPED:
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
