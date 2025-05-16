#include "mainpage.h"

MainPage::MainPage(MainController* controller, QWidget *parent)
	: QWidget(parent), pController(controller)
{
	ui.setupUi(this);

	initWidgets();
	initSignals();
}

MainPage::~MainPage()
{}

void MainPage::initWidgets()
{
	// Init device list widget
	ui.deviceListWidget->setDeviceController(pController->deviceController());

	// Init session controls widget
	ui.sessionControls->setController(pController);
}

void MainPage::initSignals() {
	DeviceController* pDeviceController = pController->deviceController();

	// Device added ui updates
	connect(pDeviceController, &DeviceController::deviceAdded, [this](Device* device) {
		// Check if the device is a camera
		if (device->deviceType() == Device::Type::CAMERA) {
			// Add the camera to the preview widget
			ui.devicePreviewWidget->addVideoWidget(static_cast<CameraDevice*>(device));
		}
		});

}