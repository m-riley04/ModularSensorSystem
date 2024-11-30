#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    p_controller = new SensorController(this);

    // Initialize
    initSignals();
    initWidgets();
}

MainWindow::~MainWindow()
{
    // Delete controller
    delete p_controller;

    // Remove all generated video widgets
    for (QVideoWidget *p : m_videoWidgets) {
        delete p;
    }
}

void MainWindow::initWidgets() {

}

void MainWindow::initSignals() {
    // Camera Controls
    connect(ui.buttonStop, &QPushButton::clicked, p_controller, &SensorController::stopSensors);
    connect(ui.buttonStart, &QPushButton::clicked, p_controller, &SensorController::startSensors);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::addVideoWidget(Camera *camera)
{
    QVideoWidget *videoWidget = new QVideoWidget();
    m_videoWidgets.push_back(videoWidget);
    QString tabName = "Camera " + m_videoWidgets.length();
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