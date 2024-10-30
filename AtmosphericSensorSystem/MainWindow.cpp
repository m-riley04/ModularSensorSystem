#include "MainWindow.h"

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    bool hasCamera = checkCameraAvailability();

    if (hasCamera) {
        camera = new QCamera(QMediaDevices::defaultVideoInput());
        captureSession.setCamera(this->camera);
        captureSession.setVideoOutput(ui.video);

        this->camera->start(); // to start the camera
    }
}

MainWindow::~MainWindow()
{}
