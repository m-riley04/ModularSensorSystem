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
    QCamera* pCamera = nullptr;
    bool isRecording = false;

    void initWidgets();
    void initSignals();

public:
    bool recording() const { return isRecording; }

public slots:
    void addVideoWidget(Camera *camera);
    void addCamera();
    void removeCamera();

    void quit();
    void restart();

signals:
	void cameraChanged(Camera* camera);
    void clicked_record();
    void clicked_stop();
    void clicked_display_video(QVideoWidget *widget);
};
