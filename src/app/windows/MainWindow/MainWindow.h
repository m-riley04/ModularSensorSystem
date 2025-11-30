#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "controllers/maincontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "data/required_actions.hpp"
#include "dialogs/PluginsDialog/pluginsdialog.h"
#include <controllers/AppActionController/appactioncontroller.h>

// Forward declaration for element tree node
struct ElementTreeNode;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void updateToolbarButtonsState();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    AppActions createActions();
    void initWidgets();
    void initSignals();

	void loadAppSettings();

    ElementTreeActions getElementTreeActions() const;

    void syncViewActionChecks();

    Ui::MainWindowClass ui;
    MainController m_controller;
    QSettings m_appSettings;
    QListWidgetItem* pSelectedPresetItem = nullptr;
    AppActionController* m_actionController = nullptr;
};
