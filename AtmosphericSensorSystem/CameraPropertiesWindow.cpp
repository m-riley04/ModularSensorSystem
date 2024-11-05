#include "CameraPropertiesWindow.h"

CameraPropertiesWindow::CameraPropertiesWindow(QWidget *parent, Camera *camera)
	: QDialog(parent), camera(camera)
{
	ui.setupUi(this);

    // Initialize
    brightnessInitial = camera->brightness();
    contrastInitial = camera->contrast();
    saturationInitial = camera->saturation();
    gainInitial = camera->gain();
    exposureInitial = camera->exposure();
    backlightInitial = camera->backlight();
    autoExposureInitial = camera->autoExposure();

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
    ui.sliderExposure->setValue(camera->exposure());
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
    connect(ui.sliderExposure, &QSlider::valueChanged, camera, &Camera::setExposure);
    connect(ui.checkboxBacklight, &QCheckBox::checkStateChanged, camera, &Camera::setBacklight);
    connect(ui.checkboxAutoExposure, &QCheckBox::checkStateChanged, camera, &Camera::setAutoExposure);

    // Buttons
    connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &CameraPropertiesWindow::dialogButtonClicked);
}

void CameraPropertiesWindow::updateDeviceDropdown() {
    ui.dropdownDevice->clear();
    QStringList deviceNames;

    for (int i = 0; i < QMediaDevices::videoInputs().size(); i++) {
        deviceNames.push_back(QMediaDevices::videoInputs()[i].description());
    }

    ui.dropdownDevice->insertItems(0, deviceNames);
}

void CameraPropertiesWindow::restoreDefaults() {
    ui.sliderBrightness->setValue(DEFAULT_BRIGHTNESS);
    ui.sliderContrast->setValue(DEFAULT_CONTRAST);
    ui.sliderSaturation->setValue(DEFAULT_SATURATION);
    ui.sliderGain->setValue(DEFAULT_GAIN);
    ui.sliderExposure->setValue(DEFAULT_EXPOSURE);
    ui.checkboxBacklight->setChecked(DEFAULT_BACKLIGHT);
    ui.checkboxAutoExposure->setChecked(DEFAULT_AUTO_EXPOSURE);
}

void CameraPropertiesWindow::restoreInitialChanges() {
    ui.sliderBrightness->setValue(brightnessInitial);
    ui.sliderContrast->setValue(contrastInitial);
    ui.sliderSaturation->setValue(saturationInitial);
    ui.sliderGain->setValue(gainInitial);
    ui.sliderExposure->setValue(exposureInitial);
    ui.checkboxBacklight->setChecked(backlightInitial);
    ui.checkboxAutoExposure->setChecked(autoExposureInitial);
}

void CameraPropertiesWindow::dialogButtonClicked(QAbstractButton* button) {

    switch (ui.buttonBox->standardButton(button)) {
    case (QDialogButtonBox::Cancel):
        this->restoreInitialChanges();
    case (QDialogButtonBox::Apply):
        this->close();
        break;
    case (QDialogButtonBox::RestoreDefaults):
        restoreDefaults();
    }
}