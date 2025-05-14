#include "devicelistwidget.h"

DeviceListWidget::DeviceListWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initSignals();
}

DeviceListWidget::DeviceListWidget(DeviceController* deviceController, QWidget* parent)
	: QWidget(parent), pDeviceController(deviceController)
{
	ui.setupUi(this);
	initSignals();
}

DeviceListWidget::~DeviceListWidget()
{}

void DeviceListWidget::initSignals()
{
	// UI button signals
	connect(ui.buttonAdd, &QPushButton::clicked, this, &DeviceListWidget::openAddDeviceDialog);
	//connect(ui.buttonRemove, &QPushButton::clicked, this, &DeviceListWidget::openRemoveDeviceDialog);

	initDeviceControllerSignals();
}

void DeviceListWidget::initDeviceControllerSignals()
{
	if (pDeviceController == nullptr) return;

	// Disconnect previous signals
	disconnect(pDeviceController, &DeviceController::deviceAdded, this, nullptr);

	// Connect device controller signals
	connect(pDeviceController, &DeviceController::deviceAdded, this, [this](Device* device) {
		if (device == nullptr) return;

		// Add the device to the list
		QString listItemName = device->id() + " (" + Device::typeToString(device->deviceType()) + ")";
		ui.listDevices->addItem(listItemName);
		});
}

void DeviceListWidget::setDeviceController(DeviceController* deviceController)
{
	if (deviceController == nullptr) return;
	pDeviceController = deviceController;

	// Connect signals
	initDeviceControllerSignals();

	emit deviceControllerChanged(deviceController);
}

void DeviceListWidget::openAddDeviceDialog()
{
	AddCameraDialog* addCameraDialog = new AddCameraDialog(this);

	connect(addCameraDialog, &AddCameraDialog::deviceSelected, pDeviceController, &DeviceController::addCamera);

	addCameraDialog->setAttribute(Qt::WA_DeleteOnClose);
	addCameraDialog->setWindowModality(Qt::WindowModal);
	addCameraDialog->show();
}
