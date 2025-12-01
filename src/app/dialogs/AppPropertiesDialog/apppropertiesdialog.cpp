#include "apppropertiesdialog.h"

AppPropertiesDialog::AppPropertiesDialog(SettingsController& sc, QWidget *parent)
	: QDialog(parent), m_settingsController(sc)
{
	ui.setupUi(this);

	// Connect tab buttons to their respective slots
	connect(ui.buttonGeneral, &QPushButton::clicked, this, &AppPropertiesDialog::onGeneralTabClicked);
	connect(ui.buttonAppearance, &QPushButton::clicked, this, &AppPropertiesDialog::onAppearanceTabClicked);
	connect(ui.buttonSession, &QPushButton::clicked, this, &AppPropertiesDialog::onSessionTabClicked);
	connect(ui.buttonSources, &QPushButton::clicked, this, &AppPropertiesDialog::onSourcesTabClicked);
	connect(ui.buttonKeybinds, &QPushButton::clicked, this, &AppPropertiesDialog::onKeybindsTabClicked);
	connect(ui.buttonAccessibility, &QPushButton::clicked, this, &AppPropertiesDialog::onAccessibilityTabClicked);
	connect(ui.buttonAdvanced, &QPushButton::clicked, this, &AppPropertiesDialog::onAdvancedTabClicked);
	onGeneralTabClicked(); // Default to general tab

	/// GENERAL TAB SETUP
	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &AppPropertiesDialog::onButtonBoxClicked);
	
}

AppPropertiesDialog::~AppPropertiesDialog()
{}

void AppPropertiesDialog::onButtonBoxClicked(QAbstractButton* button)
{
	if (button == ui.buttonBox->button(QDialogButtonBox::Reset)) onResetClicked();
	else if (button == ui.buttonBox->button(QDialogButtonBox::RestoreDefaults)) onRestoreDefaultsClicked();
	else if (button == ui.buttonBox->button(QDialogButtonBox::Apply)) onApplyClicked();
	else reject();
}

void AppPropertiesDialog::onResetClicked()
{
	// TODO: Reset settings to previous
}

void AppPropertiesDialog::onRestoreDefaultsClicked()
{
	// TODO: Restore default settings
}

void AppPropertiesDialog::onApplyClicked()
{
	// TODO: Apply settings changes
	accept();
}


