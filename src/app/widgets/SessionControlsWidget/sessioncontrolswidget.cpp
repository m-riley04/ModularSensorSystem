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
	// Init start/stop button
	/*connect(ui.buttonStartStop, &QPushButton::clicked, m_actions.startStopSession, &QAction::toggle);
	connect(ui.buttonRestart, &QPushButton::clicked, m_actions.restartSession, &QAction::trigger);
	connect(ui.buttonStartStop, &QPushButton::clicked, m_actions.startStopSession, &QAction::toggle);*/

	// Properties button
	//connect(ui.buttonProperties, &QPushButton::clicked, [this]() {
	//	SessionPropertiesDialog* pSessionPropertiesDialog = new SessionPropertiesDialog(pController, this);
	//	pSessionPropertiesDialog->setAttribute(Qt::WA_DeleteOnClose);
	//	//pSessionPropertiesDialog->setWindowModality(Qt::WindowModal);
	//	pSessionPropertiesDialog->show();
	//	});

	// External UI updates
	// TODO: add ui update for recording state changes, clipping, etc.
	/*connect(pSourceController, &SourceController::sourceRemoved, this, &SessionControlsWidget::updateUi);
	connect(pSourceController, &SourceController::sourceAdded, this, &SessionControlsWidget::updateUi);	*/
}

void SessionControlsWidget::updateUi()
{
	//this->setEnabled(m_actions.startStopSession != nullptr); // TODO: check other actions too?

	//// Check if there are any sources
	//bool isEmpty = pController->sourceController()->isEmpty();
	//this->setEnabled(!isEmpty);
	//ui.buttonStartStop->setEnabled(!isEmpty);
	//if (isEmpty) return;

	//// Device controller updates
	//bool deviceState = pController->sessionController()->isPipelineBuilt();
	//if (deviceState) {
	//	ui.buttonStartStop->setText("Stop Session");
	//}
	//else {
	//	ui.buttonStartStop->setText("Start Session");
	//}
	
}
