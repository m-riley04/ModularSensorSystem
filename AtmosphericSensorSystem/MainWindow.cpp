#include "MainWindow.h"

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void MainWindow::initWidgets() {
    // Initialize recorder
    mediaRecorder = new QMediaRecorder();
    connect(mediaRecorder, &QMediaRecorder::recorderStateChanged, this, &MainWindow::updateRecorderState);

    // Initialize the camera
    bool hasCamera = checkCameraAvailability();

    if (hasCamera) {
        camera = new QCamera(QMediaDevices::defaultVideoInput());
        captureSession.setCamera(camera);
        captureSession.setVideoOutput(ui.video);
        captureSession.setRecorder(mediaRecorder);

        camera->start(); // to start the camera
    }
    
    // Initialize the devices dropdown
    updateDeviceDropdown();

    // Initialize the display settings
    ui.sliderBrightness->setValue(DEFAULT_BRIGHTNESS);
    ui.sliderContrast->setValue(DEFAULT_CONTRAST);
    ui.sliderSaturation->setValue(DEFAULT_SATURATION);
    ui.sliderGain->setValue(DEFAULT_GAIN);
    ui.checkboxBacklight->setChecked(false);
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
    connect(ui.buttonOpenOutputDirectory, &QPushButton::clicked, this, &MainWindow::openOutputDirectory);
    connect(ui.buttonSetOutputDirectory, &QPushButton::clicked, this, &MainWindow::setOutputDirectory);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initSignals();
    initWidgets();
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

    startCamera();
}

void MainWindow::updateRecorderState(QMediaRecorder::RecorderState state) {
    switch (state) {
        case QMediaRecorder::StoppedState:
            ui.buttonRecord->setEnabled(true);
            break;
        case QMediaRecorder::PausedState:
            ui.buttonRecord->setEnabled(true);
            break;
        case QMediaRecorder::RecordingState:
            ui.buttonRecord->setEnabled(false);
            break;
    }
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
    camera->start();
}

void MainWindow::stopCamera() {
    camera->stop();
}

void MainWindow::record() {
    mediaRecorder->record();
}

void MainWindow::pause() {
    mediaRecorder->pause();
}

void MainWindow::stop() {
    mediaRecorder->stop();
    
}

void MainWindow::openOutputDirectory() {
    QDesktopServices::openUrl(outputDir);
}

void MainWindow::setOutputDirectory() {
    outputDir = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    if (!outputDir.isEmpty()) {
        mediaRecorder->setOutputLocation(outputDir);
    }
}