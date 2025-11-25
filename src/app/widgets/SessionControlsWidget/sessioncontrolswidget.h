#pragma once

#include <QWidget>
#include "ui_sessioncontrolswidget.h"
#include "controllers/maincontroller.hpp"
#include "dialogs/SessionPropertiesDialog/sessionpropertiesdialog.h"
#include <data/required_actions.hpp>

class SessionControlsWidget : public QWidget
{
	Q_OBJECT

public:
	SessionControlsWidget(QWidget *parent = nullptr);
	~SessionControlsWidget();

	void setSessionControlActions(SessionControlsActions actions) {
		m_actions = std::move(actions); // TODO: is move necessary here?
		
		ui.buttonStartStop->setAction(m_actions.startStopSession);
		ui.buttonRestart->setAction(m_actions.restartSession);
		ui.buttonRecord->setAction(m_actions.recordSession);
		ui.buttonClip->setAction(m_actions.clipSession);
		ui.buttonProperties->setAction(m_actions.openSessionProperties);
	}

private:
	Ui::SessionControlsWidgetClass ui;

	SessionControlsActions m_actions{};
	
};
