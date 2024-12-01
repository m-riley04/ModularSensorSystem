#include "AddCameraDialog.h"

void AddCameraDialog::populateDropdown()
{	
	// Get current devices
	mDevices = QMediaDevices::videoInputs();
	for (QCameraDevice device : mDevices) {
		ui.dropdownDevices->addItem(device.description());
	}
}

AddCameraDialog::AddCameraDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	populateDropdown();

	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &AddCameraDialog::transmit);
	connect(ui.dropdownDevices, &QComboBox::currentIndexChanged, this, &AddCameraDialog::findDeviceFromIndex);
}

AddCameraDialog::~AddCameraDialog()
{
	this->deleteLater();
}

void AddCameraDialog::findDeviceFromIndex(int index) {
	for (int i = 0; i < mDevices.length(); i++) {
		if (i == index) mSelectedDevice = mDevices[i];
	}
}

void AddCameraDialog::transmit(QAbstractButton* button) {
	
	auto role = ui.buttonBox->buttonRole(button);
	switch (role) {
	case (QDialogButtonBox::AcceptRole):
		emit deviceSelected(mSelectedDevice);
		break;
	default:
		break;
	}
	
	this->close();
}
