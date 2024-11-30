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
	QCameraDevice mSelectedDevice;

	void populateDropdown();

public:
	AddCameraDialog(QWidget *parent = nullptr);
	~AddCameraDialog();

public slots:
	void transmit(QAbstractButton *button);
	void findDeviceFromIndex(int index);

signals:
	void deviceSelected(QCameraDevice device);

};
