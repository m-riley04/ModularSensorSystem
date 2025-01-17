#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "../Controllers/SensorController.h"
#include <QVideoFrame>
#include <QVideoWidget>
#include "AddCameraDialog/AddCameraDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    std::unique_ptr<SensorController> pController;
    std::vector<QVideoWidget*> mVideoWidgets;

    void initWidgets();
    void initSignals();

public slots:
    void addVideoWidget(Camera *camera);
    void addCamera();
    void removeCamera();

    void quit();
    void restart();

signals:
    void clicked_record();
    void clicked_stop();
    void clicked_display_video(QVideoWidget *widget);
};
