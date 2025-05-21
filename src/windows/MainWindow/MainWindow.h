#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QVideoFrame>
#include <QVideoWidget>
#include "dialogs/AddDeviceDialog/adddevicedialog.h"
#include "dialogs/QtCameraControlsDialog/qtcameracontrolsdialog.h"
#include "Widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h"
#include "Controllers/MainController/maincontroller.h"
#include <Controllers/ClipController/clipcontroller.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<MainController> pController;

    void initPages();
    void initWidgets();
    void initSignals();

public slots:
    void quit();
    void restart();
};
