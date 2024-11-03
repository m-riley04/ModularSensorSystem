#include "MainWindow.h"

void MainWindow::initWidgets() {
    // Initialize the timer to capture frames
    connect(frameTimer, &QTimer::timeout, this, [this]() {
        if (camera->isOpened()) {
            cv::Mat frame;
            *camera >> frame;
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
    ui.checkboxBacklight->setChecked(DEFAULT_BACKLIGHT);
    ui.checkboxAutoExposure->setChecked(DEFAULT_AUTO_EXPOSURE);
}

void MainWindow::initSignals() {
    // Dropdown
    connect(ui.dropdownDevice, &QComboBox::currentIndexChanged, this, &MainWindow::updateDevice);

    // Sliders
    connect(ui.sliderBrightness, &QSlider::valueChanged, camera, &Camera::setBrightness);
    connect(ui.sliderContrast, &QSlider::valueChanged, camera, &Camera::setContrast);
    connect(ui.sliderSaturation, &QSlider::valueChanged, camera, &Camera::setSaturation);
    connect(ui.sliderGain, &QSlider::valueChanged, camera, &Camera::setGain);
    connect(ui.checkboxBacklight, &QCheckBox::checkStateChanged, camera, &Camera::setBacklight);
    connect(ui.checkboxAutoExposure, &QCheckBox::checkStateChanged, camera, &Camera::setAutoExposure);

    // Buttons
    connect(ui.buttonRecord, &QPushButton::clicked, camera, &Camera::startRecording); /// TODO: Change this wording to be more like "toggleRecording"
    connect(ui.buttonOpenOutputDirectory, &QPushButton::clicked, this, &MainWindow::openOutputDirectory);
    connect(ui.buttonSetOutputDirectory, &QPushButton::clicked, this, &MainWindow::setOutputDirectory);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Initialize Camera
    camera = new Camera(this);
    frameTimer = camera->getFrameTimer();

    // Initialize signals
    initSignals();

    // Initialize widgets
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
    camera->stop();

    camera->open(deviceIndex);

    if (camera->isOpened()) {
        camera->start();
    }
}

void MainWindow::displayFrame(const cv::Mat& frame) {
    // Convert the frame to QImage for displaying in QLabel
    QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui.video->setPixmap(QPixmap::fromImage(image).scaled(ui.video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Write frame to video file if recording
    /*if (state == STATE_RECORDING && videoWriter.isOpened()) {
        videoWriter.write(frame);
    }*/
}

void MainWindow::openOutputDirectory() {
    QDesktopServices::openUrl(camera->getOutputDirectory());
}

void MainWindow::setOutputDirectory() {
    QUrl temp = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    if (!temp.isEmpty()) {
        camera->setOutputDirectory(temp);
    }
}