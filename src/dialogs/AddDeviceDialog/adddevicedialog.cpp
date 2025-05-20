#include "adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(PluginController* pluginController, QWidget *parent)
	: QDialog(parent), pPluginController(pluginController)
{
	ui.setupUi(this);
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
}

void AddDeviceDialog::populateDeviceDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownDevice->clear();

	// Get the selected device type from the dropdown
	Device::Type selectedType = static_cast<Device::Type>(ui.dropdownDeviceType->currentData().toInt());

	// Populate the device dropdown with available devices of the selected type
	
}
