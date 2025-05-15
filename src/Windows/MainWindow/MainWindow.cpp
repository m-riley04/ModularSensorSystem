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

	// Init session controls widget
	ui.sessionControls->setController(pController.get());
}

void MainWindow::initSignals() {
	DeviceController* pDeviceController = pController->deviceController();

    // Pages
	connect(ui.buttonHome, &QPushButton::clicked, [this]() {
		ui.stackedWidget->setCurrentIndex(0);
		});
    connect(ui.buttonPlayback, &QPushButton::clicked, [this]() {
        ui.stackedWidget->setCurrentIndex(1);
        });

	// Device added ui updates
	connect(pDeviceController, &DeviceController::deviceAdded, [this](Device* device) {
		// Check if the device is a camera
		if (device->deviceType() == Device::Type::CAMERA) {
			// Add the camera to the preview widget
			ui.devicePreviewWidget->addVideoWidget(static_cast<CameraDevice*>(device));

			// Set the clipping controls
			ui.clippingControls->setVideoBuffer(static_cast<CameraDevice*>(device)->videoBuffer());
		}
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