#include "cameracontrols.h"
#include <Windows/QtCameraControlsWindow/QtCameraControlsDialog.h>
#include <Windows/MainWindow.h>

CameraControls::CameraControls(QWidget *parent, Camera* camera)
	: QWidget(parent), pCamera(camera)
{
	ui.setupUi(this);

	if (pCamera)
	{
		ui.labelDeviceName->setText(pCamera->camera().cameraDevice().description());

		// Connect to MainWindow
		connect(parent, &MainWindow::cameraChanged, this, CameraControls::setCamera);

		// Camera Controls
		connect(ui.buttonStart, &QPushButton::clicked, pCamera, &Camera::start);
		connect(ui.buttonStop, &QPushButton::clicked, pCamera, &Camera::stop);

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
		connect(ui.inputSaveDirectory, &QLineEdit::textChanged, pCamera, &Camera::setMediaDirectory);
	}

	
}

CameraControls::~CameraControls()
{}


void CameraControls::setCamera(Camera* camera) {
	// Set camera
	pCamera = camera;

	// Update the camera name
	ui.labelDeviceName->setText(pCamera->camera().cameraDevice().description());

	// Update the camera settings
	ui.inputSaveDirectory->setText(pCamera->recorder().outputLocation().toString());

	// Emit signal
	emit cameraChanged(camera);
}