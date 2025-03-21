#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	// Initialize the main controller
	pController = std::make_unique<MainController>(this);

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

void MainWindow::initWidgets()
{
	// Set default page to home
	ui.stackedWidget->setCurrentIndex(0);

	// Set the sensor controller
	ui.groupControls->setSensorController(pController->sensorController());
	ui.groupControls->setController(pController.get());

	// Set the camera controls
	ui.cameraControls->setCamera(nullptr); // No camera selected by default

	// Set the tab to be empty
	ui.tabCameras->clear();
}

void MainWindow::initSignals() {
    // Pages
	connect(ui.buttonHome, &QPushButton::clicked, [this]() {
		ui.stackedWidget->setCurrentIndex(0);
		});
    connect(ui.buttonPlayback, &QPushButton::clicked, [this]() {
        ui.stackedWidget->setCurrentIndex(1);
        });

    // Camera View
    connect(ui.buttonAddSensor, &QPushButton::clicked, this, &MainWindow::openAddCameraDialog);
    connect(ui.buttonRemoveSensor, &QPushButton::clicked, this, &MainWindow::removeCamera);
    connect(pController->sensorController(), &SensorController::cameraAdded, this, &MainWindow::addVideoWidget);

    // Camera Controls
    connect(ui.tabCameras, &QTabWidget::currentChanged, [this](int index) {
        if (index == -1 || !(index < mSinkWidgets.size())) return;

        // Get the camera from the video widgets
		Camera* cam = pController->sensorController()->findCamera(mSinkWidgets.at(index));

        // Emit the camera changed signal
        emit cameraChanged(cam);
    });

	connect(this, &MainWindow::cameraChanged, ui.cameraControls, &CameraControls::setCamera);

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
    auto sinkWidget = new CustomSinkWidget(this);
    mSinkWidgets.push_back(sinkWidget);
    QString name = camera->camera()->cameraDevice().description();
    camera->setVideoOutput(sinkWidget); // Set output BEFORE adding tab
    ui.tabCameras->addTab(sinkWidget, name);
}

void MainWindow::openAddCameraDialog()
{
    AddCameraDialog* addDialog = new AddCameraDialog(this, pController->sensorController()->videoDevices());
    connect(addDialog, &AddCameraDialog::deviceSelected, pController->sensorController(), &SensorController::addCamera);
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
        Camera* cam = pController->sensorController()->findCamera(videoWidget);

        // Remove video widget from UI
        videoWidget->deleteLater();

        // Remove camera and video widget
        pController->sensorController()->removeCamera(cam);
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