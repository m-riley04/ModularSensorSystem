#include "QtCameraControlsDialog.h"

QtCameraControlsDialog::QtCameraControlsDialog(Camera* camera, QWidget* parent)
	: QDialog(parent), pCamera(camera)
{
	ui.setupUi(this);

	// Connect controls
	connect(ui.checkboxActive, &QCheckBox::checkStateChanged, camera, [camera](Qt::CheckState state) {
		camera->setActive(state == Qt::CheckState::Checked);
		});
	connect(ui.sliderColorTemp, &QSlider::valueChanged, camera, &Camera::setColorTemperature);
	connect(ui.dropdownExposureMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->setExposureMode(static_cast<QCamera::ExposureMode>(index));
		});
	connect(ui.sliderExposureComp, &QSlider::valueChanged, camera, &Camera::setExposureCompensation);
	connect(ui.dropdownFlashMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->setFlashMode(static_cast<QCamera::FlashMode>(index));
		});
	connect(ui.checkboxAutoExposureTime, &QCheckBox::checkStateChanged, camera, [camera](Qt::CheckState state) {
		camera->setAutoExposureTime(state == Qt::CheckState::Checked);
		});
	connect(ui.sliderManualExposureTime, &QSlider::valueChanged, camera, &Camera::setManualExposureTime);
	connect(ui.checkboxAutoIsoSensitivity, &QCheckBox::checkStateChanged, camera, [camera](Qt::CheckState state) {
		camera->setAutoIsoSensitivity(state == Qt::CheckState::Checked);
		});
	connect(ui.sliderManualIsoSensitivity, &QSlider::valueChanged, camera, &Camera::setManualIsoSensitivity);
	connect(ui.dropdownTorchMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->setTorchMode(static_cast<QCamera::TorchMode>(index));
		});
	connect(ui.dropdownWhiteBalanceMode, &QComboBox::currentIndexChanged, camera, [camera](int index) {
		camera->setWhiteBalanceMode(static_cast<QCamera::WhiteBalanceMode>(index));
		});

}

QtCameraControlsDialog::~QtCameraControlsDialog()
{}
