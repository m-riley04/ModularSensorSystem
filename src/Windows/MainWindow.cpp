#include "MainWindow.h"
#include "QtCameraControlsWindow/qtcameracontrolsdialog.h"
#include <Widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h>

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
    //ui.cameraControls->setCamera(pCamera);
}

void MainWindow::initSignals() {
    // Camera View
    connect(ui.buttonAddSensor, &QPushButton::clicked, this, &MainWindow::addCamera);
    connect(ui.buttonRemoveSensor, &QPushButton::clicked, this, &MainWindow::removeCamera);
    connect(pController.get(), &SensorController::cameraAdded, this, &MainWindow::addVideoWidget);

    // Camera Controls
    connect(ui.tabCameras, &QTabWidget::currentChanged, [this](int index) {
        if (index == -1 || !(index < mVideoWidgets.size())) return;

        // Get the camera from the video widgets
		Camera* cam = pController->findCamera(mVideoWidgets.at(index));

        // Emit the camera changed signal
        emit cameraChanged(cam);
    });

	connect(this, &MainWindow::cameraChanged, ui.cameraControls, &CameraControls::setCamera);
    
    // Recording
	connect(ui.buttonRecord, &QPushButton::clicked, [this]() {
        if (!isRecording) {
            ui.buttonRecord->setText("Stop Recording");
        }
        else {
            ui.buttonRecord->setText("Record");
        }

        pController->recordCameras();

        isRecording = !isRecording;

		});

    connect(ui.buttonPlayback, &QPushButton::clicked, [this]() {
        // Get files to open for playback
        QStringList files = QFileDialog::getOpenFileNames(this, "Open recorded files");

        if (files.length() <= 0) return; // Empty list

        SimultaneousMediaPlayer* player = new SimultaneousMediaPlayer(this, files);
        player->show();
        });

    // Menu Bar
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui.actionRestart, &QAction::triggered, this, &MainWindow::restart);
}

void MainWindow::addVideoWidget(Camera *camera)
{
    auto videoWidget = new QVideoWidget(this);
    mVideoWidgets.push_back(videoWidget);
    QString name = camera->camera()->cameraDevice().description();
    camera->setOutput(videoWidget); // Set output BEFORE adding tab
    ui.tabCameras->addTab(videoWidget, name);
}

void MainWindow::addCamera()
{
    AddCameraDialog* addDialog = new AddCameraDialog(this, pController->videoDevices());
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