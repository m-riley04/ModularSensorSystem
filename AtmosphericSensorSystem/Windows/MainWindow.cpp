#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    controller = new SensorController(this);

    // Initialize
    initSignals();
    initWidgets();
}

MainWindow::~MainWindow()
{
    // Delete controller
    delete controller;

    // Remove all generated video widgets
    for (QVideoWidget* widget : videoWidgets) {
        delete widget;
    }
}

void MainWindow::initWidgets() {

}

void MainWindow::initSignals() {
    // Camera Controls
    connect(ui.buttonStop, &QPushButton::clicked, controller, &SensorController::stopSensors);
    connect(ui.buttonStart, &QPushButton::clicked, controller, &SensorController::startSensors);

    // Recording Controls
    connect(ui.buttonRecord, &QPushButton::clicked, this, &MainWindow::clicked_record);
    connect(ui.buttonStopRecording, &QPushButton::clicked, this, &MainWindow::clicked_stop);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::addVideoWidget(Camera* camera)
{
    QVideoWidget* videoWidget = new QVideoWidget();
    videoWidgets.push_back(videoWidget);
    QString tabName = "Camera " + videoWidgets.length();
    ui.tabCameras->addTab(videoWidget, tabName);
    camera->setVideoWidget(videoWidget);
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}