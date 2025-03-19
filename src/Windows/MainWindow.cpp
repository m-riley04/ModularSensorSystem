#include "MainWindow.h"
#include "QtCameraControlsWindow/qtcameracontrolsdialog.h"
#include <Widgets/SimultaneousMediaPlayer/simultaneousmediaplayer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    pController = std::make_unique<SensorController>(this);

    // Initialize core app
    QCoreApplication::setApplicationName("ModularSensorSystem");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("Riley Meyerkorth");
    QCoreApplication::setOrganizationDomain("rileymeyerkorth.com");

    // Initialize
    initWidgets();
    initSignals();
}

MainWindow::~MainWindow()
{
    mSinkWidgets.clear();
}

void MainWindow::initWidgets() {
    
}

void MainWindow::initSignals() {
    // Pages
	connect(ui.buttonHome, &QPushButton::clicked, [this]() {
		ui.stackedWidget->setCurrentIndex(0);
		});
    connect(ui.buttonTest, &QPushButton::clicked, [this]() {
        ui.stackedWidget->setCurrentIndex(1);
        });

    // Connect detection checkbox to current SinkView's detection state
	connect(ui.checkboxDetection, &QCheckBox::checkStateChanged, [this](int state) {
		if (ui.tabCameras->currentIndex() == -1 || !(ui.tabCameras->currentIndex() < mSinkWidgets.size())) return;

		// Find the current SinkView
		auto& videoWidget = mSinkWidgets.at(ui.tabCameras->currentIndex());

		// Set the detection state
		videoWidget->setDetectionState(state == Qt::Checked);
		});

    // Connect debug checkbox to current SinkView's debug state
    connect(ui.checkboxDebugInfo, &QCheckBox::checkStateChanged, [this](int state) {
        if (ui.tabCameras->currentIndex() == -1 || !(ui.tabCameras->currentIndex() < mSinkWidgets.size())) return;

        // Find the current SinkView
        auto& videoWidget = mSinkWidgets.at(ui.tabCameras->currentIndex());

        // Set the debug state
        videoWidget->setDebugInfoVisible(state == Qt::Checked);
        });

    // Camera View
    connect(ui.buttonAddSensor, &QPushButton::clicked, this, &MainWindow::addCamera);
    connect(ui.buttonRemoveSensor, &QPushButton::clicked, this, &MainWindow::removeCamera);
    connect(pController.get(), &SensorController::cameraAdded, this, &MainWindow::addVideoWidget);

    // Camera Controls
    connect(ui.tabCameras, &QTabWidget::currentChanged, [this](int index) {
        if (index == -1 || !(index < mSinkWidgets.size())) return;

        // Get the camera from the video widgets
		Camera* cam = pController->findCamera(mSinkWidgets.at(index));

        // Emit the camera changed signal
        emit cameraChanged(cam);
    });

	connect(this, &MainWindow::cameraChanged, ui.cameraControls, &CameraControls::setCamera);
    
    // Recording
	connect(ui.buttonRecord, &QPushButton::clicked, [this]() {
        ui.buttonRecord->setText(isRecording ? "Record" : "Stop Recording");

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
    auto sink = new SinkView(this);
    mSinkWidgets.push_back(sink);
    QString name = camera->camera()->cameraDevice().description();
    camera->setVideoSink(sink->sink()); // Set output BEFORE adding tab
    ui.tabCameras->addTab(sink, name);
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
        if (currentTabIndex == -1 || !(currentTabIndex < mSinkWidgets.size()))
        {
            QMessageBox::warning(this, "Error", "The selected camera does not exist and cannot be deleted.", QMessageBox::StandardButton::Ok);
            return;
        }

        // Find camera and video widget
        auto &videoWidget = mSinkWidgets.at(currentTabIndex);
        Camera* cam = pController->findCamera(videoWidget);

        // Remove video widget from UI
        videoWidget->deleteLater();

        // Remove camera and video widget
        pController->removeCamera(cam);
        mSinkWidgets.erase(mSinkWidgets.begin() + currentTabIndex);
    }
}

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}