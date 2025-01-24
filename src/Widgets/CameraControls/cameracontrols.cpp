#include "cameracontrols.h"
#include <Windows/QtCameraControlsWindow/QtCameraControlsDialog.h>
#include <Windows/QtRecordingControlsDialog/qtrecordingcontrolsdialog.h>

CameraControls::CameraControls(QWidget* parent, Camera* camera)
	: QWidget(parent), pCamera(camera)
{
	ui.setupUi(this);
}

CameraControls::~CameraControls()
{}

void CameraControls::initSignals()
{
	if (pCamera)
	{
		ui.labelDeviceName->setText(pCamera->camera().cameraDevice().description());

		// Camera Controls
		connect(ui.buttonStart, &QPushButton::clicked, pCamera, &Camera::start);
		connect(ui.buttonStop, &QPushButton::clicked, pCamera, &Camera::stop);
		connect(ui.buttonRestart, &QPushButton::clicked, pCamera, &Camera::restart);

		// Camera settings
		connect(ui.buttonCameraSettings, &QPushButton::clicked, [this]() {
			if (!pCamera) {
				QMessageBox::warning(this, "Error", "No camera available to configure.");
				return;
			}

			// Create the settings box
			QtCameraControlsDialog* controls = new QtCameraControlsDialog(&pCamera->camera(), &pCamera->session(), this);
			controls->setAttribute(Qt::WA_DeleteOnClose);
			controls->show();
			});

		// Recording settings
		connect(ui.buttonRecordingSettings, &QPushButton::clicked, [this]() {
			if (!pCamera) {
				QMessageBox::warning(this, "Error", "No camera available to configure.");
				return;
			}

			// Create the settings box
			QtRecordingControlsDialog* controls = new QtRecordingControlsDialog(this, &pCamera->recorder());
			controls->setAttribute(Qt::WA_DeleteOnClose);
			controls->show();
			});
	}
}

void CameraControls::initWidgets()
{
	if (!pCamera) {
		// Set default text
		ui.labelDeviceName->setText("No Camera Selected");

		// Disable widgets
		ui.buttonStart->setEnabled(false);
		ui.buttonStop->setEnabled(false);
		ui.buttonCameraSettings->setEnabled(false);
		return;
	}

	// Enable buttons
	ui.buttonStart->setEnabled(true);
	ui.buttonStop->setEnabled(true);
	ui.buttonCameraSettings->setEnabled(true);

	// Update device name
	ui.labelDeviceName->setText(pCamera->camera().cameraDevice().description());
}


void CameraControls::setCamera(Camera* camera) {
	// Set camera
	pCamera = camera;

	// Init widgets
	initWidgets();

	// Initialize signals
	initSignals();

	// Emit signal
	emit cameraChanged(camera);
}