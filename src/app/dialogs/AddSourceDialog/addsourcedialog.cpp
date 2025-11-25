#include "addsourcedialog.h"

AddSourceDialog::AddSourceDialog(PluginController* pluginController, QWidget *parent)
	: QDialog(parent), pPluginController(pluginController)
{
	ui.setupUi(this);

	populateSourceTypeDropdown();
	populateSourceDropdown();

	// Connect signals
	connect(ui.dropdownSourceType, &QComboBox::currentIndexChanged, this, [this](int index) {
		if (index >= 0) {
			
			QVariant data = ui.dropdownSourceType->itemData(index);
			ISourcePlugin* plugin = data.value<ISourcePlugin*>();
			pSelectedSourcePlugin = pPluginController->sourcePlugins().at(index);;
			emit sourceTypeSelected(plugin);

			// Repopulate with available sources of the selected type
			populateSourceDropdown();
		}
		});

	connect(ui.dropdownSource, &QComboBox::currentIndexChanged, this, [this](int index) {
		if (index >= 0) {
			SourceInfo sourceInfo = ui.dropdownSource->itemData(index).value<SourceInfo>();
			mSelectedSourceInfo = sourceInfo;
			emit sourceSelected(pSelectedSourcePlugin, mSelectedSourceInfo);
		}
		});

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, [this]() {
		// Get the selected source
		int sourceIndex = ui.dropdownSource->currentIndex();
		if (sourceIndex >= 0) {
			SourceInfo sourceInfo = ui.dropdownSource->itemData(sourceIndex).value<SourceInfo>();
			emit sourceConfirmed(pSelectedSourcePlugin, sourceInfo);
		}
		else {
			// Handle the case where no source is selected
			QMessageBox::warning(this, "No Source Selected", "Please select a source before confirming.");
			return;
		}

		this->accept(); // TODO: Maybe do more here?
		});

	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, [this]() {
		// Close the dialog without doing anything
		this->reject();
		});
}

AddSourceDialog::~AddSourceDialog()
{}

void AddSourceDialog::populateSourceTypeDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownSourceType->clear();

	// Populate the source type dropdown with available source types
	for (const auto& plugin : pPluginController->sourcePlugins()) {
		if (plugin) {
			ui.dropdownSourceType->addItem(QString::fromStdString(plugin->name()), QVariant::fromValue(plugin));
		}
	}

	// Set the default selection to the first item
	if (ui.dropdownSourceType->count() > 0) {
		ui.dropdownSourceType->setCurrentIndex(0);
		pSelectedSourcePlugin = pPluginController->sourcePlugins().first();
	}
}

void AddSourceDialog::populateSourceDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownSource->clear();

	// Check if a source plugin is selected
	if (!pSelectedSourcePlugin) return;

	// Get the selected source type from the dropdown
	Source::Type selectedType = static_cast<Source::Type>(ui.dropdownSourceType->currentData().toInt());

	// Populate the source dropdown with available sources of the selected type
	for (auto source : pSelectedSourcePlugin->discover()) {
		// Add the source to the dropdown
		ui.dropdownSource->addItem(QString::fromStdString(source.elementInfo.displayName), QVariant::fromValue(source));
	}

	// Set the default selection to the first item
	if (ui.dropdownSource->count() > 0) {
		ui.dropdownSource->setCurrentIndex(0);
	}
}
