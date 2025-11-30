#pragma once

#include <QWidget>
#include "ui_sessioncontrolswidget.h"
#include "controllers/maincontroller.hpp"
#include "dialogs/SessionPropertiesDialog/sessionpropertiesdialog.h"
#include <controllers/AppActionController/appactioncontroller.h>

class SessionControlsWidget : public QWidget
{
	Q_OBJECT

public:
	SessionControlsWidget(QWidget *parent = nullptr);
	~SessionControlsWidget();

	void setSessionControlActions(SessionActions* actions) {
		m_actions = actions;
		
		if (!m_actions) return;
		ui.buttonStartStop->setAction(m_actions->toggleSession);
		ui.buttonRestart->setAction(m_actions->restartSession);
		ui.buttonRecord->setAction(m_actions->toggleRecording);
		ui.buttonClip->setAction(m_actions->clipSession);
		ui.buttonProperties->setAction(m_actions->openSessionProperties);
	}

private:
	Ui::SessionControlsWidgetClass ui;

	SessionActions* m_actions = nullptr;
	
};
