#include "addprocessordialog.h"

AddProcessorDialog::AddProcessorDialog(PluginController* pluginController, QWidget *parent)
	: QDialog(parent), pPluginController(pluginController)
{
	ui.setupUi(this);

	populateProcessorsDropdown();

	// Connect signals
	connect(ui.dropdownProcessor, &QComboBox::currentIndexChanged, this, &AddProcessorDialog::onProcessorSelected);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &AddProcessorDialog::onConfirmButtonClicked);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &AddProcessorDialog::onCancelButtonClicked);
}

AddProcessorDialog::~AddProcessorDialog()
{}

void AddProcessorDialog::onProcessorSelected(int index) {
	if (index < 0 || index >= ui.dropdownProcessor->count()) {
		return; // Invalid index, do nothing
	}

	ProcessorInfo processorInfo = ui.dropdownProcessor->itemData(index).value<ProcessorInfo>();
	mSelectedProcessor = processorInfo;
	emit processorSelected(pSelectedProcessorPlugin);
}

void AddProcessorDialog::populateProcessorsDropdown()
{
	// Clear the existing items in the dropdown
	ui.dropdownProcessor->clear();

	// Populate the source type dropdown with available source types
	for (const auto& plugin : pPluginController->processorPlugins()) {
		if (plugin) {
			ui.dropdownProcessor->addItem(QString::fromStdString(plugin->name()), QVariant::fromValue(plugin));
		}
	}

	// Check if there are any processors available
	if (pPluginController->processorPlugins().count() <= 0) { // This is a more robust check. Sorta redunant, but still good to have.
		qDebug() << "No processor plugins available in controller";
		return;
	}
	if (ui.dropdownProcessor->count() <= 0) {
		qDebug() << "No processor plugins available in dropdown";
		return;
	}

	// Set the first processor as selected by default
	ui.dropdownProcessor->setCurrentIndex(0);
	pSelectedProcessorPlugin = pPluginController->processorPlugins().first();
}

void AddProcessorDialog::onConfirmButtonClicked() {
	// Get the selected processor plugin and info
	emit processorConfirmed(pSelectedProcessorPlugin);
	this->accept(); // TODO: Maybe do more here?
}

void AddProcessorDialog::onCancelButtonClicked()
{
	this->reject(); // Close the dialog without doing anything
}
