#pragma once

#include <QDialog>
#include "ui_qtrecordingcontrolsdialog.h"
#include <QtMultimedia>
#include <qfiledialog.h>

class QtRecordingControlsDialog : public QDialog
{
	Q_OBJECT

public:
	QtRecordingControlsDialog(QWidget *parent = nullptr, QMediaRecorder* recorder = nullptr);
	~QtRecordingControlsDialog();

private:
	Ui::QtRecordingControlsDialogClass ui;
	QMediaRecorder* pRecorder = nullptr;
	QMediaCaptureSession* pCaptureSession = nullptr;
	QAudioInput* pAudioInput = nullptr;
	QCamera* pCamera = nullptr;

	void initWidgets();
	void initSignals();

	void initGeneralWidgets();
	void initVideoWidgets();
	void initAudioWidgets();

	void initGeneralSignals();
	void initVideoSignals();
	void initAudioSignals();

	void initDefaultValues();
};
