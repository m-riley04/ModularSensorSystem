#include "sessionpropertiesdialog.h"

SessionPropertiesDialog::SessionPropertiesDialog(MainController* controller, SessionProperties* currentData, QWidget *parent)
	: QDialog(parent), m_currentData(currentData), m_newData(*currentData), m_controller(controller)
{
	ui.setupUi(this);
	updateUIFromData();

	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &SessionPropertiesDialog::transmit);

	// Connect value changes to update m_newData
	connect(ui.checkboxEnableClipping, &QCheckBox::toggled, this, [this](bool checked) {
		m_newData.clippingProperties.enabled = checked;
		});
	// TODO: Add rest of clipping signals
	connect(ui.dirPickerRecording, &QDirectoryPickerWidget::directoryChanged, this, [this](const QDir& newDir) {
		m_newData.recordingProperties.outputDirectory = newDir;
		});
	connect(ui.lineRecordingPrefix, &QLineEdit::textChanged, this, [this](const QString& newText) {
		m_newData.recordingProperties.outputPrefix = newText;
		});
	connect(ui.checkboxEnableProcessing, &QCheckBox::toggled, this, [this](bool checked) {
		m_newData.processingProperties.enabled = checked;
		});

}

SessionPropertiesDialog::~SessionPropertiesDialog()
{}

void SessionPropertiesDialog::updateUIFromData() {

	/// CLIPPING TAB
	ui.checkboxEnableClipping->setChecked(m_newData.clippingProperties.enabled);

	/// RECORDING TAB
	ui.dirPickerRecording->setSelectedDirectory(m_newData.recordingProperties.outputDirectory);
	ui.lineRecordingPrefix->setText(m_newData.recordingProperties.outputPrefix);

	/// PROCESSING TAB
	ui.checkboxEnableProcessing->setChecked(m_newData.processingProperties.enabled);
}

void SessionPropertiesDialog::transmit(QAbstractButton* button) {

	auto role = ui.buttonBox->buttonRole(button);
	
	switch (role) {
	case (QDialogButtonBox::ResetRole):
		m_newData = *m_currentData; // Reset new data to current data
		updateUIFromData();
		break;
	case (QDialogButtonBox::ApplyRole):
	case (QDialogButtonBox::RejectRole):
	default:
		emit settingsChanged(m_newData);
		this->close();
		break;
	}
}