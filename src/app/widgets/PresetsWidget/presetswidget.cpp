#include "presetswidget.h"


PresetsWidget::PresetsWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initWidgets();
	initSignals();
}

PresetsWidget::~PresetsWidget()
{}

void PresetsWidget::initWidgets()
{
	if (!pController) {
		qWarning() << "PresetsWidget: pController is null; cannot initialize widgets";
		return;
	}

	// Turn off controls frame visibility by default
	ui.frameControls->setVisible(mControlsVisible);

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
	connect(ui.listPresets, &QListWidget::currentItemChanged, this, &PresetsWidget::selectedPresetChanged);

	if (!pController) {
		qWarning() << "PresetsWidget: pController is null; cannot initialize signals";
		return;
	}

	PresetsController* pPresetsController = pController->presetsController();

	connect(ui.buttonRemove, &QPushButton::clicked, this, &PresetsWidget::onRemoveClicked);
	connect(ui.buttonSave, &QPushButton::clicked, this, &PresetsWidget::onSaveClicked);
	connect(ui.buttonLoad, &QPushButton::clicked, this, &PresetsWidget::onLoadClicked);

	connect(ui.listPresets, &QListWidget::itemClicked, this, &PresetsWidget::onSelected);

	connect(pPresetsController, &PresetsController::presetSaved, this, &PresetsWidget::repopulateList);
	connect(pPresetsController, &PresetsController::presetRemoved, this, &PresetsWidget::repopulateList);
	connect(pPresetsController, &PresetsController::presetsScanned, this, &PresetsWidget::repopulateList);
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

void PresetsWidget::onRemoveClicked()
{
	
}

void PresetsWidget::onSaveClicked()
{
	
}

void PresetsWidget::onLoadClicked()
{
	
}

void PresetsWidget::onSelected(QListWidgetItem* item)
{
	if (pSelectedItem == item) {
		return;
	}
	pSelectedItem = item;

	if (!item) {
		ui.buttonLoad->setEnabled(false);
		ui.buttonRemove->setEnabled(false);
		return;
	}

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

}