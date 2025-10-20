#include "addmountdialog.h"

AddMountDialog::AddMountDialog(PluginController* pluginController, QWidget *parent)
	: QDialog(parent), m_pluginController(pluginController)
{
	ui.setupUi(this);

	populateMountTypeDropdown();
	populateMountDropdown();

	// Connect signals
	connect(ui.dropdownMountType, &QComboBox::currentIndexChanged, this, [this](int index) {
		if (index >= 0) {

			QVariant data = ui.dropdownMountType->itemData(index);
			IMountPlugin* plugin = data.value<IMountPlugin*>();
			m_selectedMountPlugin = m_pluginController->mountPlugins().at(index);
			emit mountTypeSelected(plugin);

			// Repopulate with available sources of the selected type
			populateMountDropdown();
		}
		});

	connect(ui.dropdownMount, &QComboBox::currentIndexChanged, this, [this](int index) {
		if (index >= 0) {
			MountInfo mountInfo = ui.dropdownMount->itemData(index).value<MountInfo>();
			m_selectedMountInfo = mountInfo;
			emit mountSelected(m_selectedMountPlugin, m_selectedMountInfo);
		}
		});

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, [this]() {
		// Get the selected mount
		int mountIndex = ui.dropdownMount->currentIndex();
		if (mountIndex >= 0) {
			MountInfo mountInfo = ui.dropdownMount->itemData(mountIndex).value<MountInfo>();
			emit mountConfirmed(m_selectedMountPlugin, mountInfo);
		}
		else {
			// Handle the case where no mount is selected
			QMessageBox::warning(this, "No Mount Selected", "Please select a mount before confirming.");
			return;
		}

		this->accept(); // TODO: Maybe do more here?
		});

	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, [this]() {
		// Close the dialog without doing anything
		this->reject();
		});
}

void AddMountDialog::populateMountTypeDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownMountType->clear();

	// Populate the mount type dropdown with available mount types
	for (const auto& plugin : m_pluginController->mountPlugins()) {
		if (plugin) {
			ui.dropdownMountType->addItem(QString::fromStdString(plugin->name()), QVariant::fromValue(plugin));
		}
	}

	// Set the default selection to the first item
	if (ui.dropdownMountType->count() > 0) {
		ui.dropdownMountType->setCurrentIndex(0);
		m_selectedMountPlugin = m_pluginController->mountPlugins().first();
	}
}

void AddMountDialog::populateMountDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownMount->clear();

	// Check if a mount plugin is selected
	if (!m_selectedMountPlugin) return;

	// Get the selected mount type from the dropdown
	Mount::Kind selectedType = static_cast<Mount::Kind>(ui.dropdownMountType->currentData().toInt());

	// Populate the mount dropdown with available mounts of the selected type
	for (auto mount : m_selectedMountPlugin->discover()) {
		// Add the mount to the dropdown
		ui.dropdownMount->addItem(QString::fromStdString(mount->name()), QVariant::fromValue(mount));
	}

	// Set the default selection to the first item
	if (ui.dropdownMount->count() > 0) {
		ui.dropdownMount->setCurrentIndex(0);
	}
}

