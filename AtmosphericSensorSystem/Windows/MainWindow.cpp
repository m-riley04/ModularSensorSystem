#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    pController = new SensorController(this);

    // Initialize
    initSignals();
    initWidgets();
}

MainWindow::~MainWindow()
{
    // Delete controller
    delete pController;

    // Remove all generated video widgets
    for (QVideoWidget *p : mVideoWidgets) {
        delete p;
    }
}

void MainWindow::initWidgets() {

}

void MainWindow::initSignals() {
    // Camera Controls
    connect(ui.buttonStop, &QPushButton::clicked, pController, &SensorController::stopSensors);
    connect(ui.buttonStart, &QPushButton::clicked, pController, &SensorController::startSensors);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::addVideoWidget(Camera *camera)
{
    QVideoWidget *videoWidget = new QVideoWidget();
    mVideoWidgets.push_back(videoWidget);
    QString tabName = "Camera " + mVideoWidgets.length();
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