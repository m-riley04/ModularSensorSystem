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

    void initStyles();
    void initPages();
    void initWidgets();
    void initActions();
    void initSignals();

public slots:
    void quit();
    void restart();
};
