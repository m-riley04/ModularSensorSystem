#include "devicelistwidget.h"
#include <QMimeData>

DeviceListWidget::DeviceListWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initSignals();
}

DeviceListWidget::DeviceListWidget(SourceController* sourceController, QWidget* parent)
	: QWidget(parent), pDeviceController(sourceController)
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
	connect(ui.buttonRemove, &QPushButton::clicked, this, &DeviceListWidget::openRemoveDeviceDialog);
	connect(ui.buttonProperties, &QPushButton::clicked, this, &DeviceListWidget::openProperties);

	initDeviceControllerSignals();
}

void DeviceListWidget::initDeviceControllerSignals()
{
	if (pDeviceController == nullptr) return;

	// Disconnect previous signals
	disconnect(pDeviceController, &SourceController::sourceAdded, this, nullptr);
	disconnect(pDeviceController, &SourceController::sourceRemoved, this, nullptr);

	// Connect device controller signals
	connect(pDeviceController, &SourceController::sourceAdded, this, [this](Device* device) {
		if (device == nullptr) return;

		// Add the device to the list
		QString listItemName = device->name() + " (" + Device::typeToString(device->sourceType()) + ")";
		QVariant devicePtr = QVariant::fromValue(device);
		QListWidgetItem* item = new QListWidgetItem(listItemName, ui.listDevices);
		item->setData(Qt::UserRole, devicePtr);
		ui.listDevices->addItem(item);
		});
	connect(pDeviceController, &SourceController::sourceRemoved, this, [this](Device* device) {
		if (device == nullptr) return;

		// Find device in list widget
		for (int i = 0; i < ui.listDevices->count(); ++i) {
			QListWidgetItem* itm = ui.listDevices->item(i);
			Device* stored = itm->data(Qt::UserRole).value<Device*>();
			if (stored == device) {
				delete ui.listDevices->takeItem(i); // returns the item; we delete it
				// break; // CONSIDER: don't break so that it continues to remove that pointer (if in multiple times)
			}
		}
		});
}

void DeviceListWidget::openRemoveDeviceDialog()
{
	auto response = QMessageBox::question(this, "Remove Device", "Are you sure you want to remove the selected device?", QMessageBox::Yes | QMessageBox::No);

	if (response == QMessageBox::Yes) {
		auto selectedItem = ui.listDevices->currentItem();
		if (selectedItem) {
			Device* device = selectedItem->data(Qt::UserRole).value<Device*>();

			// Remove device from the controller
			pDeviceController->removeSource(device);
		}
	}
}

void DeviceListWidget::openProperties()
{
	// Get the selected device
	auto selectedItem = ui.listDevices->currentItem();

	if (!selectedItem) return;

	Device* device = selectedItem->data(Qt::UserRole).value<Device*>();

	if (auto cfg = qobject_cast<IConfigurableDevice*>(device)) {
		QWidget* w = cfg->createConfigWidget(this);
		QDialog dlg(this);
		dlg.setWindowTitle(device->name() + " Properties");
		QVBoxLayout lay(&dlg);
		lay.addWidget(w);
		dlg.exec();
	}
	else {
		// fallback: show generic property inspector
		//showGenericPropertyDialog(device);
		QMessageBox::information(this, "Properties", "No properties available for this device.");
	}
}

void DeviceListWidget::setDeviceController(SourceController* sourceController)
{
	if (sourceController == nullptr) return;
	pDeviceController = sourceController;

	// Connect signals
	initDeviceControllerSignals();

	emit deviceControllerChanged(sourceController);
}

void DeviceListWidget::openAddDeviceDialog()
{
	AddSourceDialog* addDeviceDialog = new AddSourceDialog(pDeviceController->pluginController(), this);
	addDeviceDialog->setWindowModality(Qt::WindowModal);

	connect(addDeviceDialog, &AddSourceDialog::sourceConfirmed, pDeviceController, &SourceController::addSource);

	addDeviceDialog->show();
}
