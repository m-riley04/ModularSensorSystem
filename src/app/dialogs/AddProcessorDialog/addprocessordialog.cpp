#include "addprocessordialog.h"
#include <controllers/loggingcontroller.hpp>

AddProcessorDialog::AddProcessorDialog(PluginController& pc, ElementsController& ec, QWidget *parent)
	: QDialog(parent)
	, m_pluginController(pc), m_elementsController(ec)
{
	ui.setupUi(this);

	populateProcessorsDropdown();

	// Connect signals
	connect(ui.dropdownProcessor, &QComboBox::currentIndexChanged, this, &AddProcessorDialog::onProcessorSelected);
	connect(ui.dropdownSources, &QComboBox::currentIndexChanged, this, &AddProcessorDialog::onSourceSelected);
	connect(ui.buttonRefresh, &QPushButton::clicked, this, &AddProcessorDialog::onRefreshProcessorsButtonClicked);
	//connect(ui.buttonRefreshSources, &QPushButton::clicked, this, &AddProcessorDialog::onRefreshSourcesButtonClicked);
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
	for (const auto* plugin : m_pluginController.processorPlugins()) {
		if (!plugin) continue;
		ui.dropdownProcessor->addItem(QString::fromStdString(plugin->name()), QVariant::fromValue(plugin));
	}

	if (ui.dropdownProcessor->count() <= 0) {
		LoggingController::warning("Dropdown processor count is zero after population");
		return;
	}

	// Set the first processor as selected by default
	ui.dropdownProcessor->setCurrentIndex(0);
	pSelectedProcessorPlugin = m_pluginController.processorPlugins().first();
}

void AddProcessorDialog::populateSourcesDropdown()
{
	// Get the selected processor plugin
	auto& sources = m_elementsController.sourceController().sources();

	// Clear the existing items in the dropdown
	ui.dropdownSources->clear();

	// Populate the source type dropdown with available source types
	// TODO: filter sources based on compatibility with selected processor
	for (auto* src : sources) {
		if (!src) continue;
		ui.dropdownProcessor->addItem(QString::fromStdString(src->displayName()), QVariant::fromValue(src));
	}

	// Check if there are any sources available
	if (ui.dropdownSources->count() <= 0) {
		LoggingController::warning("Dropdown sources count is zero after population");
		return;
	}

	// Set the first source as selected by default
	ui.dropdownSources->setCurrentIndex(0);
	pSelectedProcessorPlugin = m_pluginController.processorPlugins().first();
}

void AddProcessorDialog::onSourceSelected(int index)
{
	if (index < 0 || index >= ui.dropdownSources->count()) {
		return; // Invalid index, do nothing
	}

	Source* source = ui.dropdownSources->itemData(index).value<Source*>();
	m_selectedSource = source;
	emit sourceSelected(m_selectedSource);
}

void AddProcessorDialog::onConfirmButtonClicked() {
	// TODO: check selections or lack thereof

	if (m_selectedSource == nullptr) {
		QMessageBox::warning(this, "No Source Selected", "Please select a source to attach processor to before confirming.");
		return;
	}

	// Get the selected processor plugin and info
	emit processorConfirmed(pSelectedProcessorPlugin);
	this->accept(); // TODO: Maybe do more here?
}

void AddProcessorDialog::onCancelButtonClicked()
{
	this->reject(); // Close the dialog without doing anything
}

void AddProcessorDialog::onRefreshProcessorsButtonClicked()
{
	// TODO: implement refreshing processor plugins list
}

void AddProcessorDialog::onRefreshSourcesButtonClicked()
{
	// TODO: implement refreshing sources list
}
