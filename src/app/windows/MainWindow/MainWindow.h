#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QVideoFrame>
#include <QVideoWidget>
#include "controllers/MainController/maincontroller.h"
#include "controllers/ClipController/clipcontroller.h"
#include "widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h"
#include "widgets/CustomTitleBar/customtitlebar.h"
#include "pages/MainPage/mainpage.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override; // TODO: Make this cross-platform and implement other platforms

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<MainController> pController;

    QListWidgetItem* pSelectedSourceItem = nullptr;
    QListWidgetItem* pSelectedPresetItem = nullptr;
    QListWidgetItem* pSelectedProcessorItem = nullptr;

    MainPage* pMainPage = nullptr;

    void initStyles();
    void initPages();
    void initWidgets();
    void initSignals();
    void initActionSignals();

    /**
     * @brief Checks whether the specified main controller and its associated controllers are in a valid state.
	 * Logs if any issues are found.
     * @param controller: Pointer to the main controller to be checked.
	 * @return True if all controllers are valid, false otherwise.
     */
    bool checkIfControllersAreOk(MainController* controller) const;

private slots:
    void openSavePresetDialog();
	void onLoadPresetClicked();
    void openDeletePresetDialog();
    void openConfigurePresetDialog();
    void onRefreshPresetClicked();
    void openAddSourceDialog();
    void openRemoveSourceDialog();
    void openConfigureSourceDialog();
	void openAddProcessorDialog();
	void openRemoveProcessorDialog();
	void openConfigureProcessorDialog();

	void onSelectedSourceItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void onSelectedPresetItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void onSelectedProcessorItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void updateToolbarButtonsState();

public slots:
    void quit();
    void restart();
};
