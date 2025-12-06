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
	if (!m_controller) {
		LoggingController::warning("PresetsWidget: pController is null; cannot initialize widgets");
		return;
	}

	// Turn off controls frame visibility by default
	ui.frameControls->setVisible(mControlsVisible);

	// Clear the list
	ui.listPresets->clear();

	// Load all presets
	PresetsController& m_presetsController = m_controller->presetsController();
	m_presetsController.scanForPresets();

	// Add presets to the list
	repopulateList();
}

void PresetsWidget::initSignals()
{
	connect(ui.listPresets, &QListWidget::currentItemChanged, this, &PresetsWidget::selectedPresetChanged);

	if (!m_controller) {
		LoggingController::warning("PresetsWidget: pController is null; cannot initialize signals");
		return;
	}

	PresetsController& m_presetsController = m_controller->presetsController();

	connect(ui.buttonRemove, &QPushButton::clicked, this, &PresetsWidget::onRemoveClicked);
	connect(ui.buttonSave, &QPushButton::clicked, this, &PresetsWidget::onSaveClicked);
	connect(ui.buttonLoad, &QPushButton::clicked, this, &PresetsWidget::onLoadClicked);

	connect(ui.listPresets, &QListWidget::itemClicked, this, &PresetsWidget::onSelected);

	connect(&m_presetsController, &PresetsController::presetSaved, this, &PresetsWidget::repopulateList);
	connect(&m_presetsController, &PresetsController::presetRemoved, this, &PresetsWidget::repopulateList);
	connect(&m_presetsController, &PresetsController::presetsScanned, this, &PresetsWidget::repopulateList);
}

void PresetsWidget::repopulateList()
{
	// Get and check presets controller
	PresetsController& m_presetsController = m_controller->presetsController();

	// Clear list
	ui.listPresets->clear();

	// Iterate through presets
	for (const auto& preset : m_presetsController.presets()) {
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