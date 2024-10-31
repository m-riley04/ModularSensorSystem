#include "MainWindow.h"

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void MainWindow::initWidgets() {
    // Initialize the devices dropdown
    updateDeviceDropdown();

    // Initialize the camera
    bool hasCamera = checkCameraAvailability();

    if (hasCamera) {
        camera = new QCamera(QMediaDevices::defaultVideoInput());
        captureSession.setCamera(camera);
        captureSession.setVideoOutput(ui.video);

        camera->start(); // to start the camera
    }
}

void MainWindow::initSignals() {
    connect(ui.dropdownDevice, &QComboBox::currentIndexChanged, &MainWindow::updateDevice);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initWidgets();
    initSignals();

}

MainWindow::~MainWindow()
{}


void MainWindow::updateDeviceDropdown() {
    ui.dropdownDevice->clear();
    QStringList deviceNames;
    for (auto& device : QMediaDevices::videoInputs()) {
        deviceNames.push_back(device.description());
    }
    ui.dropdownDevice->insertItems(0, deviceNames);
}

void MainWindow::updateDevice(int deviceIndex) {
    camera = new QCamera(QMediaDevices::videoInputs()[deviceIndex]);
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(ui.video);

    camera->start(); // to start the camera
}

void MainWindow::setBrightness(int value) {
    
}

void MainWindow::setContrast(int value) {

}

void MainWindow::setSaturation(int value) {

}

void MainWindow::setGain(int value) {

}

void MainWindow::setBacklight(bool value) {

}