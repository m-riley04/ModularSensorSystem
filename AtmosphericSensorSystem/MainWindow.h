#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    QCamera* camera;
    QMediaCaptureSession captureSession;
    QMediaRecorder* mediaRecorder;

    void initWidgets();
    void initSignals();

private slots:
    void setCameraDevice();
    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();

    void setBrightness(int value);
    void setContrast(int value);
    void setSaturation(int value);
    void setGain(int value);
    void setBacklight(bool value);

    void openVideos();

    void updateDeviceDropdown();
    void updateDevice(int deviceIndex);
    void updateSpaceRemaining();

};
