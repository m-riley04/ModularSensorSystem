#include "AddCameraDialog.h"

AddCameraDialog::AddCameraDialog(QWidget *parent, QList<QCameraDevice> currentCameras)
	: QDialog(parent), mCurrentCameras(currentCameras)
{
	ui.setupUi(this);

	populateDropdown();

	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &AddCameraDialog::transmit);
	connect(ui.dropdownDevices, &QComboBox::currentIndexChanged, this, &AddCameraDialog::findDeviceFromIndex);

	// Refresh dropdown
	connect(ui.buttonRefresh, &QPushButton::clicked, this, [this]() {
		ui.dropdownDevices->clear();
		populateDropdown();
		});
}

AddCameraDialog::~AddCameraDialog()
{}

void AddCameraDialog::populateDropdown()
{
	mDevices.clear();

	// Get current devices
	for (QCameraDevice device : QMediaDevices::videoInputs()) {
		// Check if device is already in current device list
		//if (!mCurrentCameras.contains(device)) {
			ui.dropdownDevices->addItem(device.description(), device.id());
			mDevices.append(device);
		//}
	}

	// Check if there are any devices
	if (mDevices.isEmpty()) return;

	// Set default selected device
	mSelectedDevice = mDevices[0];
}

void AddCameraDialog::findDeviceFromIndex(int index) {
	for (int i = 0; i < mDevices.length(); i++) {
		if (i == index) mSelectedDevice = mDevices[i];
	}
}

void AddCameraDialog::updateDeviceList(QList<QCameraDevice> devices)
{
	mDevices = devices;
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
