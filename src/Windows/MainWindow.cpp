#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    pController = std::make_unique<SensorController>(this);

    // Initialize
    initSignals();
    initWidgets();
}

MainWindow::~MainWindow()
{
    mVideoWidgets.clear();
}

void MainWindow::initWidgets() {

}

void MainWindow::initSignals() {
    // Camera Controls
    connect(ui.buttonStop, &QPushButton::clicked, pController.get(), &SensorController::stopSensors);
    connect(ui.buttonStart, &QPushButton::clicked, pController.get(), &SensorController::startSensors);

    // Camera View
    connect(ui.buttonAddSensor, &QPushButton::clicked, this, &MainWindow::addCamera);
    connect(ui.buttonRemoveSensor, &QPushButton::clicked, this, &MainWindow::removeCamera);
    connect(pController.get(), &SensorController::cameraAdded, this, &MainWindow::addVideoWidget);

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::addVideoWidget(Camera *camera)
{
    auto videoWidget = new QVideoWidget(this);
    mVideoWidgets.push_back(videoWidget);
    QString tabName = "Camera " + mVideoWidgets.size();
    ui.tabCameras->addTab(videoWidget, tabName);
    camera->setOutput(videoWidget);
}

void MainWindow::addCamera()
{
    AddCameraDialog* addDialog = new AddCameraDialog();
    connect(addDialog, &AddCameraDialog::deviceSelected, pController.get(), &SensorController::addCamera);
    addDialog->show();
}

void MainWindow::removeCamera()
{
    const QString title = "Delete";
    const QString text = "Are you sure you want to remove this camera?";
    QMessageBox::StandardButton reply = QMessageBox::question(this, title, text, QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

    // Check output
    if (reply == QMessageBox::Yes)
    {
        // Find the selected tab
        int currentTabIndex = ui.tabCameras->currentIndex();
        if (currentTabIndex == -1 || !(currentTabIndex < mVideoWidgets.size()))
        {
            QMessageBox::warning(this, "Error", "The selected camera does not exist and cannot be deleted.", QMessageBox::StandardButton::Ok);
            return;
        }

        // Find camera and video widget
        auto &videoWidget = mVideoWidgets.at(currentTabIndex);
        Camera* cam = pController->findCamera(videoWidget);

        // Remove video widget from UI
        videoWidget->deleteLater();

        // Remove camera and video widget
        pController->removeCamera(cam);
        mVideoWidgets.erase(mVideoWidgets.begin() + currentTabIndex);
    }
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}