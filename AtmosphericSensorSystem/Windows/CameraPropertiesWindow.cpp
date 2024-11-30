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

    // Initialize the devices dropdown
    updateDeviceDropdown();
}

void CameraPropertiesWindow::initSignals() {

}

void CameraPropertiesWindow::updateDeviceDropdown() {
    ui.dropdownDevice->clear();
    QStringList deviceNames;

    for (int i = 0; i < QMediaDevices::videoInputs().size(); i++) {
        deviceNames.push_back(QMediaDevices::videoInputs()[i].description());
    }

    ui.dropdownDevice->insertItems(0, deviceNames);
}