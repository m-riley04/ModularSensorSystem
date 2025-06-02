#include "presetswidget.h"


PresetsWidget::PresetsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PresetsWidget::~PresetsWidget()
{}

void PresetsWidget::onRemoveClicked()
{
	if (!pSelectedItem) return;

	// Get the preset name from the item
	QString presetName = pSelectedItem->text();

	// Confirm removal
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("Remove Preset"),
		tr("Are you sure you want to remove the preset: %1?").arg(presetName),
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No) return;

	// Find the preset in the controller
	PresetsController* pPresetsController = pController->presetsController();
	auto presets = pPresetsController->presets();
	auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
		return preset.name == presetName;
		});
	if (it != presets.end()) {
		// Remove the preset
		pPresetsController->removePreset(it->path);
	}
	ui.listPresets->removeItemWidget(pSelectedItem);
	delete pSelectedItem;
	pSelectedItem = nullptr;

	// Update UI
	ui.buttonLoad->setEnabled(false);
	ui.buttonRemove->setEnabled(false);
}

void PresetsWidget::onSaveClicked()
{
	// Get the preset name from the user
	bool ok;
	QString presetName = QInputDialog::getText(this, tr("Save Preset"),
		tr("Preset name:"), QLineEdit::Normal,
		QString(), &ok);
	if (ok && !presetName.isEmpty()) {
		// Check if the preset name already exists
		// If so, ask to overwrite
		auto existingItems = ui.listPresets->findItems(presetName, Qt::MatchExactly);
		if (!existingItems.isEmpty()) {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr("Overwrite Preset"),
				tr("Preset already exists. Do you want to overwrite it?"),
				QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::No) {
				return;
			}
		}

		PresetsController* pPresetsController = pController->presetsController();
		pPresetsController->savePreset(presetName, pController->sourceController()->sources());
	}
	else if (ok) {
		QMessageBox::warning(this, tr("Invalid Name"), tr("Preset name cannot be empty."));
		return;
	}
}

void PresetsWidget::onLoadClicked()
{
	if (!pSelectedItem) return;

	// Get the preset name from the item
	QString presetName = pSelectedItem->text();

	// Find the preset in the controller
	PresetsController* pPresetsController = pController->presetsController();
	auto presets = pPresetsController->presets();
	auto it = std::find_if(presets.begin(), presets.end(), [&presetName](const Preset& preset) {
		return preset.name == presetName;
		});
	if (it != presets.end()) {
		// Load the preset
		pPresetsController->loadPreset(it->path, pController->sourceController(), pController->pluginController());
	}
}

void PresetsWidget::onSelected(QListWidgetItem* item)
{
	if (pSelectedItem == item) {
		return;
	}
	pSelectedItem = item;

	item->setSelected(true);

	// Update ui/buttons
	ui.buttonLoad->setEnabled(true);
	ui.buttonRemove->setEnabled(true);
}

void PresetsWidget::onDoubleClicked(QListWidgetItem* item)
{

}

void PresetsWidget::onRefreshClicked()
{
	PresetsController* pPresetsController = pController->presetsController();
	if (!pPresetsController) {
		qWarning() << "Cannot refresh: presets controller is null";
	}

	// Scan
	pPresetsController->scanForPresets(); // CONSIDER: Pass specific path?

	// Repopulate
	repopulateList();

}

void PresetsWidget::initWidgets()
{
	// Clear the list
	ui.listPresets->clear();

	// Load all presets
	PresetsController* pPresetsController = pController->presetsController();
	if (!pPresetsController) {
		qWarning() << "PresetsController is null; aborting presets scan";
		return;
	}
	pPresetsController->scanForPresets();

	// Add presets to the list
	repopulateList();
}

void PresetsWidget::initSignals()
{
	PresetsController* pPresetsController = pController->presetsController();

	connect(ui.buttonRemove, &QPushButton::clicked, this, &PresetsWidget::onRemoveClicked);
	connect(ui.buttonSave, &QPushButton::clicked, this, &PresetsWidget::onSaveClicked);
	connect(ui.buttonLoad, &QPushButton::clicked, this, &PresetsWidget::onLoadClicked);

	connect(ui.listPresets, &QListWidget::itemClicked, this, &PresetsWidget::onSelected);

	connect(pPresetsController, &PresetsController::presetSaved, this, &PresetsWidget::repopulateList);
	connect(pPresetsController, &PresetsController::presetRemoved, this, &PresetsWidget::repopulateList);
}

void PresetsWidget::repopulateList()
{
	// Get and check presets controller
	PresetsController* pPresetsController = pController->presetsController();
	if (!pPresetsController) {
		qWarning() << "PresetsController is null; aborting presets population";
		return;
	}

	// Clear list
	ui.listPresets->clear();

	// Iterate through presets
	for (const auto& preset : pPresetsController->presets()) {
		QListWidgetItem* item = new QListWidgetItem(preset.name);
		item->setData(Qt::UserRole, preset.path);
		ui.listPresets->addItem(item);
	}
	ui.listPresets->setCurrentRow(0);
	pSelectedItem = ui.listPresets->currentItem();
}
