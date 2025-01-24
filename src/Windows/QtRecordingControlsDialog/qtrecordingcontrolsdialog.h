#pragma once

#include <QDialog>
#include "ui_qtrecordingcontrolsdialog.h"
#include <QtMultimedia>
#include <qfiledialog.h>

class QtRecordingControlsDialog : public QDialog
{
	Q_OBJECT

public:
	QtRecordingControlsDialog(QWidget *parent = nullptr, QMediaRecorder* recorder = nullptr, QCameraDevice device = QCameraDevice());
	~QtRecordingControlsDialog();

private:
	Ui::QtRecordingControlsDialogClass ui;
	QMediaRecorder* pRecorder;
	QCameraDevice mDevice;

	void initWidgets();
	void initSignals();
};
