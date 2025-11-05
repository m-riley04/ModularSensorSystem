#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "controllers/maincontroller.h"
#include "controllers/mountcontroller.h"
#include "widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h"
#include "widgets/CustomTitleBar/customtitlebar.h"
#include "data/requried_actions.h"

// Forward declaration for element tree node
struct ElementTreeNode;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ElementTreeActions getElementTreeActions() const;

private slots:
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

    void onSelectedElementChanged(ElementTreeNode* node);
    void onSelectedElementRemoved();

    void openGithubRepository();

    void onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void updateToolbarButtonsState();

	void onPrintPipelineDebugClicked();

public slots:
    void quit();
    void restart();

private:
    Ui::MainWindowClass ui;
    MainController* pController;

    QListWidgetItem* pSelectedPresetItem = nullptr;
	ElementTreeNode* m_selectedElement = nullptr;

    void initWidgets();
    void initSignals();
    void initActionSignals();
};
