#include "cameracontrols.h"

CameraControls::CameraControls(QWidget* parent, CameraDevice* camera)
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
	connect(ui.buttonStart, &QPushButton::clicked, pCamera, &CameraDevice::start);
	connect(ui.buttonStop, &QPushButton::clicked, pCamera, &CameraDevice::stop);
	connect(ui.buttonRestart, &QPushButton::clicked, pCamera, &CameraDevice::restart);

	// Devices
	connect(ui.checkboxObjectDetection, &QCheckBox::checkStateChanged, [this](int state) {
		if (!pCamera) return;
		
		// Find the current SinkView
		auto videoWidget = pCamera->sinkView();

		// Check if sinkView is valid
		if (!videoWidget) {
			QMessageBox::warning(this, "Error", "No video widget available", QMessageBox::StandardButton::Ok);
			return;
		}

		// Set the detection state
		videoWidget->setDetectionState(state == Qt::Checked);
		});

	// Connect debug checkbox to current SinkView's debug state
	connect(ui.checkboxDebugInfo, &QCheckBox::checkStateChanged, [this](int state) {
		if (!pCamera) return;

		// Find the current SinkView
		auto videoWidget = pCamera->sinkView();

		// Check if sinkView is valid
		if (!videoWidget) {
			QMessageBox::warning(this, "Error", "No video widget available", QMessageBox::StandardButton::Ok);
			return;
		}

		// Set the debug state
		videoWidget->setDebugInfoVisible(state == Qt::Checked);
		});

	// Connect model changing button
	connect(ui.buttonLoadModel, &QPushButton::clicked, [this]() {
		if (!pCamera) return;

		// Get the new model path from a file dialog
		QUrl newModelPath = QFileDialog::getOpenFileUrl(this, "Select YOLO Model", QString(), "Model Files (*.onnx);;All Files (*)", nullptr, QFileDialog::Option::DontUseNativeDialog);

		// Find the current SinkView
		auto videoWidget = pCamera->sinkView();

		// Check if sinkView is valid
		if (!videoWidget) {
			QMessageBox::warning(this, "Error", "No video widget available", QMessageBox::StandardButton::Ok);
			return;
		}

		if (videoWidget->yolo() != nullptr) {
			// Set the new model path
			videoWidget->yolo()->setModel(newModelPath.toLocalFile());
		}
		else {
			QMessageBox::warning(this, "Error", "YOLO not initialized", QMessageBox::StandardButton::Ok);
		}
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
	double available = UnitConversionHelper::bytesToKilobytes(availableBytes);
	label = QString::number(available) + "KB";

	// Check if it's bigger than a megabyte
	if (available > InBytes::KILOBYTE) {
		available = UnitConversionHelper::bytesToMegabytes(availableBytes);
		label = QString::number(available) + "MB";
	}

	// Check if it's bigger than a gigabyte
	if (available > InBytes::KILOBYTE) {
		available = UnitConversionHelper::bytesToGigabytes(availableBytes);
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

void CameraControls::setCamera(CameraDevice* camera) {
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