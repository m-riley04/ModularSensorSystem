#include "cameracontrols.h"

double bytesToKilobytes(qint64 bytes) {
	return bytes / static_cast<double>(1024);
}

double bytesToMegabytes(qint64 bytes) {
	return bytes / static_cast<double>(1024 * 1024);
}

double bytesToGigabytes(qint64 bytes) {
	return bytes / static_cast<double>(1024 * 1024 * 1024);
}

CameraControls::CameraControls(QWidget* parent, Camera* camera)
	: QWidget(parent), pCamera(camera)
{
	ui.setupUi(this);

	if (!pCamera)
	{
		// Disable
		this->setEnabled(false);
		return;
	}

	initWidgets();
	initSignals();
}

CameraControls::~CameraControls()
{}

void CameraControls::initSignals()
{
	// Camera Controls
	connect(ui.buttonStart, &QPushButton::clicked, pCamera, &Camera::start);
	connect(ui.buttonStop, &QPushButton::clicked, pCamera, &Camera::stop);
	connect(ui.buttonRestart, &QPushButton::clicked, pCamera, &Camera::restart);

	// Devices
	connect(ui.checkboxVideoProcessing, &QCheckBox::checkStateChanged, [this](Qt::CheckState state) {
		pCamera->changeVideoInputMethod(state == Qt::CheckState::Checked ? VideoInputMethod::QVIDEOFRAMEINPUT : VideoInputMethod::QCAMERA);
		});

	connect(ui.dropdownAudioDevices, &QComboBox::currentIndexChanged, [this](int index) {
		auto audioInputs = QMediaDevices::audioInputs();

		// Find and assign the input based on index
		pCamera->audioInput()->setDevice(audioInputs.at(index));
		pCamera->session()->setAudioInput(pCamera->audioInput());
		});

	// Storage
	connect(pCamera->recorder(), &QMediaRecorder::durationChanged, [this](qint64 duration) {
		storageInfo.refresh();
		updateStorageLabel(storageInfo.bytesAvailable());
		});

	// Camera settings
	connect(ui.buttonCameraSettings, &QPushButton::clicked, [this]() {
		if (!pCamera) {
			QMessageBox::warning(this, "Error", "No camera available to configure.");
			return;
		}

		// Create the settings box
		QtCameraControlsDialog* controls = new QtCameraControlsDialog(pCamera->camera(), pCamera->session(), this);
		controls->show();
		});

	// Recording settings
	connect(ui.buttonRecordingSettings, &QPushButton::clicked, [this]() {
		if (!pCamera) {
			QMessageBox::warning(this, "Error", "No camera available to configure.");
			return;
		}

		// Create the settings box
		QtRecordingControlsDialog* controls = new QtRecordingControlsDialog(this, pCamera->recorder());
		controls->show();
		});

	// Set signals init flag
	areSignalsInitialized = true;
}

void CameraControls::initWidgets()
{
	ui.dropdownVideoDevices->clear();
	ui.dropdownAudioDevices->clear();
	populateAudioDevices();

	// init storage label
	storageInfo.setPath(pCamera->recorder()->outputLocation().toString());
	updateStorageLabel(storageInfo.bytesAvailable());

	// Update current devices /// TODO: Make dropdown usable
	if (pCamera->session()->camera() != nullptr) ui.dropdownVideoDevices->addItem(pCamera->camera()->cameraDevice().description());

	//if (pCamera->session()->audioInput() != nullptr) ui.dropdownAudioDevices->addItem(pCamera->session()->audioInput()->device().description());
}

void CameraControls::updateStorageLabel(qint64 availableBytes)
{
	QString label = QString::number(availableBytes) + "B";

	// Get kilobytes
	double available = bytesToKilobytes(availableBytes);
	label = QString::number(available) + "KB";

	// Check if it's bigger than a megabyte
	if (available > 1024) {
		available = bytesToMegabytes(availableBytes);
		label = QString::number(available) + "MB";
	}

	// Check if it's bigger than a gigabyte
	if (available > 1024) {
		available = bytesToGigabytes(availableBytes);
		label = QString::number(available) + "GB";
	}

	ui.labelOutputStorage->setText(label);
}

void CameraControls::populateAudioDevices()
{
	ui.dropdownAudioDevices->blockSignals(true);

	ui.dropdownAudioDevices->clear();

	for (auto device : QMediaDevices::audioInputs()) {
		ui.dropdownAudioDevices->addItem(device.description());
	}

	// Set to unselected
	ui.dropdownAudioDevices->setCurrentIndex(-1);

	ui.dropdownAudioDevices->blockSignals(false);
}

void CameraControls::setCamera(Camera* camera) {
	// Set camera
	pCamera = camera;

	// Check if enabled
	if (!pCamera)
	{
		this->setEnabled(false);
		return;
	}

	this->setEnabled(true);

	// Init widgets
	initWidgets();

	if (!areSignalsInitialized) initSignals();

	// Emit signal
	emit cameraChanged(camera);
}