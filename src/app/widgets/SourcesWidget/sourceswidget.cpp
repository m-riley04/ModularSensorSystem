#include "sourceswidget.h"

SourcesWidget::SourcesWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initSignals();
}

SourcesWidget::SourcesWidget(SourceController* sourceController, QWidget* parent)
	: QWidget(parent), pSourceController(sourceController)
{
	ui.setupUi(this);
	initSignals();
}

SourcesWidget::~SourcesWidget()
{}

void SourcesWidget::initSignals()
{
	// UI button signals
	connect(ui.buttonAdd, &QPushButton::clicked, this, &SourcesWidget::openAddSourceDialog);
	connect(ui.buttonRemove, &QPushButton::clicked, this, &SourcesWidget::openRemoveSourceDialog);
	connect(ui.buttonProperties, &QPushButton::clicked, this, &SourcesWidget::openProperties);

	initDeviceControllerSignals();
}

void SourcesWidget::initDeviceControllerSignals()
{
	if (pSourceController == nullptr) return;

	// Disconnect previous signals
	disconnect(pSourceController, &SourceController::sourceAdded, this, nullptr);
	disconnect(pSourceController, &SourceController::sourceRemoved, this, nullptr);

	// Connect device controller signals
	connect(pSourceController, &SourceController::sourceAdded, this, [this](Device* device) {
		if (device == nullptr) return;

		// Add the device to the list
		QString listItemName = device->name() + " (" + Device::typeToString(device->sourceType()) + ")";
		QVariant devicePtr = QVariant::fromValue(device);
		QListWidgetItem* item = new QListWidgetItem(listItemName, ui.listDevices);
		item->setData(Qt::UserRole, devicePtr);
		ui.listDevices->addItem(item);
		});
	connect(pSourceController, &SourceController::sourceRemoved, this, [this](Device* device) {
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

void SourcesWidget::openRemoveSourceDialog()
{
	auto response = QMessageBox::question(this, "Remove Device", "Are you sure you want to remove the selected device?", QMessageBox::Yes | QMessageBox::No);

	if (response == QMessageBox::Yes) {
		auto selectedItem = ui.listDevices->currentItem();
		if (selectedItem) {
			Device* device = selectedItem->data(Qt::UserRole).value<Device*>();

			// Remove device from the controller
			pSourceController->removeSource(device);
		}
	}
}

void SourcesWidget::openProperties()
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

void SourcesWidget::setSourceController(SourceController* sourceController)
{
	if (sourceController == nullptr) return;
	pSourceController = sourceController;

	// Connect signals
	initDeviceControllerSignals();

	emit sourceControllerChanged(sourceController);
}

void SourcesWidget::openAddSourceDialog()
{
	AddSourceDialog* addDeviceDialog = new AddSourceDialog(pSourceController->pluginController(), this);
	addDeviceDialog->setWindowModality(Qt::WindowModal);

	connect(addDeviceDialog, &AddSourceDialog::sourceConfirmed, pSourceController, &SourceController::addSource);

	addDeviceDialog->show();
}
