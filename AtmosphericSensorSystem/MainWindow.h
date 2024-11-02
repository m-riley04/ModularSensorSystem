#pragma once

#define DEFAULT_BRIGHTNESS 0
#define DEFAULT_CONTRAST 10
#define DEFAULT_SATURATION 10
#define DEFAULT_GAIN 100
#define DEFAULT_BACKLIGHT 0
#define DEFAULT_WHITE_BALANCE 4500
#define DEFAULT_AUTO_EXPOSURE 1

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    Camera* camera;
    QUrl outputDir;

    void initWidgets();
    void initSignals();
    void displayFrame(const cv::Mat& frame);

private slots:
    void openOutputDirectory();
    void setOutputDirectory();

    void updateDeviceDropdown();
    void updateDevice(int deviceIndex);
    /*void updateSpaceRemaining();*/

};
