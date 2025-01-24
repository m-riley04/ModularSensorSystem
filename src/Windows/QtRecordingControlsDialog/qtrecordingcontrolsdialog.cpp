#include "qtrecordingcontrolsdialog.h"
#include <QMessageBox>



enum RecordingControlsTab {
	GENERAL,
	VIDEO,
	AUDIO
};

QtRecordingControlsDialog::QtRecordingControlsDialog(QWidget *parent, QMediaRecorder* recorder)
	: QDialog(parent), pRecorder(recorder)
{
	ui.setupUi(this);

	// Overall null check
	if (!pRecorder) { /// CONSIDER: Move this into widget inits?
		QMessageBox::warning(this, "Error", "Cannot open recording controls: no recorder found.");
		this->reject();

		return;
	}

	// Check capture session
	pCaptureSession = pRecorder->captureSession();
	if (!pCaptureSession) { /// CONSIDER: Move this into widget inits?
		QMessageBox::warning(this, "Error", "Cannot open recording controls: no associated capture session found.");
		this->reject();
		return;
	}

	// Initialize input pointers
	pCamera = pCaptureSession->camera();
	pAudioInput = pCaptureSession->audioInput();

	// Input check
	if (!pCamera && !pAudioInput) { /// CONSIDER: Move this into widget inits?
		QMessageBox::warning(this, "Error", "No input device found.");
	}

	initWidgets();
	initSignals();
}

QtRecordingControlsDialog::~QtRecordingControlsDialog()
{}

void QtRecordingControlsDialog::initWidgets()
{
	// Default set all tabs to disabled (except general)
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::GENERAL, true);
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::VIDEO, false);
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::AUDIO, false);

	/// INITIALIZE ALL TABS
	initGeneralWidgets();

	// Check if video tab is needed
	if (pCamera) initVideoWidgets();

	// Check if audio tab is needed
	if (pAudioInput) initAudioWidgets();
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

	connect(ui.dropdownEncodingMode, &QComboBox::currentIndexChanged, [this](int index) {
		pRecorder->setEncodingMode(static_cast<QMediaRecorder::EncodingMode>(index));
		});

	connect(ui.spinboxBitrate, &QSpinBox::editingFinished, [this]() {
		pRecorder->setVideoBitRate(ui.spinboxBitrate->value());
		});

	connect(ui.spinboxFrameRate, &QSpinBox::editingFinished, [this]() {
		pRecorder->setVideoFrameRate(ui.spinboxFrameRate->value());
		});

	connect(ui.spinboxWidth, &QSpinBox::editingFinished, [this]() {
		pRecorder->setVideoResolution(ui.spinboxWidth->value(), ui.spinboxHeight->value());
		});

	connect(ui.spinboxHeight, &QSpinBox::editingFinished, [this]() {
		pRecorder->setVideoResolution(ui.spinboxWidth->value(), ui.spinboxHeight->value());
		});
}

void QtRecordingControlsDialog::initGeneralWidgets()
{
	/// FIELDS
	ui.inputSaveDirectory->setText(pRecorder->outputLocation().toString());
	ui.checkboxAutoStop->setChecked(pRecorder->autoStop());
	ui.dropdownQuality->setCurrentIndex(pRecorder->quality());
	ui.dropdownEncodingMode->setCurrentIndex(pRecorder->encodingMode());
}

void QtRecordingControlsDialog::initVideoWidgets()
{
	/// TAB
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::VIDEO, true);

	/// DEVICE
	ui.labelVideoDevice->setText(pCamera->cameraDevice().description());

	/// FIELDS
	//ui.dropdownVideoCodec->setCurrentIndex();
	ui.spinboxBitrate->setValue(pRecorder->videoBitRate());
	ui.spinboxFrameRate->setValue(pRecorder->videoFrameRate());
	ui.spinboxWidth->setValue(pRecorder->videoResolution().width());
	ui.spinboxHeight->setValue(pRecorder->videoResolution().height());
}

void QtRecordingControlsDialog::initAudioWidgets()
{
	/// TAB
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::AUDIO, true);

	/// DEVICES
	ui.labelAudioDevice->setText(pAudioInput->device().description());

	/// FIELDS
	//ui.dropdownAudioCodec->setCurrentIndex();
	ui.spinboxAudioBitrate->setValue(pRecorder->audioBitRate());
	ui.spinboxSampleRate->setValue(pRecorder->audioSampleRate());
	ui.spinboxChannels->setValue(pRecorder->audioChannelCount());
}

void QtRecordingControlsDialog::initGeneralSignals()
{
}

void QtRecordingControlsDialog::initVideoSignals()
{
}

void QtRecordingControlsDialog::initAudioSignals()
{
}

void QtRecordingControlsDialog::initDefaultValues()
{
	
}


