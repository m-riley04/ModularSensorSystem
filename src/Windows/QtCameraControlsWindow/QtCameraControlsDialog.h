#pragma once

#include <QDialog>
#include "ui_QtCameraControlsDialog.h"
#include "../../Sensors/Camera.h"

class QtCameraControlsDialog : public QDialog
{
	Q_OBJECT

public:
	QtCameraControlsDialog(Camera* camera, QWidget *parent = nullptr);
	~QtCameraControlsDialog();

private:
	Ui::QtCameraControlsDialogClass ui;
};
