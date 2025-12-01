#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "controllers/maincontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "dialogs/PluginsDialog/pluginsdialog.h"
#include <controllers/AppActionController/appactioncontroller.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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
    MainController m_controller;
    QSettings m_appSettings;
    AppActionController* m_actionController = nullptr;
};
