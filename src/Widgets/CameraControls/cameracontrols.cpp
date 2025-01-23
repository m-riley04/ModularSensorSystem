#include "cameracontrols.h"
#include <Windows/QtCameraControlsWindow/QtCameraControlsDialog.h>

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

		// Connect to MainWindow
		//connect(qobject_cast<MainWindow*>(parent), &MainWindow::cameraChanged, this, &CameraControls::setCamera);

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

		// Recording controls
		connect(ui.buttonOpenOutput, &QPushButton::clicked, [this]() {
			// Open the output directory
			QDesktopServices::openUrl(QUrl::fromLocalFile(pCamera->recorder().outputLocation().path()));
			});
		connect(ui.buttonSetOutput, &QPushButton::clicked, [this]() {
			// Set the output directory
			QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory", pCamera->recorder().outputLocation().path());
			if (dir.isEmpty()) return;

			// Set the output directory
			pCamera->setMediaDirectory(QUrl::fromLocalFile(dir));
			ui.inputSaveDirectory->setText(dir);
			});
	}
}

void CameraControls::initWidgets()
{
	if (!pCamera) {
		// Set default text
		ui.labelDeviceName->setText("No Camera Selected");
		ui.inputSaveDirectory->setText("");

		// Disable widgets
		ui.inputSaveDirectory->setEnabled(false);
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

	// Update the camera settings
	ui.inputSaveDirectory->setText(pCamera->recorder().outputLocation().toString());
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