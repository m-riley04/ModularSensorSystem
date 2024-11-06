#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include "CameraPropertiesWindow.h"
#include "SensorRecordingScheduler.h"
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
    QTimer* frameTimer;
	SensorRecordingScheduler* scheduler;

    void initSensors();
    void initWidgets();
    void initSignals();
    void displayFrame(const cv::Mat& frame);
    void detectSensors();

public slots:
    void openOutputDirectory();
    void setOutputDirectory();

    void openCameraProperties();

    void quit();
    void restart();

    /*void updateSpaceRemaining();*/

};
