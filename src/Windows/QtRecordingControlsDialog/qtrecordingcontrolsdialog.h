#pragma once

#include <QDialog>
#include "ui_qtrecordingcontrolsdialog.h"
#include <QtMultimedia>
#include <qfiledialog.h>
#include <QMessageBox>

enum RecordingControlsTab {
	GENERAL,
	VIDEO,
	AUDIO
};

class QtRecordingControlsDialog : public QDialog
{
	Q_OBJECT

public:
	QtRecordingControlsDialog(QWidget *parent = nullptr, QMediaRecorder* recorder = nullptr);
	~QtRecordingControlsDialog();

	static QString recorderStateDescription(QMediaRecorder::RecorderState state);

private:
	Ui::QtRecordingControlsDialogClass ui;
	QMediaRecorder* pRecorder = nullptr;
	QMediaCaptureSession* pCaptureSession = nullptr;
	QAudioInput* pAudioInput = nullptr;
	QCamera* pCamera = nullptr;

	void initWidgets();
	void initSignals();

	template <typename Enum>
	void populateEnumDropdown(QComboBox* dropdown, QList<Enum> enumList, std::function<QString(Enum)> enumTextFunction, bool clearDropdown = false);

	void initGeneralWidgets();
	void initVideoWidgets();
	void initAudioWidgets();

	void initGeneralSignals();
	void initVideoSignals();
	void initAudioSignals();

	void initDefaultValues();
};

template<typename Enum>
inline void QtRecordingControlsDialog::populateEnumDropdown(QComboBox* dropdown, QList<Enum> enumList, std::function<QString(Enum)> enumTextFunction, bool clearDropdown)
{
	if (clearDropdown) dropdown->clear();

	for (Enum codec : enumList) {
		dropdown->addItem(enumTextFunction(codec), QVariant(static_cast<int>(codec)));
	}
}
