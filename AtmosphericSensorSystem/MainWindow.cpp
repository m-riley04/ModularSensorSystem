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
    ui.checkboxBacklight->setChecked(DEFAULT_BACKLIGHT);
    ui.checkboxAutoExposure->setChecked(DEFAULT_AUTO_EXPOSURE);
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
    connect(ui.checkboxAutoExposure, &QCheckBox::checkStateChanged, this, &MainWindow::setAutoExposure);

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

    // Write frame to video file if recording
    if (state == STATE_RECORDING && videoWriter.isOpened()) {
        videoWriter.write(frame);
    }
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
        camera.set(cv::CAP_PROP_BRIGHTNESS, value);
    }
}

void MainWindow::setContrast(int value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_CONTRAST, value);
    }
}

void MainWindow::setSaturation(int value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_SATURATION, value);
    }
}

void MainWindow::setGain(int value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_GAIN, value);
    }
}

void MainWindow::setBacklight(bool value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_BACKLIGHT, value);
    }
}

void MainWindow::setAutoExposure(bool value) {
    if (camera.isOpened()) {
        camera.set(cv::CAP_PROP_AUTO_EXPOSURE, value);
    }
}

void MainWindow::setCameraDevice() {

}

void MainWindow::restartCamera() {
    stopCamera();
    startCamera();
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
    /// TODO: Maybe rename this from "record" or make a "toggleRecording" slot
    if (state == STATE_RECORDING) {
        stop();
    }

    if (camera.isOpened()) {
        QString filename = QFileDialog::getSaveFileName(this, "Save Video", outputDir.toString(), "Video Files (*.avi *.mp4)");

        if (!filename.isEmpty()) {
            // Set up VideoWriter with file path, codec, frame rate, and frame size
            int frameWidth = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_WIDTH));
            int frameHeight = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_HEIGHT));
            double fps = 30.0; // Set desired frame rate

            // Use an appropriate codec, e.g., 'MJPG' for .avi, 'MP4V' for .mp4
            videoWriter.open(filename.toStdString(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight), true);

            if (videoWriter.isOpened()) {
                state = STATE_RECORDING;
                ui.buttonRecord->setText("Stop");
                //ui.buttonPause->setEnabled(true);
                //ui.buttonStop->setEnabled(true);
            }
            else {
                QMessageBox::warning(this, "Recording Error", "Failed to open video file for recording.");
            }
        }
    }
}

void MainWindow::pause() {
    //mediaRecorder->pause();
}

void MainWindow::stop() {
    videoWriter.release(); // Stop recording
    ui.buttonRecord->setText("Record");
    state = STATE_IDLE;
    QMessageBox::information(this, "Recording Stopped", "Video recording has been stopped and saved.");
}

void MainWindow::openOutputDirectory() {
    QDesktopServices::openUrl(outputDir);
}

void MainWindow::setOutputDirectory() {
    QUrl temp = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    if (temp.isEmpty()) {
        outputDir = temp;
    }
}