#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QVideoFrame>
#include <QVideoWidget>
#include "controllers/maincontroller.h"
#include "controllers/clipcontroller.h"
#include "controllers/mountcontroller.h"
#include "widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h"
#include "widgets/CustomTitleBar/customtitlebar.h"
#include "pages/MainPage/mainpage.h"
#include "data/requried_actions.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// Forward declaration for element tree node
struct Node;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ElementTreeActions getElementTreeActions() const;

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override; // TODO: Make this cross-platform and implement other platforms

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

    void onSelectedElementChanged(Node* node);
    void onSelectedElementRemoved();

    void openGithubRepository();

    void onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void updateToolbarButtonsState();

public slots:
    void quit();
    void restart();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<MainController> pController;

    QListWidgetItem* pSelectedPresetItem = nullptr;
	Node* m_selectedElement = nullptr;

    MainPage* pMainPage = nullptr;

    void initStyles();
    void initPages();
    void initWidgets();
    void initSignals();
    void initActionSignals();
};
