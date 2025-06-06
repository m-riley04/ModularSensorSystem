#pragma once

#include <QDialog>
#include "ui_usbvideorecordingpropertieswidget.h"
#include <QtMultimedia>
#include <qfiledialog.h>
#include <QMessageBox>

enum RecordingControlsTab {
	GENERAL,
	VIDEO,
	AUDIO
};

class USBVideoRecordingPropertiesWidget : public QDialog
{
	Q_OBJECT

public:
	USBVideoRecordingPropertiesWidget(QWidget* parent = nullptr, QMediaRecorder* recorder = nullptr);
	~USBVideoRecordingPropertiesWidget();

	static QString recorderStateDescription(QMediaRecorder::RecorderState state);

private:
	Ui::USBVideoRecordingPropertiesWidgetClass ui;
	QMediaRecorder* pRecorder = nullptr;
	QMediaCaptureSession* pCaptureSession = nullptr;
	QAudioInput* pAudioInput = nullptr;
	QCamera* pCamera = nullptr;

	void initWidgets();
	void initSignals();

	template <typename Enum>
	void populateEnumDropdown(QComboBox* dropdown, QList<Enum> enumList,
		std::function<QString(Enum)> enumTextFunction, bool clearDropdown = false, QList<Enum> allowedEnums = {});

	void initGeneralWidgets();
	void initVideoWidgets();
	void initAudioWidgets();

	void initGeneralSignals();
	void initVideoSignals();
	void initAudioSignals();

	void initDefaultValues();
};

template<typename Enum>
inline void USBVideoRecordingPropertiesWidget::populateEnumDropdown(QComboBox* dropdown, QList<Enum> enumList,
	std::function<QString(Enum)> enumTextFunction, bool clearDropdown, QList<Enum> allowedEnums)
{
	if (clearDropdown) dropdown->clear();

	for (Enum codec : enumList) {
		if (allowedEnums.contains(codec)) dropdown->addItem(enumTextFunction(codec), QVariant(static_cast<int>(codec)));
	}
}
