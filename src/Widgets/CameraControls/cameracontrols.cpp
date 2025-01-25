#include "cameracontrols.h"
#include <Windows/QtCameraControlsWindow/QtCameraControlsDialog.h>
#include <Windows/QtRecordingControlsDialog/qtrecordingcontrolsdialog.h>

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
	connect(ui.dropdownAudioDevices, &QComboBox::currentIndexChanged, [this](int index) {
		auto audioInputs = QMediaDevices::audioInputs();

		// Find and assign the input based on index
		pCamera->audioInput()->setDevice(audioInputs.at(index));
		pCamera->session()->setAudioInput(pCamera->audioInput());
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
}

void CameraControls::initWidgets()
{
	ui.dropdownVideoDevices->clear();
	ui.dropdownAudioDevices->clear();
	populateAudioDevices();

	// Update current devices /// TODO: Make dropdown usable
	if (pCamera->session()->camera() != nullptr) ui.dropdownVideoDevices->addItem(pCamera->camera()->cameraDevice().description());

	//if (pCamera->session()->audioInput() != nullptr) ui.dropdownAudioDevices->addItem(pCamera->session()->audioInput()->device().description());
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

	// Initialize signals
	initSignals();

	// Emit signal
	emit cameraChanged(camera);
}