#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "controllers/maincontroller.hpp"
#include "controllers/mountcontroller.hpp"
#include "data/required_actions.hpp"
#include "dialogs/PluginsDialog/pluginsdialog.h"

// Forward declaration for element tree node
struct ElementTreeNode;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenPluginDialog();

    void openSavePresetDialog();
    void onLoadPresetClicked();
    void openDeletePresetDialog();
    void openConfigurePresetDialog();
    void onRefreshPresetClicked();

    void openAddMountDialog();
    void openRemoveMountDialog();
    void openEditMountDialog();

    void openAddSourceDialog();
    void openRemoveSourceDialog();
    void openConfigureSourceDialog();

    void openAddProcessorDialog();
    void openRemoveProcessorDialog();
    void openConfigureProcessorDialog();

    void openGithubRepository();

    void onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void updateToolbarButtonsState();

	void onPrintPipelineDebugClicked();

public slots:
    void quit();
    void restart();

private:
    void initWidgets();
    void initSignals();
    void initActionSignals();

    ElementTreeActions getElementTreeActions() const;

    void syncViewActionChecks();

    Ui::MainWindowClass ui;
    MainController m_controller;

    QListWidgetItem* pSelectedPresetItem = nullptr;
};
