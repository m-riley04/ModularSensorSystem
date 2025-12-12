#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "controllers/maincontroller.hpp"
#include <controllers/AppActionController/appactioncontroller.h>
#include <controllers/UiSettingsController/uisettingscontroller.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MainController& mc, UiSettingsController& uisc, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
	void handleSessionError(const QString& errorMessage);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    /**
     * @brief Creates an AppActions struct with all actions initialized from UI.
	 * @return AppActions struct with sub-structs with action pointers.
     */
    AppActions createActions();
    void initWidgets();
    void initSignals();

	void loadAppSettings();

    void syncViewActionChecks();

    Ui::MainWindowClass ui;
    QSettings m_appSettings;
    MainController& m_controller;
    UiSettingsController& m_uiSettingsController;
    AppActionController* m_actionController = nullptr;
};
