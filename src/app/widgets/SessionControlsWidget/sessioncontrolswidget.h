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
		initSignals();
		updateUi();
	}

private:
	Ui::SessionControlsWidgetClass ui;

	SessionControlsActions m_actions{};

	void initSignals();
	
private slots:
	void updateUi();
};
