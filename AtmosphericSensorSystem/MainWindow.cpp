#include "MainWindow.h"

bool checkCameraAvailability()
{
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void MainWindow::initWidgets() {
    // Initialize the timer to capture frames
    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, [this]() {
        if (camera.isOpened()) {
            cv::Mat frame;
            camera >> frame;
            if (!frame.empty()) {
                displayFrame(frame);
            }
        }
        });
    
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
    for (int i = 0; i < 10; i++) {
        cv::VideoCapture _tempCam(i);
        if (_tempCam.isOpened()) {
            deviceNames.push_back(QString("Camera %1").arg(i));
            _tempCam.release();
        }
    }
    ui.dropdownDevice->insertItems(0, deviceNames);
}

void MainWindow::updateDevice(int deviceIndex) {
    if (camera.isOpened()) {
        camera.release();
    }

    camera.open(deviceIndex);

    if (camera.isOpened()) {
        startCamera();
    }
}

void MainWindow::displayFrame(const cv::Mat& frame) {
    // Convert the frame to QImage for displaying in QLabel
    QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui.video->setPixmap(QPixmap::fromImage(image).scaled(ui.video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_BRIGHTNESS, value / 100.0);
    }
}

void MainWindow::setContrast(int value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_CONTRAST, value / 100.0);
    }
}

void MainWindow::setSaturation(int value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_SATURATION, value / 100.0);
    }
}

void MainWindow::setGain(int value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_GAIN, value);
    }
}

void MainWindow::setBacklight(bool value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_BACKLIGHT, value ? 1 : 0);
    }
}

void MainWindow::setCameraDevice() {

}

void MainWindow::startCamera() {
    if (camera.isOpened()) {
        frameTimer->start(30);  // Adjust the interval for desired frame rate
    }
}

void MainWindow::stopCamera() {
    frameTimer->stop();
    if (camera.isOpened()) {
        camera.release();
    }
}

void MainWindow::record() {
    //mediaRecorder->record();
}

void MainWindow::pause() {
    //mediaRecorder->pause();
}

void MainWindow::stop() {
    //mediaRecorder->stop();
    
}

void MainWindow::openOutputDirectory() {
    QDesktopServices::openUrl(outputDir);
}

void MainWindow::setOutputDirectory() {
    outputDir = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    
    // TODO: Set output directory for CV 
}