#pragma once

#include <QDialog>
#include "ui_AddCameraDialog.h"
#include <QtMultimedia>

class AddCameraDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::AddCameraDialogClass ui;
	QList<QCameraDevice> mDevices;
	QList<QCameraDevice> mCurrentCameras;
	QCameraDevice mSelectedDevice;

	void populateDropdown();

public:
	AddCameraDialog(QWidget* parent = nullptr, QList<QCameraDevice> currentCameras = {});
	~AddCameraDialog();

public slots:
	void transmit(QAbstractButton *button);
	void findDeviceFromIndex(int index);
	void updateDeviceList(QList<QCameraDevice> devices);

signals:
	void deviceSelected(QCameraDevice device);

};
