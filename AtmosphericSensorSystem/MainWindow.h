#pragma once

#define DEFAULT_BRIGHTNESS 50
#define DEFAULT_CONTRAST 50
#define DEFAULT_SATURATION 5
#define DEFAULT_GAIN 200

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include <QFileDialog>
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
    QUrl outputDir;

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

    void openOutputDirectory();
    void setOutputDirectory();

    void updateRecorderState(QMediaRecorder::RecorderState state);
    void updateDeviceDropdown();
    void updateDevice(int deviceIndex);
    /*void updateSpaceRemaining();*/

};
