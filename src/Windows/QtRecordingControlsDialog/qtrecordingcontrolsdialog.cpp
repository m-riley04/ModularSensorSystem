#include "qtrecordingcontrolsdialog.h"

QtRecordingControlsDialog::QtRecordingControlsDialog(QWidget *parent, QMediaRecorder* recorder, QCameraDevice device)
	: QDialog(parent), pRecorder(recorder), mDevice(device)
{
	ui.setupUi(this);

	initWidgets();
	initSignals();
}

QtRecordingControlsDialog::~QtRecordingControlsDialog()
{}

void QtRecordingControlsDialog::initWidgets()
{
	if (mDevice.isNull()) {
		ui.labelDevice ->setText("No Camera Selected");
		return;
	}

	if (!pRecorder) {
		// Set default text
		ui.inputSaveDirectory->setText("");

		// Disable widgets
		ui.inputSaveDirectory->setEnabled(false);
		return;
	}

	// Init device
	ui.labelDevice->setText(mDevice.description());

	/// GENERAL TAB
	ui.inputSaveDirectory->setText(pRecorder->outputLocation().toString());

	/// VIDEO TAB
	ui.dropdownQuality->setCurrentIndex(pRecorder->quality());
	ui.dropdownEncodingMode->setCurrentIndex(pRecorder->encodingMode());
	ui.spinboxBitrate->setValue(pRecorder->videoBitRate());
	ui.spinboxFrameRate->setValue(pRecorder->videoFrameRate());
	ui.spinboxWidth->setValue(pRecorder->videoResolution().width());
	ui.spinboxHeight->setValue(pRecorder->videoResolution().height());

	/// TODO: AUDIO TAB
}

void QtRecordingControlsDialog::initSignals()
{

	/// GENERAL TAB
	connect(ui.buttonOpenOutput, &QPushButton::clicked, [this]() {
		// Open the output directory
		QDesktopServices::openUrl(QUrl::fromLocalFile(pRecorder->outputLocation().path()));
		});
	connect(ui.buttonSetOutput, &QPushButton::clicked, [this]() {
		// Set the output directory
		QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory", pRecorder->outputLocation().path());
		if (dir.isEmpty()) return;

		// Set the output directory
		pRecorder->setOutputLocation(QUrl::fromLocalFile(dir));
		ui.inputSaveDirectory->setText(dir);
		});

	/// VIDEO TAB
	connect(ui.dropdownQuality, &QComboBox::currentIndexChanged, [this](int index) {
		pRecorder->setQuality(static_cast<QMediaRecorder::Quality>(index));
		});
}
