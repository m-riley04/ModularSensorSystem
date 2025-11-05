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

	SessionController* pSessionController = pController->sessionController();
	SourceController* pSourceController = pController->sourceController();

	// Init start/stop button
	connect(ui.buttonStartStop, &QPushButton::clicked, [this, pSessionController]() {
		if (pSessionController->isPipelineBuilt()) { // TODO/CONSIDER: check recording state too?
			pSessionController->closePipeline();
		}
		else {
			pSessionController->buildPipeline();
		}

		updateUi();
		});

	// Init restart session
	connect(ui.buttonRestart, &QPushButton::clicked, [this, pSessionController]() {
		if (pSessionController->isPipelineBuilt()) { // TODO/CONSIDER: check recording state too?
			pSessionController->closePipeline();
		}

		pSessionController->buildPipeline();
		});

	// Properties button
	connect(ui.buttonProperties, &QPushButton::clicked, [this]() {
		SessionPropertiesDialog* pSessionPropertiesDialog = new SessionPropertiesDialog(pController, this);
		pSessionPropertiesDialog->setAttribute(Qt::WA_DeleteOnClose);
		//pSessionPropertiesDialog->setWindowModality(Qt::WindowModal);
		pSessionPropertiesDialog->show();
		});

	// External UI updates
	// TODO: add ui update for recording state changes, clipping, etc.
	connect(pSourceController, &SourceController::sourceRemoved, this, &SessionControlsWidget::updateUi);
	connect(pSourceController, &SourceController::sourceAdded, this, &SessionControlsWidget::updateUi);	
}

void SessionControlsWidget::updateUi()
{
	this->setEnabled(!pController);
	if (!pController) return;

	// Check if there are any sources
	bool isEmpty = pController->sourceController()->isEmpty();
	this->setEnabled(!isEmpty);
	ui.buttonStartStop->setEnabled(!isEmpty);
	if (isEmpty) return;

	// Device controller updates
	bool deviceState = pController->sessionController()->isPipelineBuilt();
	if (deviceState) {
		ui.buttonStartStop->setText("Stop Session");
	}
	else {
		ui.buttonStartStop->setText("Start Session");
	}
	/*switch (deviceState) {
	case SourceController::OPENED:
		ui.buttonStartStop->setEnabled(true);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case SourceController::CLOSED:
		ui.buttonStartStop->setEnabled(false);
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case SourceController::STARTED:
		ui.buttonStartStop->setText("Stop Session");
		ui.buttonRecord->setEnabled(true);
		ui.buttonClip->setEnabled(true);
		break;
	case SourceController::STOPPED:
		ui.buttonStartStop->setText("Start Session");
		ui.buttonRecord->setEnabled(false);
		ui.buttonClip->setEnabled(false);
		break;
	case SourceController::ERROR:
		ui.buttonStartStop->setText("Start Session");
		ui.buttonStartStop->setEnabled(false);
		break;
	default:
		break;
	}*/
}
