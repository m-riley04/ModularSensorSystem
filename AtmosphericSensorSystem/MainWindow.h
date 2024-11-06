#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia>
#include <QFileDialog>
#include <QVariant>
#include <qmessagebox.h>
#include <libusb-1.0/libusb.h>
#include <opencv2/opencv.hpp>
#include "CameraPropertiesWindow.h"
#include "Controllers/SensorController.h"
#include "Sensors/Camera.h"
#include "ui_MainWindow.h"
#include "Writers/VideoWriter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    Camera* camera;
	VideoWriter* videoWriter;
    QUrl outputDir;
	SensorController* sensorController;
    bool isRecording = false;

    void initSensors();
    void initWidgets();
    void initSignals();
    void displayFrame(const cv::Mat& frame);
    void detectSensors();

public slots:
    void openOutputDirectory();
    void setOutputDirectory();

    void openCameraProperties();

	void record_clicked();

    void quit();
    void restart();

    /*void updateSpaceRemaining();*/

};
