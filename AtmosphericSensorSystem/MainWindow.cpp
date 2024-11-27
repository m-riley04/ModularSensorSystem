#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    controller = new MainController(this);

    // Initialize
    initSignals();
    initWidgets();
}

MainWindow::~MainWindow()
{
    delete controller;
}

void MainWindow::initWidgets() {

}

void MainWindow::initSignals() {
    // Camera Controls
    connect(ui.buttonStop, &QPushButton::clicked, controller, &MainController::stopSensors);
    connect(ui.buttonStart, &QPushButton::clicked, controller, &MainController::startSensors);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
    connect(ui.actionCameraProperties, &QAction::triggered, this, &MainWindow::openCameraProperties);
}

void MainWindow::displayFrame(QVariant data) {
    if (data.canConvert<cv::Mat>()) {
		cv::Mat frame = data.value<cv::Mat>();

        // Convert the frame to QImage for displaying in QLabel
        QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
        ui.video->setPixmap(QPixmap::fromImage(image).scaled(ui.video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    
}

void MainWindow::openCameraProperties() {
    //CameraPropertiesWindow *w = new CameraPropertiesWindow(this, camera);
    //w->setAttribute(Qt::WA_DeleteOnClose);
    //w->show();
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}