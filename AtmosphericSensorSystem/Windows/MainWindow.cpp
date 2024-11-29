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

    // Recording Controls
    connect(ui.buttonRecord, &QPushButton::clicked, this, &MainWindow::clicked_record);
    connect(ui.buttonStopRecording, &QPushButton::clicked, this, &MainWindow::clicked_stop);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::displayFrame(QVariant data, qint64 timestamp) {
    if (data.canConvert<QVideoFrame>()) {
		QVideoFrame frame = data.value<QVideoFrame>();

        // Convert the frame to QImage for displaying in QLabel
        QImage image = frame.toImage();
        ui.video->setPixmap(QPixmap::fromImage(image).scaled(ui.video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}