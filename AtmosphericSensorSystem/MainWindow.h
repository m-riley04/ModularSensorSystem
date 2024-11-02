#pragma once

#define DEFAULT_BRIGHTNESS 100
#define DEFAULT_CONTRAST 100
#define DEFAULT_SATURATION 100
#define DEFAULT_GAIN 100
#define DEFAULT_BACKLIGHT 0

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    cv::VideoCapture camera;
    QUrl outputDir;
    QTimer* frameTimer;

    void initWidgets();
    void initSignals();
    void displayFrame(const cv::Mat& frame);

private slots:
    void setCameraDevice();

    void restartCamera();
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
    /*void setFPS();
    void setFrameWidth();
    void setFrameHeight();
    void setHue();
    void setExposure();
    void setSharpness();
    void setGamma();
    void setAutoExposure(bool value);
    void setBitrate();*/

    void openOutputDirectory();
    void setOutputDirectory();

    void updateRecorderState(QMediaRecorder::RecorderState state);
    void updateDeviceDropdown();
    void updateDevice(int deviceIndex);
    /*void updateSpaceRemaining();*/

};
