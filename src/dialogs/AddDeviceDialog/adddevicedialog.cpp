#include "adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(PluginController* pluginController, QWidget *parent)
	: QDialog(parent), pPluginController(pluginController)
{
	ui.setupUi(this);

	populateDeviceTypeDropdown();
	populateDeviceDropdown();

	// Connect signals
	connect(ui.dropdownDeviceType, &QComboBox::currentIndexChanged, this, [this](int index) {
		if (index >= 0) {
			QVariant data = ui.dropdownDeviceType->itemData(index);
			IDevicePlugin* plugin = data.value<IDevicePlugin*>();
			pSelectedDevicePlugin = plugin;
			emit deviceTypeSelected(plugin);

			// Repopulate with available devices of the selected type
			populateDeviceDropdown();
		}
		});

	connect(ui.dropdownDevice, &QComboBox::currentIndexChanged, this, [this](int index) {
		if (index >= 0) {
			DeviceInfo deviceInfo = ui.dropdownDevice->itemData(index).value<DeviceInfo>();
			mSelectedDeviceInfo = deviceInfo;
			emit deviceSelected(pSelectedDevicePlugin, mSelectedDeviceInfo);
            
		}
		});

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, [this]() {
		// Get the selected device
		int deviceIndex = ui.dropdownDevice->currentIndex();
		if (deviceIndex >= 0) {
			DeviceInfo deviceInfo = ui.dropdownDevice->itemData(deviceIndex).value<DeviceInfo>();
			emit deviceConfirmed(pSelectedDevicePlugin, deviceInfo);
		}
		else {
			// Handle the case where no device is selected
			QMessageBox::warning(this, "No Device Selected", "Please select a device before confirming.");
			return;
		}

		this->accept(); // TODO: Maybe do more here?
		});

	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, [this]() {
		// Close the dialog without doing anything
		this->reject();
		});
}

AddDeviceDialog::~AddDeviceDialog()
{}

void AddDeviceDialog::populateDeviceTypeDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownDeviceType->clear();

	// Set to store unique device types
	QSet<Device::Type> uniqueDeviceTypes;

	// Populate the device type dropdown with available device types
	for (const auto& plugin : pPluginController->devicePlugins()) {
		if (plugin) {
			uniqueDeviceTypes.insert(plugin->deviceType());
		}
	}
	
	// Add the unique device types to the dropdown
	for (const auto& type : uniqueDeviceTypes) {
		ui.dropdownDeviceType->addItem(Device::typeToString(type), static_cast<int>(type));
	}

	// Set the default selection to the first item
	if (ui.dropdownDeviceType->count() > 0) {
		ui.dropdownDeviceType->setCurrentIndex(0);
		pSelectedDevicePlugin = pPluginController->devicePlugins().first();
	}
}

void AddDeviceDialog::populateDeviceDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownDevice->clear();

	// Check if a device plugin is selected
	if (!pSelectedDevicePlugin) return;

	// Get the selected device type from the dropdown
	Device::Type selectedType = static_cast<Device::Type>(ui.dropdownDeviceType->currentData().toInt());

	// Populate the device dropdown with available devices of the selected type
	for (auto device : pSelectedDevicePlugin->availableDevices()) {
		// Add the device to the dropdown
		ui.dropdownDevice->addItem(device.displayName, QVariant::fromValue(device));
	}

	// Set the default selection to the first item
	if (ui.dropdownDevice->count() > 0) {
		ui.dropdownDevice->setCurrentIndex(0);
	}
}
