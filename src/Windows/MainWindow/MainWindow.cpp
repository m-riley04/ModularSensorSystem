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
{}

void MainWindow::initWidgets()
{
	// Set default page to home
	ui.stackedWidget->setCurrentIndex(0);

	// Init device list widget
	ui.deviceListWidget->setDeviceController(pController->deviceController());

	// Init sensor controller widget
	//ui.sensorControllerWidget->setSensorController(pController->sensorController());

	// Init group controls
	/*ui.groupControls->setSensorController(pController->sensorController());
	ui.groupControls->setController(pController.get());*/

	// Set the camera controls
	ui.cameraControls->setCamera(nullptr); // No camera selected by default
}

void MainWindow::initSignals() {
    // Pages
	connect(ui.buttonHome, &QPushButton::clicked, [this]() {
		ui.stackedWidget->setCurrentIndex(0);
		});
    connect(ui.buttonPlayback, &QPushButton::clicked, [this]() {
        ui.stackedWidget->setCurrentIndex(1);
        });

	connect(pController->deviceController(), &DeviceController::deviceAdded, [this](Device* device) {
		// Check if the device is a camera
		if (device->deviceType() == Device::Type::CAMERA) {
			// Add the camera to the preview widget
			ui.devicePreviewWidget->addVideoWidget(static_cast<CameraDevice*>(device));

			// Set the camera controls
			ui.cameraControls->setCamera(static_cast<CameraDevice*>(device));

			// Set the clipping controls
			ui.clippingControls->setVideoBuffer(static_cast<CameraDevice*>(device)->videoBuffer());
		}
		});

    // Sensor view widget
	//connect(ui.sensorControllerWidget, &SensorControllerWidget::controlsChanged, ui.frameControls, &QFrame::setVisible);
	//connect(ui.sensorControllerWidget, &SensorControllerWidget::cameraChanged, ui.cameraControls, &CameraControls::setCamera);
 //   connect(ui.sensorControllerWidget, &SensorControllerWidget::videoWidgetAdded, [this](Camera* camera) {
 //       // Set clipping controls video buffer
 //       ui.clippingControls->setVideoBuffer(camera->videoBuffer());
 //       });

    // Playback button
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

void MainWindow::quit() {
    qApp->quit();
}

void MainWindow::restart() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}