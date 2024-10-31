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
    // Dropdown
    connect(ui.dropdownDevice, &QComboBox::currentIndexChanged, this, &MainWindow::updateDevice);

    // Sliders
    connect(ui.sliderBrightness, &QSlider::valueChanged, this, &MainWindow::setBrightness);
    connect(ui.sliderContrast, &QSlider::valueChanged, this, &MainWindow::setContrast);
    connect(ui.sliderSaturation, &QSlider::valueChanged, this, &MainWindow::setSaturation);
    connect(ui.sliderGain, &QSlider::valueChanged, this, &MainWindow::setGain);
    connect(ui.checkboxBacklight, &QCheckBox::checkStateChanged, this, &MainWindow::setBacklight);

    // Buttons
    connect(ui.buttonRecord, &QPushButton::clicked, this, &MainWindow::record);
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

void MainWindow::setCameraDevice() {

}

void MainWindow::startCamera() {

}

void MainWindow::stopCamera() {

}

void MainWindow::record() {
    
}

void MainWindow::pause() {

}

void MainWindow::stop() {

}