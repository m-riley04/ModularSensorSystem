#pragma once

#include <QDialog>
#include "ui_sessionpropertiesdialog.h"
#include <controllers/maincontroller.hpp>
#include "models/session_properties.hpp"

class SessionPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	SessionPropertiesDialog(MainController* controller, SessionProperties* currentData, QWidget *parent = nullptr);
	~SessionPropertiesDialog();

private slots:
	void updateUIFromData();
	void transmit(QAbstractButton* button);

private:
	Ui::SessionPropertiesDialogClass ui;

	MainController* m_controller = nullptr;
	SessionProperties* m_currentData = nullptr;
	SessionProperties m_newData{};

signals:
	void settingsChanged(SessionProperties data);

};
