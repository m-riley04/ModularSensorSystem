#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include <QFileDialog>
#include <qmessagebox.h>
#include <opencv2/opencv.hpp>
#include "CameraPropertiesWindow.h"
#include "ui_MainWindow.h"
#include <QVariant>
#include "Controllers/SensorController.h"
#include "Sensors/Camera.h"
#include <libusb-1.0/libusb.h>

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
	SensorController* sensorController;

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
