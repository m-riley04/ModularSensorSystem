#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QVideoFrame>
#include <QVideoWidget>
#include "controllers/MainController/maincontroller.h"
#include "controllers/ClipController/clipcontroller.h"
#include "widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h"
#include "widgets/CustomTitleBar/customtitlebar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<MainController> pController;

    void initStyles();
    void initPages();
    void initWidgets();
    void initSignals();

public slots:
    void quit();
    void restart();
};
