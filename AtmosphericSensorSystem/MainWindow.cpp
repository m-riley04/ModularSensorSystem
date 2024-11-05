#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Initialize
    initCamera();
    initSignals();
    initWidgets();
}

MainWindow::~MainWindow()
{}

void MainWindow::initWidgets() {

}

void MainWindow::initCamera() {
    // Create camera
    camera = new Camera(this);
    
    // Initialize the timer to capture frames
    frameTimer = camera->frameTimer();
    connect(frameTimer, &QTimer::timeout, this, [this]() {
        if (camera->isOpened()) {
            cv::Mat frame;
            *camera >> frame;
            if (!frame.empty()) {
                displayFrame(frame);
            }
        }
    });
}

void MainWindow::initSignals() {
    // Buttons
    connect(ui.buttonRecord, &QPushButton::clicked, camera, &Camera::startRecording); /// TODO: Change this wording to be more like "toggleRecording"
    connect(ui.buttonOpenOutputDirectory, &QPushButton::clicked, this, &MainWindow::openOutputDirectory);
    connect(ui.buttonSetOutputDirectory, &QPushButton::clicked, this, &MainWindow::setOutputDirectory);
    connect(ui.buttonStop, &QPushButton::clicked, camera, &Camera::release);
    connect(ui.buttonStart, &QPushButton::clicked, camera, &Camera::open);
    connect(ui.buttonRestart, &QPushButton::clicked, camera, &Camera::restart);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
    connect(ui.actionCameraProperties, &QAction::triggered, this, &MainWindow::openCameraProperties);
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
    QDesktopServices::openUrl(camera->outputDirectory());
}

void MainWindow::setOutputDirectory() {
    QUrl temp = QFileDialog::getExistingDirectoryUrl(this, "Select the output directory");
    if (!temp.isEmpty()) {
        camera->setOutputDirectory(temp);
    }
}

void MainWindow::openCameraProperties() {
    CameraPropertiesWindow *w = new CameraPropertiesWindow(this, camera);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}