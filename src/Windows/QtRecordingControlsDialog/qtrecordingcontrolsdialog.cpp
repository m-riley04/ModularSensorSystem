#include "qtrecordingcontrolsdialog.h"

QList<QMediaFormat::VideoCodec> videoCodecs = {
	QMediaFormat::VideoCodec::VP8,
	QMediaFormat::VideoCodec::MPEG2,
	QMediaFormat::VideoCodec::MPEG1,
	QMediaFormat::VideoCodec::WMV,
	QMediaFormat::VideoCodec::H265,
	QMediaFormat::VideoCodec::H264,
	QMediaFormat::VideoCodec::MPEG4,
	QMediaFormat::VideoCodec::AV1,
	QMediaFormat::VideoCodec::MotionJPEG,
	QMediaFormat::VideoCodec::VP9,
	QMediaFormat::VideoCodec::Theora,
	QMediaFormat::VideoCodec::Unspecified
};

QList<QMediaFormat::AudioCodec> audioCodecs = {
	QMediaFormat::AudioCodec::WMA,
	QMediaFormat::AudioCodec::AC3,
	QMediaFormat::AudioCodec::AAC,
	QMediaFormat::AudioCodec::ALAC,
	QMediaFormat::AudioCodec::DolbyTrueHD,
	QMediaFormat::AudioCodec::EAC3,
	QMediaFormat::AudioCodec::MP3,
	QMediaFormat::AudioCodec::Wave,
	QMediaFormat::AudioCodec::Vorbis,
	QMediaFormat::AudioCodec::FLAC,
	QMediaFormat::AudioCodec::Opus,
	QMediaFormat::AudioCodec::Unspecified
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
	initGeneralSignals();
	initVideoSignals();
	initAudioSignals();
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
	populateEnumDropdown<QMediaFormat::VideoCodec>(ui.dropdownVideoCodec, videoCodecs, QMediaFormat::videoCodecDescription);

	int i = ui.dropdownVideoCodec->findData(QVariant(static_cast<int>(pRecorder->mediaFormat().videoCodec())));
	ui.dropdownVideoCodec->setCurrentIndex(i);
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
	populateEnumDropdown<QMediaFormat::AudioCodec>(ui.dropdownAudioCodec, audioCodecs, QMediaFormat::audioCodecDescription);

	int i = ui.dropdownAudioCodec->findData(QVariant(static_cast<int>(pRecorder->mediaFormat().audioCodec())));
	ui.dropdownAudioCodec->setCurrentIndex(i);
	ui.spinboxAudioBitrate->setValue(pRecorder->audioBitRate());
	ui.spinboxSampleRate->setValue(pRecorder->audioSampleRate());
	ui.spinboxChannels->setValue(pRecorder->audioChannelCount());
}

void QtRecordingControlsDialog::initGeneralSignals()
{
	connect(ui.buttonOpenOutput, &QPushButton::clicked, [this]() {
		// Open the output directory
		QDesktopServices::openUrl(QUrl::fromLocalFile(pRecorder->outputLocation().path()));
		});
	connect(ui.buttonSetOutput, &QPushButton::clicked, [this]() {
		// Select output directory
		QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory", pRecorder->outputLocation().path());
		if (dir.isEmpty()) return;

		// Set the output directory
		pRecorder->setOutputLocation(QUrl::fromLocalFile(dir));
		ui.inputSaveDirectory->setText(dir);
		});

	connect(ui.checkboxAutoStop, &QCheckBox::checkStateChanged, [this](Qt::CheckState state) {
		pRecorder->setAutoStop(static_cast<bool>(state));
		});

	connect(ui.dropdownQuality, &QComboBox::currentIndexChanged, [this](int index) {
		pRecorder->setQuality(static_cast<QMediaRecorder::Quality>(index));
		});

	connect(ui.dropdownEncodingMode, &QComboBox::currentIndexChanged, [this](int index) {
		pRecorder->setEncodingMode(static_cast<QMediaRecorder::EncodingMode>(index));
		});
}

void QtRecordingControlsDialog::initVideoSignals()
{
	connect(ui.dropdownVideoCodec, &QComboBox::currentIndexChanged, [this](int index) {
		auto codec = ui.dropdownVideoCodec->itemData(index).value<QMediaFormat::VideoCodec>();
		pRecorder->mediaFormat().setVideoCodec(codec);
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

void QtRecordingControlsDialog::initAudioSignals()
{
	connect(ui.dropdownAudioCodec, &QComboBox::currentIndexChanged, [this](int index) {
		auto codec = ui.dropdownAudioCodec->itemData(index).value<QMediaFormat::AudioCodec>();
		pRecorder->mediaFormat().setAudioCodec(codec);
		});

	connect(ui.spinboxAudioBitrate, &QSpinBox::editingFinished, [this]() {
		pRecorder->setAudioBitRate(ui.spinboxAudioBitrate->value());
		});

	connect(ui.spinboxSampleRate, &QSpinBox::editingFinished, [this]() {
		pRecorder->setAudioSampleRate(ui.spinboxSampleRate->value());
		});

	connect(ui.spinboxChannels, &QSpinBox::editingFinished, [this]() {
		pRecorder->setAudioChannelCount(ui.spinboxChannels->value());
		});
}

void QtRecordingControlsDialog::initDefaultValues()
{
	
}


