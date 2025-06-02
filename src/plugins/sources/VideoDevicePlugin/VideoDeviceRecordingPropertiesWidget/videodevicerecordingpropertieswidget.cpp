#include "videodevicerecordingpropertieswidget.h"

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

QList<QMediaFormat::FileFormat> fileFormats = {
	QMediaFormat::WMA,
	QMediaFormat::AAC,
	QMediaFormat::Matroska,
	QMediaFormat::WMV,
	QMediaFormat::MP3,
	QMediaFormat::Wave,
	QMediaFormat::Ogg,
	QMediaFormat::MPEG4,
	QMediaFormat::AVI,
	QMediaFormat::QuickTime,
	QMediaFormat::WebM,
	QMediaFormat::Mpeg4Audio,
	QMediaFormat::FLAC,
	QMediaFormat::UnspecifiedFormat
};

VideoDeviceRecordingPropertiesWidget::VideoDeviceRecordingPropertiesWidget(QWidget* parent, QMediaRecorder* recorder)
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

VideoDeviceRecordingPropertiesWidget::~VideoDeviceRecordingPropertiesWidget()
{
}

QString VideoDeviceRecordingPropertiesWidget::recorderStateDescription(QMediaRecorder::RecorderState state)
{
	switch (state) {
	case QMediaRecorder::RecorderState::PausedState:
		return "Paused";
	case QMediaRecorder::RecorderState::RecordingState:
		return "Recording";
	case QMediaRecorder::RecorderState::StoppedState:
		return "Stopped";
	}

	return QString();
}

void VideoDeviceRecordingPropertiesWidget::initWidgets()
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

void VideoDeviceRecordingPropertiesWidget::initSignals()
{
	connect(pRecorder, &QMediaRecorder::errorOccurred, [this](QMediaRecorder::Error error, QString errorString) {
		QMessageBox::warning(this, "Error #" + QString::number(error), errorString);
		});

	initGeneralSignals();
	initVideoSignals();
	initAudioSignals();
}

void VideoDeviceRecordingPropertiesWidget::initGeneralWidgets()
{
	/// FIELDS
	ui.labelRecordingStatus->setText(recorderStateDescription(pRecorder->recorderState()));
	ui.labelElapsedTime->setText(QString::number(pRecorder->duration()));
	ui.inputSaveDirectory->setText(pRecorder->outputLocation().toString());
	ui.checkboxAutoStop->setChecked(pRecorder->autoStop());
	ui.dropdownQuality->setCurrentIndex(pRecorder->quality());
	ui.dropdownEncodingMode->setCurrentIndex(pRecorder->encodingMode());

	auto supportedFormats = pRecorder->mediaFormat().supportedFileFormats(QMediaFormat::ConversionMode::Encode);
	populateEnumDropdown<QMediaFormat::FileFormat>(ui.dropdownFileFormat, fileFormats, QMediaFormat::fileFormatDescription, true, supportedFormats);
	int i = ui.dropdownFileFormat->findData(QVariant(static_cast<int>(pRecorder->mediaFormat().fileFormat())));
	ui.dropdownFileFormat->setCurrentIndex(i);
}

void VideoDeviceRecordingPropertiesWidget::initVideoWidgets()
{
	/// TAB
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::VIDEO, true);

	/// DEVICE
	ui.labelVideoDevice->setText(pCamera->cameraDevice().description());

	/// FIELDS
	auto supportedCodecs = pRecorder->mediaFormat().supportedVideoCodecs(QMediaFormat::ConversionMode::Encode);
	populateEnumDropdown<QMediaFormat::VideoCodec>(ui.dropdownVideoCodec, videoCodecs, QMediaFormat::videoCodecDescription, true, supportedCodecs);

	int i = ui.dropdownVideoCodec->findData(QVariant(static_cast<int>(pRecorder->mediaFormat().videoCodec())));
	ui.dropdownVideoCodec->setCurrentIndex(i);
	ui.spinboxBitrate->setValue(pRecorder->videoBitRate());
	ui.spinboxFrameRate->setValue(pRecorder->videoFrameRate());
	ui.spinboxWidth->setValue(pRecorder->videoResolution().width());
	ui.spinboxHeight->setValue(pRecorder->videoResolution().height());
}

void VideoDeviceRecordingPropertiesWidget::initAudioWidgets()
{
	/// TAB
	ui.tabs->tabBar()->setTabEnabled(RecordingControlsTab::AUDIO, true);

	/// DEVICES
	ui.labelAudioDevice->setText(pAudioInput->device().description());

	/// FIELDS
	auto supportedCodecs = pRecorder->mediaFormat().supportedAudioCodecs(QMediaFormat::ConversionMode::Encode);
	populateEnumDropdown<QMediaFormat::AudioCodec>(ui.dropdownAudioCodec, audioCodecs, QMediaFormat::audioCodecDescription, true, supportedCodecs);

	int i = ui.dropdownAudioCodec->findData(QVariant(static_cast<int>(pRecorder->mediaFormat().audioCodec())));
	ui.dropdownAudioCodec->setCurrentIndex(i);
	ui.spinboxAudioBitrate->setValue(pRecorder->audioBitRate());
	ui.spinboxSampleRate->setValue(pRecorder->audioSampleRate());
	ui.spinboxChannels->setValue(pRecorder->audioChannelCount());
}

void VideoDeviceRecordingPropertiesWidget::initGeneralSignals()
{
	connect(pRecorder, &QMediaRecorder::recorderStateChanged, [this](QMediaRecorder::RecorderState state) {
		ui.labelRecordingStatus->setText(recorderStateDescription(state));
		});

	connect(pRecorder, &QMediaRecorder::durationChanged, [this](qint64 duration) {
		ui.labelElapsedTime->setText(QString::number(duration));
		});

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

	connect(ui.dropdownFileFormat, &QComboBox::currentIndexChanged, [this](int index) {
		auto fileFormat = ui.dropdownFileFormat->itemData(index).value<QMediaFormat::FileFormat>();
		pRecorder->mediaFormat().setFileFormat(fileFormat);
		});
}

void VideoDeviceRecordingPropertiesWidget::initVideoSignals()
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

void VideoDeviceRecordingPropertiesWidget::initAudioSignals()
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

void VideoDeviceRecordingPropertiesWidget::initDefaultValues()
{

}
