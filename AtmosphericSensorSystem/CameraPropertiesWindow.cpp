#include "CameraPropertiesWindow.h"

CameraPropertiesWindow::CameraPropertiesWindow(QWidget *parent, Camera *camera)
	: QDialog(parent), camera(camera)
{
	ui.setupUi(this);
    initWidgets();
    initSignals();
}

CameraPropertiesWindow::~CameraPropertiesWindow()
{}

void CameraPropertiesWindow::initWidgets() {
    

    // Initialize the display settings
    ui.sliderBrightness->setValue(camera->brightness());
    ui.sliderContrast->setValue(camera->contrast());
    ui.sliderSaturation->setValue(camera->saturation());
    ui.sliderGain->setValue(camera->gain());
    ui.checkboxBacklight->setChecked(camera->backlight());
    ui.checkboxAutoExposure->setChecked(camera->autoExposure());

    // Initialize the devices dropdown
    updateDeviceDropdown();
}

void CameraPropertiesWindow::initSignals() {
    // Dropdown
    connect(ui.dropdownDevice, &QComboBox::currentIndexChanged, camera, &Camera::setVideoDevice);

    // Sliders
    connect(ui.sliderBrightness, &QSlider::valueChanged, camera, &Camera::setBrightness);
    connect(ui.sliderContrast, &QSlider::valueChanged, camera, &Camera::setContrast);
    connect(ui.sliderSaturation, &QSlider::valueChanged, camera, &Camera::setSaturation);
    connect(ui.sliderGain, &QSlider::valueChanged, camera, &Camera::setGain);
    connect(ui.checkboxBacklight, &QCheckBox::checkStateChanged, camera, &Camera::setBacklight);
    connect(ui.checkboxAutoExposure, &QCheckBox::checkStateChanged, camera, &Camera::setAutoExposure);
}

void CameraPropertiesWindow::updateDeviceDropdown() {
    ui.dropdownDevice->clear();
    QStringList deviceNames;

    for (int i = 0; i < 10; i++) {
        cv::VideoCapture _tempCam(i);
        if (_tempCam.isOpened()) {
            deviceNames.push_back(QString("Camera %1").arg(i));
            _tempCam.release();
        }
    }

    ui.dropdownDevice->insertItems(0, deviceNames);
}