#include "QtCameraControlsDialog.h"

QtCameraControlsDialog::QtCameraControlsDialog(Camera* camera, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

QtCameraControlsDialog::~QtCameraControlsDialog()
{}
