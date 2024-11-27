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
#include "ui_MainWindow.h"
#include "Controllers/MainController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass ui;
    MainController* controller = nullptr;

    void initWidgets();
    void initSignals();
    void displayFrame(const cv::Mat& frame);

public slots:
    void openCameraProperties();

    void quit();
    void restart();
};
